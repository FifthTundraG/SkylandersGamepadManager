// SPDX-License-Identifier: GPL-3.0-or-later
/*
 *  SkylandersGamepadManager
 *  Copyright (c) 2026 FifthTundraG
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorperates work covered by the following copyright and
 * permission notice:
 *
 *      skylanders-gamepad-daemon - Userspace daemon
 *      Copyright (c) 2025 FifthTundraG
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, version 3.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 */

#include <stdexcept>
#include <QString>
#include <QDebug>
#include "VirtualInputDeviceLinux.h"

// MARK: VirtualInputDeviceLinux
VirtualInputDeviceLinux::VirtualInputDeviceLinux(const QString &deviceName)
{
    libevdev *dev = libevdev_new();
    if (!dev) {
        throw std::runtime_error("Failed to allocate libevdev");
    }

    libevdev_set_name(dev, deviceName.toUtf8().constData());

    libevdev_enable_event_type(dev, EV_KEY);
    libevdev_enable_event_code(dev, EV_KEY, BTN_A, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_B, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_X, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_Y, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_TL, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_TR, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_TL2, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_TR2, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_DPAD_UP, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_DPAD_DOWN, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_DPAD_LEFT, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_DPAD_RIGHT, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_START, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_SELECT, NULL);

    libevdev_enable_event_type(dev, EV_ABS);
    struct input_absinfo abs = { .minimum = -128, .maximum = 127 };
    libevdev_enable_event_code(dev, EV_ABS, ABS_X, &abs);
    libevdev_enable_event_code(dev, EV_ABS, ABS_Y, &abs);
    libevdev_enable_event_code(dev, EV_ABS, ABS_RX, &abs);
    libevdev_enable_event_code(dev, EV_ABS, ABS_RY, &abs);

    if (libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &m_uinput) < 0) {
        libevdev_free(dev);
        throw std::runtime_error("Failed to create uinput device");
    }

    libevdev_free(dev);

    m_devicePath = QString::fromUtf8(libevdev_uinput_get_devnode(m_uinput));

    qInfo() << "Virtual input device created at " << m_devicePath;
}

VirtualInputDeviceLinux::~VirtualInputDeviceLinux()
{
    if (m_uinput) {
        libevdev_uinput_destroy(m_uinput);
        m_uinput = nullptr;
    }
}

bool VirtualInputDeviceLinux::writeButtonEvent(uint buttonCode, bool pressed)
{
    if (!m_uinput) return false;

    return libevdev_uinput_write_event(m_uinput, EV_KEY, buttonCode, pressed ? 1 : 0) == 0;
}

bool VirtualInputDeviceLinux::writeAxisEvent(uint axisCode, qint16 value)
{
    if (!m_uinput) return false;

    return libevdev_uinput_write_event(m_uinput, EV_ABS, axisCode, value) == 0;
}

bool VirtualInputDeviceLinux::sync()
{
    if (!m_uinput) return false;

    return libevdev_uinput_write_event(m_uinput, EV_SYN, SYN_REPORT, 0) == 0;
}

QString VirtualInputDeviceLinux::getDevicePath() const
{
    return m_devicePath;
}

// MARK: VirtualInputFactory
VirtualInputDevice* VirtualInputDeviceFactory::create(const QString &deviceName)
{
    return new VirtualInputDeviceLinux(deviceName);
}
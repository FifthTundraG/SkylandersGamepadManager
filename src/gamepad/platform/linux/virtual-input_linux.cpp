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
 */

#include <stdexcept>
#include <QString>
#include <QDebug>
#include "virtual-input_linux.h"

// MARK: LinuxVirtualInputDevice
LinuxVirtualInputDevice::LinuxVirtualInputDevice(const QString &deviceName)
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

    if (libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &m_dev) < 0) {
        libevdev_free(dev);
        throw std::runtime_error("Failed to create uinput device");
    }

    libevdev_free(dev);

    qInfo() << "Virtual input device created at " << libevdev_uinput_get_devnode(m_dev);
}

LinuxVirtualInputDevice::~LinuxVirtualInputDevice()
{
    if (m_dev) {
        libevdev_uinput_destroy(m_dev);
    }
}

int LinuxVirtualInputDevice::writeButtonEvent(uint buttonCode, bool pressed)
{
    return libevdev_uinput_write_event(m_dev, EV_KEY, buttonCode, pressed);
}

int LinuxVirtualInputDevice::writeAxisEvent(uint axis_code, qint16 value)
{
    return libevdev_uinput_write_event(m_dev, EV_ABS, axis_code, value);
}

int LinuxVirtualInputDevice::sync()
{
    return libevdev_uinput_write_event(m_dev, EV_SYN, SYN_REPORT, 0);
}

QString LinuxVirtualInputDevice::getDevicePath() const
{
    return QString::fromUtf8(libevdev_uinput_get_devnode(m_dev));
}

// MARK: LinuxVirtualInputFactory
VirtualInputFactory *getVirtualInputFactory()
{
    static LinuxVirtualInputFactory factory;
    return &factory;
}
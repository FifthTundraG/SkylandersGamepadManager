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

#pragma once

#include "../virtual-input.h"

#include <libevdev/libevdev-uinput.h>

class LinuxVirtualInputDevice : public VirtualInputDevice {
public:
    explicit LinuxVirtualInputDevice(const QString &deviceName);
    ~LinuxVirtualInputDevice() override;

    /**
     * Writes a button event to the virtual input device.
     *
     * @param buttonCode Event code (ABS_X, REL_Y, etc.)
     * @param pressed The event value
     *
     * @returns 0 on success or a negative errno on error
     */
    int writeButtonEvent(uint buttonCode, bool pressed) override;
    /**
     * Writes an axis event to the virtual input device.
     *
     * @param axisCode Event code (ABS_X, REL_Y, etc.)
     * @param value The event value
     *
     * @returns 0 on success or a negative errno on error
     */
    int writeAxisEvent(uint axisCode, qint16 value) override;
    /**
     * Writes a sync event to the virtual input device.
     *
     * @returns 0 on success or a negative errno on error
     */
    int sync() override;
    /**
     * @returns The device node for this device, in the form of /dev/input/eventN
     */
    QString getDevicePath() const override;

private:
    libevdev_uinput *m_dev = nullptr;
};

class LinuxVirtualInputFactory
    : public VirtualInputFactory
{
public:
    std::unique_ptr<VirtualInputDevice> createDevice(const QString &deviceName) override
    {
        return std::make_unique<LinuxVirtualInputDevice>(deviceName);
    }
};
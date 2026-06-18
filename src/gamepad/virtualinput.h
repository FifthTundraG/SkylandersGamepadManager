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

#include <QtGlobal>
#include <memory>

class VirtualInputDevice {
public:
    virtual ~VirtualInputDevice() = default;

    virtual bool writeButtonEvent(uint buttonCode, bool pressed) = 0;
    virtual bool writeAxisEvent(uint axisCode, qint16 value) = 0;
    virtual bool sync() = 0;
    virtual QString getDevicePath() const = 0;
};


class VirtualInputFactory {
public:
    static std::unique_ptr<VirtualInputDevice> create(const QString &deviceName);
};
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

#include <stdbool.h>
#include <QObject>

class DeviceDiscovery : public QObject {
    Q_OBJECT

public:
    explicit DeviceDiscovery(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~DeviceDiscovery() = default;

    /**
     * Search **all connected devices** for gamepads
     *
     * Note: this likely has a notable performance impact. It should probably not be used for passive scanning unless the platform requires it.
     */
    virtual QStringList findGamepads() = 0;

    virtual bool enablePassiveScanning() = 0;

signals:
    void deviceConnected(const QString &devicePath);
    void deviceDisconnected(const QString &devicePath);
};

class DeviceDiscoveryFactory {
public:
    static DeviceDiscovery* create(QObject *parent = nullptr);
};
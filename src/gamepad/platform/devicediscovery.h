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

    virtual QStringList findGamepads() = 0;
    virtual bool isConnected(const QString &devicePath) = 0;

    virtual bool startNotify(const QString &characteristicPath) = 0;
    virtual bool stopNotify(const QString &characteristicPath) = 0;

    virtual void setup() = 0;
    virtual void cleanup() = 0;

signals:
    void deviceConnected(const QString &devicePath);
    void deviceDisconnected(const QString &devicePath);
};

class DeviceDiscoveryFactory {
public:
    static DeviceDiscovery *create(QObject *parent = nullptr);
};
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

#include "../device-discovery.h"
#include <QObject>
#include <QtDBus/QDBusConnection>

class DeviceDiscoveryLinux : public DeviceDiscovery {
    Q_OBJECT

public:
    explicit DeviceDiscoveryLinux(QObject *parent = nullptr);

    QStringList findGamepads() override;
    bool isConnected(const QString &devicePath) override;

    bool startNotify(const QString &characteristicPath) override;
    bool stopNotify(const QString &characteristicPath) override;

    void setup() override;
    void cleanup() override;

private:
    QDBusConnection *m_connection = nullptr;
};
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

#include "../DeviceDiscovery.h"
#include <QObject>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusContext>

class DeviceDiscoveryLinux : public DeviceDiscovery, protected QDBusContext {
    Q_OBJECT

public:
    explicit DeviceDiscoveryLinux(QObject *parent = nullptr);

    QStringList findGamepads() override;

    bool enablePassiveScanning() override;

private slots:
    void onInterfacesAdded(const QDBusObjectPath &path, const QMap<QString, QVariantMap> &interfaces);
    void onInterfacesRemoved(const QDBusObjectPath &path, const QMap<QString, QVariantMap> &interfaces);

    void onPropertiesChangedOnKnownDevice(const QString &interface, const QVariantMap &changedProperties, const QStringList &invalidatedProperties);

private:
    QDBusConnection m_connection;

    /**
     * List of known Skylanders GamePad devices detected from an interface being added with a name that matches our criteria, but it is NOT connected yet.
     */
    QList<QString> m_knownDevices;
};
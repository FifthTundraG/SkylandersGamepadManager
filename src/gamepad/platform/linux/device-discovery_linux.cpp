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

#include "device-discovery_linux.h"
#include "../../gamepad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusReply>
#include <QDebug>

DeviceDiscoveryLinux::DeviceDiscoveryLinux(QObject *parent)
    : DeviceDiscovery(parent)
{}

void DeviceDiscoveryLinux::setup()
{
    m_connection = new QDBusConnection(QDBusConnection::systemBus());

    if (!m_connection->isConnected()) {
        qCritical() << "Failed to connect to system D-Bus:"
                    << m_connection->lastError().message();
        delete m_connection;
        m_connection = nullptr;
        return;
    }

    qInfo() << "Connected to system D-Bus";
}

void DeviceDiscoveryLinux::cleanup()
{
    if (m_connection) {
        delete m_connection;
        m_connection = nullptr;
    }
}

QStringList DeviceDiscoveryLinux::findGamepads(void)
{
    qInfo() << "Searching for gamepad devices...";

    QStringList results;

    QDBusInterface objectManager(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        *m_connection
    );

    QDBusReply<QMap<QDBusObjectPath, QMap<QString, QVariantMap>>> reply = objectManager.call("GetManagedObjects");

    if (!reply.isValid()) {
        qCritical() << "Failed to get BlueZ managed objects: " << reply.error().message();
        return results;
    }

    const auto objects = reply.value();

    for (auto it = objects.begin(); it != objects.end(); ++it) {
        const QString objectPath = it.key().path(); 
        const auto interfaces = it.value();

        if (!interfaces.contains("org.bluez.Device1")) // we only care about Device1
            continue;

        const QVariantMap deviceProps = interfaces["org.bluez.Device1"];

        const QString name = deviceProps.value("Name").toString();
        const bool connected = deviceProps.value("Connected").toBool();

        if (connected && name == DEVICE_NAME) {
            qInfo() << "Found connected gamepad:" << objectPath;
            results.append(objectPath);
        }
    }

    return results;
}

bool DeviceDiscoveryLinux::isConnected(const QString &devicePath)
{
    QDBusInterface device(
        "org.bluez",
        devicePath,
        "org.freedesktop.DBus.Properties",
        *m_connection
    );

    QDBusReply<QVariant> reply = device.call(
        "Get",
        "org.bluez.Device1",
        "Connected"
    );

    if (!reply.isValid()) {
        qWarning() << "Connection check failed for " << devicePath << ": " << reply.error().message();
        return false;
    }

    return reply.value().toBool();
}

bool DeviceDiscoveryLinux::startNotify(const QString &characteristicPath)
{
    QDBusInterface charInterface(
        "org.bluez",
        characteristicPath,
        "org.bluez.GattCharacteristic1",
        *m_connection
    );

    QDBusReply<void> reply = charInterface.call("StartNotify");

    if (!reply.isValid()) {
        qWarning() << "StartNotify failed: " << reply.error().message();
        return false;
    }

    return true;
}

bool DeviceDiscoveryLinux::stopNotify(const QString &characteristicPath)
{
    QDBusInterface charInterface(
        "org.bluez",
        characteristicPath,
        "org.bluez.GattCharacteristic1",
        *m_connection
    );

    QDBusReply<void> reply = charInterface.call("StopNotify");

    if (!reply.isValid()) {
        qWarning() << "StopNotify failed: " << reply.error().message();
        return false;
    }

    return true;
}

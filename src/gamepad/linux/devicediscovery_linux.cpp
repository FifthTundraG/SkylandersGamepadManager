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

#include "devicediscovery_linux.h"
#include "../gamepad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>
#include <QDebug>
#include <gamepad.h>

using ManagedObjects =
    QMap<QDBusObjectPath,
        QMap<QString,
            QMap<QString, QVariant>>>;

DeviceDiscoveryLinux::DeviceDiscoveryLinux(QObject *parent)
    : DeviceDiscovery(parent), m_connection(QDBusConnection::systemBus())
{
    qDBusRegisterMetaType<ManagedObjects>();

    if (!m_connection.isConnected()) {
        qCritical() << "Failed to connect to system D-Bus:" << m_connection.lastError().message();
        return;
    }
}

QStringList DeviceDiscoveryLinux::findGamepads()
{
    if (!m_connection.isConnected()) {
        qCritical() << "DeviceDiscovery: Cannot search for gamepad devices when D-Bus connection is not established.";
        return QStringList();
    }

    QStringList results;

    QDBusInterface objectManager(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        m_connection
    );

    QDBusReply<ManagedObjects> reply = objectManager.call("GetManagedObjects");

    if (!reply.isValid()) {
        qCritical() << "DeviceDiscovery: Failed to get BlueZ managed objects: " << reply.error().message();
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
            qInfo() << "DeviceDiscovery: Found connected gamepad:" << objectPath;
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
        m_connection
    );

    return device.property("Connected").toBool();

}

bool DeviceDiscoveryLinux::startNotify(const QString &characteristicPath)
{
    QDBusInterface charInterface(
        "org.bluez",
        characteristicPath,
        "org.bluez.GattCharacteristic1",
        m_connection
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
        m_connection
    );

    QDBusReply<void> reply = charInterface.call("StopNotify");

    if (!reply.isValid()) {
        qWarning() << "StopNotify failed: " << reply.error().message();
        return false;
    }

    return true;
}

bool DeviceDiscoveryLinux::enablePassiveScanning() // todo: make this setPassiveScanning and allow unsubscribing from these D-Bus signals
{
    bool autoConnect = m_connection.connect(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        "InterfacesAdded",
        this,
        SLOT(onInterfacesAdded(QDBusObjectPath,QVariantMap))
    );
    if (!autoConnect) {
        qWarning() << "DeviceDiscovery: Passive scanning failed: Could not subscribe to InterfacesAdded signal";
        return false;
    }

    bool removedConnect = m_connection.connect(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        "InterfacesRemoved",
        this,
        SLOT(onInterfacesRemoved(QDBusObjectPath,QStringList))
    );
    if (!removedConnect) {
        qWarning() << "DeviceDiscovery: Passive scanning failed: Could not subscribe to InterfacesRemoved signal";
        return false;
    }

    return true;
}

void DeviceDiscoveryLinux::onInterfacesAdded(const QDBusObjectPath &path, const QVariantMap &interfaces)
{
    if (!interfaces.contains("org.bluez.Device1"))
        return;

    qInfo() << "Bluetooth device appeared:" << path.path();

    qInfo() << interfaces;

    // todo: implement
}

void DeviceDiscoveryLinux::onInterfacesRemoved(const QDBusObjectPath &path, const QStringList &interfaces)
{
    if (!interfaces.contains("org.bluez.Device1"))
        return;

    qInfo() << "Bluetooth device disappeared:" << path.path();

    // todo: implement
}

// MARK: DeviceDiscoveryFactory
DeviceDiscovery* DeviceDiscoveryFactory::create(QObject *parent)
{
    return new DeviceDiscoveryLinux(parent);
}
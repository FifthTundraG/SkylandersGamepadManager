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

#include "DeviceDiscoveryLinux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>
#include <QDebug>
#include "../Gamepad.h"

using ManagedObjects =
    QMap<QDBusObjectPath,
        QMap<QString,
            QMap<QString, QVariant>>>;

DeviceDiscoveryLinux::DeviceDiscoveryLinux(QObject *parent)
    : DeviceDiscovery(parent), m_connection(QDBusConnection::systemBus())
{
    qDBusRegisterMetaType<ManagedObjects>();

    if (!m_connection.isConnected()) {
        qCritical() << "DeviceDiscovery: Failed to connect to system D-Bus:" << m_connection.lastError().message();
        return;
    }
}

/**
 * Checks if a given device's properties qualifies it as a Skylander Gamepad
 *
 * @param deviceProps org.bluez.Device1 properties
 */
static bool isSkylandersGamepad(const QVariantMap deviceProps)
{
    // todo: we should probably instead check by using some sort of vendor ID
    if (deviceProps.value("Name").toString() == DEVICE_NAME) {
        return true;
    }

    return false;
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

bool DeviceDiscoveryLinux::enablePassiveScanning() // todo: make this setPassiveScanning and allow unsubscribing from these D-Bus signals
{
    bool autoConnect = m_connection.connect(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        "InterfacesAdded",
        this,
        SLOT(onInterfacesAdded(QDBusObjectPath, QMap<QString, QVariantMap>))
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
        SLOT(onInterfacesRemoved(QDBusObjectPath, QMap<QString, QVariantMap>))
    );
    if (!removedConnect) {
        qWarning() << "DeviceDiscovery: Passive scanning failed: Could not subscribe to InterfacesRemoved signal";
        return false;
    }

    return true;
}

void DeviceDiscoveryLinux::onInterfacesAdded(const QDBusObjectPath &path, const QMap<QString, QVariantMap> &interfaces)
{
    if (!interfaces.contains("org.bluez.Device1"))
        return;

    if (isSkylandersGamepad(interfaces["org.bluez.Device1"])) {
        qInfo() << "DeviceDiscovery: Found new unconnected gamepad:" << path.path();
        m_knownDevices.append(path.path());

        // subscribe to properties changed for the unconnected gamepad to check if it gets connected, and if it does then create a new Gamepad
        bool propertiesChangedConnect = m_connection.connect(
            "org.bluez",
            path.path(),
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            this,
            SLOT(onPropertiesChangedOnKnownDevice(QString, QVariantMap, QStringList))
        );
        if (!propertiesChangedConnect) {
            qWarning() << "DeviceDiscovery: Could not subscribe to PropertiesChanged signal for" << path.path();
        }
    }
}

void DeviceDiscoveryLinux::onInterfacesRemoved(const QDBusObjectPath &path, const  QMap<QString, QVariantMap> &interfaces)
{
    if (!interfaces.contains("org.bluez.Device1"))
        return;

    if (m_knownDevices.contains(path.path())) {
        qInfo() << "DeviceDiscovery: Removing known device at" << path.path();

        m_knownDevices.removeAll(path.path()); // removeAll even though there is almost certainly only one, just in case

        // unsubscribe from PropertiesChanged on this device (todo: do we need to do this?)
        bool propertiesChangedDisconnect = m_connection.disconnect(
            "org.bluez",
            path.path(),
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            this,
            SLOT(onPropertiesChangedOnKnownDevice(QDBusObjectPath,QStringList))
        );
        if (!propertiesChangedDisconnect) {
            qWarning() << "DeviceDiscovery: Could not unsubscribe from PropertiesChanged signal for" << path.path();
        }

        //* PropertiesChanged will handle removing the gamepad via deviceDisconnected signal
    }
}

void DeviceDiscoveryLinux::onPropertiesChangedOnKnownDevice(const QString &interface, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);

    if (interface != "org.bluez.Device1") {
        return;
    }

    auto devicePath = message().path();

    if (changedProperties.contains("ServicesResolved")) { //* we need services to resolve for the device to connect, for services to resolve the device must be connected so this is essentially a connected check
        bool servicesResolved = changedProperties.value("ServicesResolved").toBool();

        if (servicesResolved) {
            emit deviceConnected(devicePath);
        }
    }

    if (changedProperties.contains("Connected")) {
        bool connected = changedProperties.value("Connected").toBool();

        if (!connected) {
            emit deviceDisconnected(devicePath);
        }
    }
}

// MARK: DeviceDiscoveryFactory
DeviceDiscovery* DeviceDiscoveryFactory::create(QObject *parent)
{
    return new DeviceDiscoveryLinux(parent);
}
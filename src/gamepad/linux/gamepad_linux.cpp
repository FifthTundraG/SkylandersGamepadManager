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

#include "gamepad_linux.h"
#include <QObject>
#include <linux/input-event-codes.h>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>

GamepadLinux::GamepadLinux(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent)
    : Gamepad(devicePath, std::move(device), parent)
{
    // Find characteristic
    m_characteristicPath = findCharacteristicPath(CHARACTERISTIC_UUID);

    // send StartNotify call to BlueZ so it starts sending data to us
    QDBusInterface iface(
        "org.bluez",
        m_characteristicPath,
        "org.bluez.GattCharacteristic1",
        QDBusConnection::systemBus()
    );

    QDBusReply<void> reply = iface.call("StartNotify");

    if (!reply.isValid()) {
        qWarning() << "StartNotify failed:" << reply.error().name() << reply.error().message();
    }

    // connect to characteristic changes so we can send data to virtual gamepad
    bool success = QDBusConnection::systemBus().connect(
        "org.bluez",
        m_characteristicPath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        this,
        SLOT(onCharacteristicPropertiesChanged(QString,QVariantMap,QStringList,QDBusMessage))
    );

    if (!success) {
        qWarning() << "Failed to connect characteristic PropertiesChanged signal";
    }
}

GamepadLinux::~GamepadLinux()
{
    QDBusConnection::systemBus().disconnect(
        "org.bluez",
        m_characteristicPath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        this,
        SLOT(onCharacteristicPropertiesChanged(QString,QVariantMap,QStringList,QDBusMessage))
    );
}

void GamepadLinux::onCharacteristicPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties, const QDBusMessage &message)
{
    if (interfaceName != "org.bluez.GattCharacteristic1") {
        return;
    }

    const QString objectPath = message.path();

    if (m_characteristicPath != objectPath) {
        return;
    }

    auto it = changedProperties.find("Value");
    if (it == changedProperties.end()) {
        return;
    }

    this->processData(it.value().toByteArray());
}

using ManagedObjects =
    QMap<QDBusObjectPath,
        QMap<QString,
            QMap<QString, QVariant>>>;

QString GamepadLinux::findCharacteristicPath(const QString &uuid)
{
    qInfo() << "Searching for characteristic" << uuid << "on device" << this->m_devicePath;

    // we must register managed objects return value before any call
    qDBusRegisterMetaType<ManagedObjects>();

    QDBusInterface iface(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        QDBusConnection::systemBus()
    );

    QDBusReply<ManagedObjects> reply = iface.call("GetManagedObjects");

    if (!reply.isValid()) {
        qWarning() << "Failed to get managed objects:" << reply.error().message();
        return QString();
    }

    const auto objects = reply.value();

    int characteristicsFound = 0;

    for (auto it = objects.begin(); it != objects.end(); ++it) {
        const QString objectPath = it.key().path();

        if (!objectPath.startsWith(this->m_devicePath)) {
            continue;
        }

        QMap interfaces = it.value();

        auto charIt = interfaces.find("org.bluez.GattCharacteristic1");
        if (charIt == interfaces.end()) {
            continue;
        }

        characteristicsFound++;

        QVariantMap properties = charIt.value();

        QString charUuid = properties.value("UUID").toString();

        if (!charUuid.isEmpty() && QString::compare(charUuid, uuid, Qt::CaseInsensitive) == 0) {
            qInfo() << "Found characteristic" << uuid << "at" << objectPath << "for" << this->m_devicePath;
            return objectPath;
        }
    }

    qWarning() << "Characteristic not found! Found" << characteristicsFound << "total characteristics.";

    return QString();
}

Gamepad* GamepadFactory::create(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent)
{
    return new GamepadLinux(devicePath, std::move(device), parent);
}
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

#include "gamepad.h"
#include <QObject>
// TODO: below include needs a platform-specific abstraction
#include <linux/input-event-codes.h>

#ifdef __linux__
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>
#endif

Gamepad::Gamepad(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent)
    : QObject(parent),
      m_devicePath(devicePath),
      m_inputDevice(std::move(device))
{
#ifdef __linux__
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
#endif

    qInfo() << "Skylanders gamepad" << m_inputDevice->getDevicePath() << "ready!";
}

Gamepad::~Gamepad()
{
    resetState();
}

void Gamepad::resetState() // todo: is this necessary?
{
    m_prevButtons = 0;
    m_prevTriggerL = 0;
    m_prevTriggerR = 0;
    m_prevShoulders = 0;
}

void Gamepad::processData(const QByteArray &data)
{
    if (!m_inputDevice) { // todo: add if data size is less than expected then return
        return;
    }

    const quint8 *value = reinterpret_cast<const quint8 *>(data.constData());

    const quint8 buttons = value[8];
    const quint8 shouldersAndPause = value[9];
    const quint8 triggerL = value[10];
    const quint8 triggerR = value[11];

    // sticks
    const quint8 right_x = value[12];
    const quint8 right_y = value[13];
    const quint8 left_x = value[14];
    const quint8 left_y = value[15];

    if (buttons != m_prevButtons) {
        const auto emitButton = [&](quint8 mask, uint code) {
            bool pressed = buttons & mask;
            m_inputDevice->writeButtonEvent(code, pressed);
        };

        emitButton(BUTTON_A_MASK, BTN_A);
        emitButton(BUTTON_B_MASK, BTN_B);
        emitButton(BUTTON_X_MASK, BTN_X);
        emitButton(BUTTON_Y_MASK, BTN_Y);
        emitButton(DPAD_UP_MASK, BTN_DPAD_UP);
        emitButton(DPAD_DOWN_MASK, BTN_DPAD_DOWN);
        emitButton(DPAD_LEFT_MASK, BTN_DPAD_LEFT);
        emitButton(DPAD_RIGHT_MASK, BTN_DPAD_RIGHT);

        m_prevButtons = buttons;
    }

    if (shouldersAndPause != m_prevShoulders) {
        const auto emitButton = [&](quint8 mask, uint code) {
            bool pressed = buttons & mask;
            m_inputDevice->writeButtonEvent(code, pressed);
        };

        emitButton(PAUSE_MASK, BTN_START);
        emitButton(SHOULDER_LEFT_MASK, BTN_TL);
        emitButton(SHOULDER_RIGHT_MASK, BTN_TR);
    }

    if (triggerL != m_prevTriggerL) {
        m_inputDevice->writeButtonEvent(BTN_TL2, (triggerL == TRIGGER_DOWN) ? 1 : 0);
    }
    if (triggerR != m_prevTriggerR) {
        m_inputDevice->writeButtonEvent(BTN_TR2, (triggerR == TRIGGER_DOWN) ? 1 : 0);
    }

    m_inputDevice->writeAxisEvent(ABS_X, left_x);
    m_inputDevice->writeAxisEvent(ABS_Y, -left_y);
    m_inputDevice->writeAxisEvent(ABS_RX, right_x);
    m_inputDevice->writeAxisEvent(ABS_RY, -right_y);

    m_inputDevice->sync();

    m_prevButtons = buttons;
    m_prevShoulders = shouldersAndPause;
    m_prevTriggerL = triggerL;
    m_prevTriggerR = triggerR;
}

#ifdef __linux__
using ManagedObjects =
    QMap<QDBusObjectPath,
        QMap<QString,
            QMap<QString, QVariant>>>;

QString Gamepad::findCharacteristicPath(const QString &uuid)
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
#endif
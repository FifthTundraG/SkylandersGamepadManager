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

#include "GamepadManager.h"

#include <QDebug>
#include <QPointer>

#include "Gamepad.h"
#include "DeviceDiscovery.h"

GamepadManager::GamepadManager(QObject *parent)
    : QObject(parent)
{
    m_discovery = DeviceDiscoveryFactory::create(this);

    // Wire discovery signals → manager slots
    connect(m_discovery, &DeviceDiscovery::deviceConnected,
            this, &GamepadManager::onDeviceConnected);

    connect(m_discovery, &DeviceDiscovery::deviceDisconnected,
            this, &GamepadManager::onDeviceDisconnected);
}

GamepadManager::~GamepadManager()
{
    qDeleteAll(m_gamepads);
    m_gamepads.clear();
}

void GamepadManager::initialize()
{
    const QStringList devices = m_discovery->findGamepads();

    if (devices.isEmpty()) {
        qInfo() << "No gamepad devices found at startup.";
        return;
    }

    for (const QString &devicePath : devices) {
        qInfo() << "Device already connected at startup:" << devicePath;
        addGamepad(devicePath);
    }
}

bool GamepadManager::enablePassiveScanning()
{
    if (!m_discovery) {
        qWarning() << "GamepadManager: Cannot enable passive scanning if DeviceDiscovery is null";
    }

    return m_discovery->enablePassiveScanning();
}

void GamepadManager::onDeviceConnected(const QString &devicePath)
{
    addGamepad(devicePath);
    qInfo() << "GamepadManager: Device connected:" << devicePath;
}

void GamepadManager::onDeviceDisconnected(const QString &devicePath)
{
    removeGamepad(devicePath);
    qInfo() << "GamepadManager: Device disconnected:" << devicePath;
}

void GamepadManager::addGamepad(const QString &devicePath)
{
    for (const auto &gamepad : m_gamepads) {
        if (gamepad && gamepad->m_devicePath == devicePath) {
            return;
        }
    }

    QPointer<Gamepad> gamepad = GamepadFactory::create(devicePath, this);

    m_gamepads.append(gamepad);

    emit gamepadConnected(m_gamepads.size() - 1, gamepad); // m_gamepads.size() - 1 works because we literally just added a gamepad to the list
}

void GamepadManager::removeGamepad(const QString &devicePath)
{
    QMutableListIterator<QPointer<Gamepad>> i(m_gamepads);
    int index = 0;
    while (i.hasNext()) {
        QPointer<Gamepad> gamepad = i.next();

        if (gamepad->m_devicePath == devicePath) {
            emit gamepadDisconnected(index, gamepad); // emit first in case listeners still need gamepad info

            i.remove();

            gamepad->deleteLater(); // removing it from the QList does not destroy it

            return; // exit early since we found what we need to remove
        }

        index++;
    }
}
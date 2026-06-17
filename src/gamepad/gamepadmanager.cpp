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

#include "gamepadmanager.h"

#include <QDebug>
#include <QPointer>

#include "gamepad.h"
#include "devicediscovery.h"

GamepadManager::GamepadManager(DeviceDiscovery *discovery, QObject *parent)
    : QObject(parent),
      m_discovery(discovery)
{
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

void GamepadManager::onDeviceConnected(const QString &devicePath)
{
    qInfo() << "Device connected:" << devicePath;
    addGamepad(devicePath);
}

void GamepadManager::onDeviceDisconnected(const QString &devicePath)
{
    qInfo() << "Device disconnected:" << devicePath;
    removeGamepad(devicePath);
}

void GamepadManager::addGamepad(const QString &devicePath)
{
    if (m_gamepads.contains(devicePath)) {
        return;
    }

    auto device = getVirtualInputFactory()->createDevice(DEVICE_NAME);

    QPointer<Gamepad> gamepad = new Gamepad(std::move(device), this);

    m_gamepads.insert(devicePath, gamepad);
}

void GamepadManager::removeGamepad(const QString &devicePath)
{
    m_gamepads.remove(devicePath);
}
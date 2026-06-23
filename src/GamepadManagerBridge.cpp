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

#include "GamepadManagerBridge.h"

#include "GamepadManager.h"
#include "Gamepad.h"

GamepadManagerBridge::GamepadManagerBridge(GamepadManager *manager, QObject *parent)
    : QObject(parent),
      m_manager(manager)
{
    connect(m_manager,
            &GamepadManager::gamepadConnected,
            this,
            &GamepadManagerBridge::onGamepadConnected);

    connect(m_manager,
            &GamepadManager::gamepadDisconnected,
            this,
            &GamepadManagerBridge::onGamepadDisconnected);
}

Gamepad *GamepadManagerBridge::pad0() const
{
    return m_pad0.data();
}

Gamepad *GamepadManagerBridge::pad1() const
{
    return m_pad1.data();
}

Gamepad *GamepadManagerBridge::pad2() const
{
    return m_pad2.data();
}

Gamepad *GamepadManagerBridge::pad3() const
{
    return m_pad3.data();
}

void GamepadManagerBridge::onGamepadConnected(int index, QPointer<Gamepad> gamepad)
{
    switch (index) {
    case 0:
        m_pad0 = gamepad;
        emit pad0Changed();
        break;
        
    case 1:
        m_pad1 = gamepad;
        emit pad1Changed();
        break;

    case 2:
        m_pad2 = gamepad;
        emit pad2Changed();
        break;

    case 3:
        m_pad3 = gamepad;
        emit pad3Changed();
        break;

    default:
        break;
    }
}

void GamepadManagerBridge::onGamepadDisconnected(int index, QPointer<Gamepad> gamepad)
{
    Q_UNUSED(gamepad);

    switch (index) {
    case 0:
        m_pad0.clear();
        emit pad0Changed();
        break;

    case 1:
        m_pad1.clear();
        emit pad1Changed();
        break;

    case 2:
        m_pad2.clear();
        emit pad2Changed();
        break;

    case 3:
        m_pad3.clear();
        emit pad3Changed();
        break;

    default:
        break;
    }
}
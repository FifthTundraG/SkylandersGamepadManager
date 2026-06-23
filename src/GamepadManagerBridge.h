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

#include <QObject>
#include <QPointer>

#include "gamepad/GamepadManager.h"
#include "gamepad/Gamepad.h"

class GamepadManagerBridge : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Gamepad* pad0 READ pad0 NOTIFY pad0Changed)
    Q_PROPERTY(Gamepad* pad1 READ pad1 NOTIFY pad1Changed)
    Q_PROPERTY(Gamepad* pad2 READ pad2 NOTIFY pad2Changed)
    Q_PROPERTY(Gamepad* pad3 READ pad3 NOTIFY pad3Changed)

public:
    explicit GamepadManagerBridge(GamepadManager *manager,
                                  QObject *parent = nullptr);

    Gamepad *pad0() const;
    Gamepad *pad1() const;
    Gamepad *pad2() const;
    Gamepad *pad3() const;

signals:
    void pad0Changed();
    void pad1Changed();
    void pad2Changed();
    void pad3Changed();

private slots:
    void onGamepadConnected(int index, QPointer<Gamepad> gamepad);
    void onGamepadDisconnected(int index, QPointer<Gamepad> gamepad);

private:
    GamepadManager *m_manager;

    QPointer<Gamepad> m_pad0;
    QPointer<Gamepad> m_pad1;
    QPointer<Gamepad> m_pad2;
    QPointer<Gamepad> m_pad3;
};
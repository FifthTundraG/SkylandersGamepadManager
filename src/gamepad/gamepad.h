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

#pragma once

#include <QtGlobal>
#include <QObject>
#include <QString>
#include "virtualinput.h"

#define CHARACTERISTIC_UUID "533e1541-3abe-f33f-cd00-594e8b0a8ea3"
#define DEVICE_NAME "Skylanders GamePad"

#define DPAD_UP_MASK 0x01
#define DPAD_DOWN_MASK 0x02
#define DPAD_LEFT_MASK 0x04
#define DPAD_RIGHT_MASK 0x08
#define BUTTON_A_MASK 0x10
#define BUTTON_B_MASK 0x20
#define BUTTON_X_MASK 0x40
#define BUTTON_Y_MASK 0x80

#define PAUSE_MASK 0x04
#define SHOULDER_LEFT_MASK 0x10
#define SHOULDER_RIGHT_MASK 0x20

#define TRIGGER_DOWN 0xFF

class Gamepad : public QObject
{
    Q_OBJECT

public: // todo: what to be public and what to be private?
    explicit Gamepad(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent = nullptr);
    ~Gamepad() override;

    /**
     * The OS-specific device path where the Bluetooth device is found
     *
     * Linux example: /org/bluez/hci0/dev_D2_C6_F7_00_76_A8
     */
    const QString m_devicePath;

    void processData(const QByteArray &data);

signals:
    void disconnected();

private:
    void resetState();

private:
    std::unique_ptr<VirtualInputDevice> m_inputDevice;

    // input state cache
    quint16 m_prevButtons = 0;
    qint16 m_prevTriggerL = 0;
    qint16 m_prevTriggerR = 0;
    qint16 m_prevShoulders = 0;
};

class GamepadFactory {
public:
    static Gamepad* create(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent = nullptr);
};
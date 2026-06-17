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
#include <QString>
#include "platform/virtualinput.h"

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

class Gamepad
{
public: // todo: what to be public and what to be private?
    VirtualInputDevice *input_device;
    quint32 characteristic_properties_changed_id; // todo: this is D-Bus only right? where do we put it
    quint32 disconnected_id; // todo: ^
    quint16 prev_buttons;
    qint16 prev_trigger_l, prev_trigger_r;
    qint16 prev_shoulders;

    virtual void process_data(QByteArray data);
};
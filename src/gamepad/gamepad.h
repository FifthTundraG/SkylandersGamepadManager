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
#include "platform/virtual-input.h"

#define CHARACTERISTIC_UUID "533e1541-3abe-f33f-cd00-594e8b0a8ea3"

typedef struct {
    char *device_path;
    quint32 characteristic_properties_changed_id;
    quint32 disconnected_id;
    VirtualInputDevice *input_device;
    quint16 prev_buttons;
    qint16 prev_trigger_l, prev_trigger_r;
    qint16 prev_shoulders;
} Gamepad;
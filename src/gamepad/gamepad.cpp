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
// TODO: below include needs a platform-specific abstraction
#include <linux/input-event-codes.h>

void Gamepad::process_data(QByteArray data)
{
    if (!this->input_device) {
        return;
    }

    auto gamepadInputDevice = this->input_device;

    uint16_t buttons = data[8];
    int16_t shoulders_and_pause = data[9];
    int16_t trigger_l = data[10];
    int16_t trigger_r = data[11];
    int8_t right_x = (int8_t)data[12];
    int8_t right_y = (int8_t)data[13];
    int8_t left_x = (int8_t)data[14];
    int8_t left_y = (int8_t)data[15];

    uint16_t changed = buttons ^ this->prev_buttons;

    if (changed & BUTTON_A_MASK) gamepadInputDevice->writeButtonEvent(BTN_A, (buttons & BUTTON_A_MASK) ? 1 : 0);
    if (changed & BUTTON_B_MASK) gamepadInputDevice->writeButtonEvent(BTN_B, (buttons & BUTTON_B_MASK) ? 1 : 0);
    if (changed & BUTTON_X_MASK) gamepadInputDevice->writeButtonEvent(BTN_X, (buttons & BUTTON_X_MASK) ? 1 : 0);
    if (changed & BUTTON_Y_MASK) gamepadInputDevice->writeButtonEvent(BTN_Y, (buttons & BUTTON_Y_MASK) ? 1 : 0);
    if (changed & DPAD_UP_MASK) gamepadInputDevice->writeButtonEvent(BTN_DPAD_UP, (buttons & DPAD_UP_MASK) ? 1 : 0);
    if (changed & DPAD_DOWN_MASK) gamepadInputDevice->writeButtonEvent(BTN_DPAD_DOWN, (buttons & DPAD_DOWN_MASK) ? 1 : 0);
    if (changed & DPAD_LEFT_MASK) gamepadInputDevice->writeButtonEvent(BTN_DPAD_LEFT, (buttons & DPAD_LEFT_MASK) ? 1 : 0);
    if (changed & DPAD_RIGHT_MASK) gamepadInputDevice->writeButtonEvent(BTN_DPAD_RIGHT, (buttons & DPAD_RIGHT_MASK) ? 1 : 0);

    int16_t shoulders_changed = shoulders_and_pause ^ this->prev_shoulders;
    if (shoulders_changed & PAUSE_MASK) gamepadInputDevice->writeButtonEvent(BTN_START, (shoulders_and_pause & PAUSE_MASK) ? 1 : 0);
    if (shoulders_changed & SHOULDER_LEFT_MASK) gamepadInputDevice->writeButtonEvent(BTN_TL, (shoulders_and_pause & SHOULDER_LEFT_MASK) ? 1 : 0);
    if (shoulders_changed & SHOULDER_RIGHT_MASK) gamepadInputDevice->writeButtonEvent(BTN_TR, (shoulders_and_pause & SHOULDER_RIGHT_MASK) ? 1 : 0);

    if (trigger_l != this->prev_trigger_l) {
        gamepadInputDevice->writeButtonEvent(BTN_TL2, (trigger_l == TRIGGER_DOWN) ? 1 : 0);
    }
    if (trigger_r != this->prev_trigger_r) {
        gamepadInputDevice->writeButtonEvent(BTN_TR2, (trigger_r == TRIGGER_DOWN) ? 1 : 0);
    }
    
    gamepadInputDevice->writeAxisEvent(ABS_X, left_x);
    gamepadInputDevice->writeAxisEvent(ABS_Y, -left_y);
    gamepadInputDevice->writeAxisEvent(ABS_RX, right_x);
    gamepadInputDevice->writeAxisEvent(ABS_RY, -right_y);

    gamepadInputDevice->sync();

    this->prev_buttons = buttons;
    this->prev_shoulders = shoulders_and_pause;
    this->prev_trigger_l = trigger_l;
    this->prev_trigger_r = trigger_r;
}
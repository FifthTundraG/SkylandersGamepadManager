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

Gamepad::Gamepad(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent)
    : QObject(parent),
      m_devicePath(devicePath),
      m_inputDevice(std::move(device))
{
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
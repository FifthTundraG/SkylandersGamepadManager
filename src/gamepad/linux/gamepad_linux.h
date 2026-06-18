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

#include <QtGlobal>
#include <QObject>
#include <QString>
#include "virtualinput.h"
#include "../gamepad.h"

class GamepadLinux : public Gamepad
{
    Q_OBJECT

public: // todo: what to be public and what to be private?
    explicit GamepadLinux(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent = nullptr);
    ~GamepadLinux() = default;

    /**
     * The OS-specific device path where the Bluetooth device is found
     *
     * Linux example: /org/bluez/hci0/dev_D2_C6_F7_00_76_A8
     */
    const QString m_devicePath;
#ifdef __linux
    /** example format: /org/bluez/hci0/dev_D2_C6_F7_00_76_A8/service000c/char000d */
    QString getCharacteristicPath() const;
#endif

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

#ifdef __linux__
    /**
     * @param uuid The characteristic UUID to search for
     */
    QString findCharacteristicPath(const QString &uuid);
    /** Cached characteristic path to avoid repeated calls to findCharactertisticPath */
    QString m_characteristicPath = nullptr;
#endif
};
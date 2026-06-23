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
#include <QString>
#include <QList>

#include "DeviceDiscovery.h"
#include "Gamepad.h"

class GamepadManager : public QObject
{
    Q_OBJECT

public:
    explicit GamepadManager(QObject *parent = nullptr);
    ~GamepadManager() override;

    /**
     * Checks if devices are already connected before application startup, and if so then connects them
     */
    void initialize();

    bool enablePassiveScanning();

signals:
    void gamepadConnected(const int index, const QPointer<Gamepad> gamepad);
    void gamepadDisconnected(const int index, const QPointer<Gamepad> gamepad);

private slots:
    void onDeviceConnected(const QString &devicePath);
    void onDeviceDisconnected(const QString &devicePath);

private:
    void addGamepad(const QString &devicePath);
    void removeGamepad(const QString &devicePath);

private:
    DeviceDiscovery *m_discovery = nullptr;
    QList<QPointer<Gamepad>> m_gamepads;
};
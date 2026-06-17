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
#include <QHash>

#include "platform/devicediscovery.h"
#include "gamepad.h"

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

private slots:
    void onDeviceConnected(const QString &devicePath);
    void onDeviceDisconnected(const QString &devicePath);

private:
    void addGamepad(const QString &devicePath);
    void removeGamepad(const QString &devicePath);

private:
    DeviceDiscovery *m_discovery = nullptr;
    QHash<QString, QPointer<Gamepad>> m_gamepads;
};
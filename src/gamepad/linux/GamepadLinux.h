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
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include "../virtualinput.h"
#include "../Gamepad.h"

class GamepadLinux : public Gamepad
{
    Q_OBJECT

public: // todo: what to be public and what to be private?
    explicit GamepadLinux(const QString devicePath, std::unique_ptr<VirtualInputDevice> device, QObject *parent = nullptr);
    ~GamepadLinux() override;

    /** example format: /org/bluez/hci0/dev_D2_C6_F7_00_76_A8/service000c/char000d */
    QString getCharacteristicPath() const;

    QString getMacAddress() const;

private slots:
    void onCharacteristicPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties, const QDBusMessage &message);


private:
    QDBusConnection m_connection;

    /**
     * @param uuid The characteristic UUID to search for
     */
    QString findCharacteristicPath(const QString &uuid);
    /** Cached characteristic path to avoid repeated calls to findCharactertisticPath */
    QString m_characteristicPath = nullptr;
};
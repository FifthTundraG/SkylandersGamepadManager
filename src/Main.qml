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

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 300
    height: 325
    title: "Skylanders Gamepad Manager"

    Column {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 8
        }

        spacing: 2

        Label {
            text: "Connected Gamepads"
            // bottomPadding: 2
        }
        Frame {
            width: parent.width

            GridLayout {
                width: parent.width
                columns: 4

                ControllerBox {
                    color: gamepads.pad0 ? gamepads.pad0.color : "gray"
                }
                ControllerBox {
                    color: gamepads.pad1 ? gamepads.pad1.color : "gray"
                }
                ControllerBox {
                    color: gamepads.pad2 ? gamepads.pad2.color : "gray"
                }
                ControllerBox {
                    color: gamepads.pad3 ? gamepads.pad3.color : "gray"
                }
            }
        }
    }

    Item { // absorbs all extra vertical space, keeping the label and frame pinned to the top
        Layout.fillHeight: true
    }
}
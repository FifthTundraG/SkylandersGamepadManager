import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 300
    height: 325
    title: "Skylanders Gamepad Manager"

    GridLayout {
        columns: 4
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 4

        ControllerBox {
            color: "red"
        }
        ControllerBox {
            color: "green"
        }
        ControllerBox {
            color: "blue"
        }
        ControllerBox {
            color: "purple"
        }
    }
}
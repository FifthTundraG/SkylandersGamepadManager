import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    property string color: "red"

    Layout.fillWidth: true
    Layout.alignment: Qt.AlignHCenter

    Rectangle {
        width: 64
        height: 64
        Layout.alignment: Qt.AlignHCenter

        color: root.color
    }

    Button {
        text: "Configure"
        Layout.alignment: Qt.AlignHCenter

        onClicked: {
            console.log("clicked button below controller box with color: "+root.color);
        }
    }
}
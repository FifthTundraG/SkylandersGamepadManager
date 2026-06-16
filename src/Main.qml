import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Skylanders Gamepad Manager"

    Button {
        anchors.centerIn: parent
        text: "test button"

        onClicked: {
            console.log("button clicked");
        }
    }
}
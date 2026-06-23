qt_policy(SET QTP0001 NEW)

# Main application target
qt_add_executable(SkylandersGamepadManager
    main.cpp
    GamepadManagerBridge.h
    GamepadManagerBridge.cpp
)

qt_add_qml_module(SkylandersGamepadManager
    URI SkylandersGamepadManagerQML
    VERSION 1.0
    QML_FILES
        Main.qml
        ControllerBox.qml
)

add_subdirectory(gamepad)
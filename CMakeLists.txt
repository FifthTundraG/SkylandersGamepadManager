cmake_minimum_required(VERSION 3.13)

project(SkylandersGamepadManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (LINUX)
    find_package(PkgConfig)
    pkg_check_modules(LIBEVDEV REQUIRED libevdev)
    pkg_check_modules(LIBUDEV REQUIRED libudev)
endif()

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(SkylandersGamepadManager
    src/main.cpp
)

qt_add_qml_module(SkylandersGamepadManager
    URI SkylandersGamepadManagerQML # note: the URI needs to be different from the project name because during build QML makes a directory in `build/` with the QML URI, so linking fails since a directory wit the project name already exists
    VERSION 1.0
    QML_FILES
        src/Main.qml
)

target_link_libraries(SkylandersGamepadManager PRIVATE Qt6::Quick)
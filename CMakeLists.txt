cmake_minimum_required(VERSION 3.13)

project(SkylandersGamepadManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (LINUX)
    find_package(PkgConfig)
    pkg_check_modules(LIBEVDEV REQUIRED libevdev)
    pkg_check_modules(LIBUDEV REQUIRED libudev)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(SkylandersGamepadManager)

add_subdirectory(src)

target_link_libraries(SkylandersGamepadManager PRIVATE Qt6::Widgets)
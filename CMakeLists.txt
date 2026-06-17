cmake_minimum_required(VERSION 3.13)

project(SkylandersGamepadManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# MARK: Dependencies
find_package(Qt6 REQUIRED COMPONENTS Quick)

if (UNIX AND NOT APPLE)
    find_package(PkgConfig)
    pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)

    find_package(Qt6 REQUIRED COMPONENTS Core DBus)
endif()

qt_standard_project_setup()

# MARK: Add Project Files
add_subdirectory(src)

# MARK: Linking
target_link_libraries(SkylandersGamepadManager PRIVATE Qt6::Quick)

if(UNIX AND NOT APPLE)
    target_link_libraries(SkylandersGamepadManager PRIVATE
        PkgConfig::LIBEVDEV
        Qt6DBus
    )
endif()
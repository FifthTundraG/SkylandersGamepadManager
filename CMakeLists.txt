cmake_minimum_required(VERSION 3.13)

project(SkylandersGamepadManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# MARK: Application Options
set(APPLICATION_BINARY_NAME "SkylandersGamepadManager" CACHE STRING "Name of the Manager binary")

set(APPLICATION_VERSION_MAJOR 0)
set(APPLICATION_VERSION_MINOR 1)
set(APPLICATION_VERSION_PATCH 0)

set(APPLICATION_VERSION_NAME "${Manager_VERSION_MAJOR}.${Manager_VERSION_MINOR}.${Manager_VERSION_PATCH}")

set(APPLICATION_BUILD_PLATFORM "unknown" CACHE STRING "A short string identifying the platform that this build was built for. Only used to display in the about dialog.")

add_subdirectory(program_info)

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
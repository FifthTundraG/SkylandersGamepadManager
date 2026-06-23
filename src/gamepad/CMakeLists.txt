add_library(skylanders_gamepad_lib INTERFACE)

target_include_directories(skylanders_gamepad_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
)

target_sources(skylanders_gamepad_lib INTERFACE
    # platform-dependent sources
    ${CMAKE_CURRENT_SOURCE_DIR}/virtualinput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DeviceDiscovery.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Gamepad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Gamepad.cpp

    # platform-agonistic sources
    ${CMAKE_CURRENT_SOURCE_DIR}/GamepadManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GamepadManager.cpp
)

# Include platform-specific implementations
if(UNIX AND NOT APPLE)
    add_subdirectory(linux)
endif()


# Link gamepad to main executable
target_link_libraries(SkylandersGamepadManager PRIVATE skylanders_gamepad_lib)

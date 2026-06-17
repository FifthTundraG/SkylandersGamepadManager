add_library(skylanders_gamepad_lib INTERFACE)

target_include_directories(skylanders_gamepad_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
)

target_sources(skylanders_gamepad_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/gamepad.h
)

# Include platform-specific implementations
add_subdirectory(platform)

# Link gamepad to main executable
target_link_libraries(SkylandersGamepadManager PRIVATE skylanders_gamepad_lib)

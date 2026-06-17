# Platform abstraction layer
target_include_directories(skylanders_gamepad_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(skylanders_gamepad_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/virtualinput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/devicediscovery.h
)

# Include platform-specific implementations
if(UNIX AND NOT APPLE)
    add_subdirectory(linux)
endif()

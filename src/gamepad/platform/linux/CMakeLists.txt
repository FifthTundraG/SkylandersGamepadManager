target_sources(skylanders_gamepad_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/virtualinput_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/virtualinput_linux.h
    ${CMAKE_CURRENT_SOURCE_DIR}/devicediscovery_linux.h
    ${CMAKE_CURRENT_SOURCE_DIR}/devicediscovery_linux.cpp
)

target_include_directories(skylanders_gamepad_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link dependencies for Linux implementation
target_link_libraries(skylanders_gamepad_lib INTERFACE
    PkgConfig::LIBEVDEV
    Qt6::Core
    Qt6DBus
)

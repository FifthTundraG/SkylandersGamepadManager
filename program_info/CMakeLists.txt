set(APPLICATION_COMMON_NAME "SkylandersGamepadManager")
set(APPLICATION_DISPLAY_NAME "SkylandersGamepadManager")
set(APPLICATION_ID "io.github.fifthtundrag.SkylandersGamepadManager")
set(APPLICATION_GIT "https://github.com/FifthTundraG/SkylandersGamepadManager")

set(APPLICATION_COMMON_NAME "${APPLICATION_COMMON_NAME}" PARENT_SCOPE)
set(APPLICATION_DISPLAY_NAME "${APPLICATION_DISPLAY_NAME}" PARENT_SCOPE)
set(APPLICATION_ID "${APPLICATION_ID}" PARENT_SCOPE)
set(APPLICATION_GIT "${APPLICATION_GIT}" PARENT_SCOPE)

set(APPLICATION_COPYRIGHT "© 2026 FifthTundraG" PARENT_SCOPE)
set(APPLICATION_SVGFILE "${APPLICATION_ID}.svg" PARENT_SCOPE)

set(APPLICATION_DESKTOP "program_info/${Manager_AppID}.desktop" PARENT_SCOPE)
set(APPLICATION_METAINFO "program_info/${Manager_AppID}.metainfo.xml" PARENT_SCOPE)
set(APPLICATION_AUTHORS "FifthTundraG")

configure_file(${APPLICATION_ID}.desktop.in ${APPLICATION_ID}.desktop)
configure_file(${APPLICATION_ID}.metainfo.xml.in ${APPLICATION_ID}.metainfo.xml)
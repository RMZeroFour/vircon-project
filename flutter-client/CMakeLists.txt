list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

find_package(Flutter REQUIRED)

set(_build_mode $<IF:$<CONFIG:Release,MinSizeRel>,release,debug>)
set(_build_command "$<TARGET_FILE:Flutter::Flutter>" build apk --${_build_mode})
set(_build_output "build/app/outputs/apk/${_build_mode}/app-${_build_mode}.apk")

add_custom_target(
    vircon-flutter-client ALL
    COMMAND ${_build_command}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building client app with Flutter"
    VERBATIM
)

install(
    FILES "${_build_output}"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/vircon
    RENAME vircon-flutter-client.apk
)
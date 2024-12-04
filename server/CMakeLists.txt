cmake_minimum_required(VERSION 3.22)
project(vircon VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(NotcursesCore REQUIRED)
find_package(libevdev REQUIRED)

include(CreateNotcursesCoreTarget)
CreateNotcursesCoreTarget()

configure_file(version.h.in version.h @ONLY)

add_executable(vircon-server)
target_compile_features(vircon-server PRIVATE cxx_std_20)
target_sources(vircon-server PRIVATE main.cpp gamepad.cpp serverstate.cpp guistate.cpp)
target_include_directories(vircon-server PRIVATE "${PROJECT_BINARY_DIR}")
target_link_libraries(vircon-server PRIVATE Notcurses::NotcursesCore libevdev::libevdev)

install(TARGETS vircon-server)
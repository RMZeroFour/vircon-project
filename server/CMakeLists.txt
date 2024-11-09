cmake_minimum_required(VERSION 3.22)
project(vircon VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(NotcursesCore REQUIRED)

include(CreateNotcursesCoreTarget)
CreateNotcursesCoreTarget()

add_executable(vircon-server)
target_sources(vircon-server PRIVATE main.cpp guistate.cpp)
target_link_libraries(vircon-server PRIVATE Notcurses::NotcursesCore)

install(TARGETS vircon-server)
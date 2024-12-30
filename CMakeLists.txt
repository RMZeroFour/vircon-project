cmake_minimum_required(VERSION 3.22)
project(vircon VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(server)
add_subdirectory(terminal-client)
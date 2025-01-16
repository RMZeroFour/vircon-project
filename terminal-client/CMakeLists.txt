find_package(NotcursesCore REQUIRED)

include(CreateNotcursesCoreTarget)
CreateNotcursesCoreTarget()

configure_file(version.h.in version.h @ONLY)

add_executable(vircon-terminal-client)
target_compile_features(vircon-terminal-client PRIVATE cxx_std_20)
target_sources(vircon-terminal-client PRIVATE guistate.cpp main.cpp textinput.cpp)
target_include_directories(vircon-terminal-client PRIVATE . "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(vircon-terminal-client PRIVATE Notcurses::NotcursesCore)

install(TARGETS vircon-terminal-client)
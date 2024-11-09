# Workaround until https://github.com/dankamongmen/notcurses/pull/2800 is merged.
function(CreateNotcursesCoreTarget)
    if(NOT TARGET Notcurses::NotcursesCore)
        add_library(Notcurses::NotcursesCore INTERFACE IMPORTED)
        target_include_directories(Notcurses::NotcursesCore INTERFACE ${Notcurses_INCLUDE_DIRS})
        target_link_directories(Notcurses::NotcursesCore INTERFACE ${Notcurses_LIBRARY_DIRS})
        target_link_libraries(Notcurses::NotcursesCore INTERFACE ${Notcurses_LIBRARIES})
    endif()
endfunction()
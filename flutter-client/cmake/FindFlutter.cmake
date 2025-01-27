find_program(Flutter_EXECUTABLE NAMES flutter)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Flutter REQUIRED_VARS Flutter_EXECUTABLE)

if (Flutter_FOUND)
    mark_as_advanced(Flutter_EXECUTABLE)
    if (NOT TARGET Flutter::Flutter)
        add_executable(Flutter::Flutter IMPORTED)
        set_property(TARGET Flutter::Flutter PROPERTY IMPORTED_LOCATION ${Flutter_EXECUTABLE})
    endif()
endif()
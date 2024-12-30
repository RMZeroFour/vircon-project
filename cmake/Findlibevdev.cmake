find_package(PkgConfig REQUIRED)
pkg_check_modules(libevdev REQUIRED IMPORTED_TARGET libevdev)
add_library(libevdev::libevdev ALIAS PkgConfig::libevdev)
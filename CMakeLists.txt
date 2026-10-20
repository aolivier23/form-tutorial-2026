cmake_minimum_required(VERSION 3.31)
project(form_tutorial VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

find_package(Boost REQUIRED json)
find_package(phlex REQUIRED)
find_package(ROOT REQUIRED RIO)

add_subdirectory(writing)
add_subdirectory(reading)

configure_file(setup.sh.in setup.sh @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.sh DESTINATION bin)

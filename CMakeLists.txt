cmake_minimum_required(VERSION 2.6)
set(YAMAIL_CMAKE_MODULES ${CMAKE_INSTALL_PREFIX}/share/cmake_helpers)
include(${YAMAIL_CMAKE_MODULES}/yamail.cmake)

yamail_project(yextractor)

add_subdirectory(include)
add_subdirectory(src)

yamail_configuration_message()

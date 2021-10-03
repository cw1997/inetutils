cmake_minimum_required(VERSION 3.17)
project(inetutils VERSION 1.0 LANGUAGES C HOMEPAGE_URL https://github.com/cw1997/inetutils)

set(CMAKE_C_STANDARD 11)

set(link_libraries PUBLIC ip utils resolve icmp_echo icmp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set .so path
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#set(CMAKE_INSTALL_RPATH $ORIGIN)
#set_target_properties(project1 PROPERTIES LINK_FLAGS "-Wl,-rpath,./")

add_subdirectory(src)

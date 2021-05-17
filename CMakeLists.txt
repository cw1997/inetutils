cmake_minimum_required(VERSION 3.13)
project(inetutils VERSION 1.0 LANGUAGES C HOMEPAGE_URL https://github.com/cw1997/inetutils)

set(CMAKE_C_STANDARD 11)

SET(output_path ../../bin)

SET(EXECUTABLE_OUTPUT_PATH ${output_path})
SET(LIBRARY_OUTPUT_PATH ${output_path})

# set .so path
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)
#set_target_properties(project1 PROPERTIES LINK_FLAGS "-Wl,-rpath,./")

add_subdirectory(src/common)
add_subdirectory(src/ping)
#add_subdirectory(src/traceroute)

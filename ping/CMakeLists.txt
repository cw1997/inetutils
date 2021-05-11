cmake_minimum_required(VERSION 3.13)
project(ping C)

set(CMAKE_C_STANDARD 11)

add_executable(ping main.c ping.c ping.h icmp.c icmp.h)

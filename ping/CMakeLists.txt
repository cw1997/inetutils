cmake_minimum_required(VERSION 3.13)
project(ping C)

set(CMAKE_C_STANDARD 11)

add_executable(ping main.c icmp.c icmp.h resolve.c resolve.h ping.c ping.h)

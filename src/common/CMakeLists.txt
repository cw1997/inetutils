add_library(ip SHARED ip.h ip.c)
#SET_TARGET_PROPERTIES(ip PROPERTIES VERSION 1.0 SOVERSION 1)

add_library(utils SHARED utils.h utils.c)
#SET_TARGET_PROPERTIES(utils PROPERTIES VERSION 1.0 SOVERSION 1)

add_library(resolve SHARED resolve.h resolve.c)
#SET_TARGET_PROPERTIES(resolve PROPERTIES VERSION 1.0 SOVERSION 1)

add_library(icmp_echo SHARED icmp_echo.h icmp_echo.c)
#SET_TARGET_PROPERTIES(icmp_echo PROPERTIES VERSION 1.0 SOVERSION 1)

add_library(icmp SHARED icmp.h icmp.c)
#SET_TARGET_PROPERTIES(icmp PROPERTIES VERSION 1.0 SOVERSION 1)

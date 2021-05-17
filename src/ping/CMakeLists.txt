add_executable(ping main.c)
target_link_libraries(ping PUBLIC icmp resolve icmp_echo ip utils)

//
// Created by cw1997 on 2021/5/10.
//

#include "ip.h"

char* ipv4_uint32_to_dot_split(uint32_t int_ip) {
    struct in_addr in_address;
    in_address.s_addr = int_ip;
    char* address = inet_ntoa(in_address);
    return address;
}


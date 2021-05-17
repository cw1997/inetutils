//
// Created by cw1997 on 2021/5/10.
//

#include "ip.h"

char* ipv4_uint32_to_dot_split(uint32_t ipv4_uint32) {
    struct in_addr in_address;
    in_address.s_addr = ipv4_uint32;
    char* address = inet_ntoa(in_address);
    return address;
}

//uint32_t ipv4_dot_split_to_uint32(char* ip_dot_split) {
//    if (inet_aton(ip_dot_split, (struct in_addr*)&addr.sin_addr.s_addr) == 0) {
//        return -1;
//    }
//
//    struct in_addr in_address;
//    in_address.s_addr = int_ip;
//    char* address = inet_ntoa(in_address);
//    return address;
//}


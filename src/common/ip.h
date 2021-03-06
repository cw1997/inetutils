//
// Created by cw1997 on 2021/5/10.
//

#ifndef COMMON_IP_H
#define COMMON_IP_H

#include <stdint.h>
#include <arpa/inet.h>

typedef struct {
    uint8_t        version:4;
    uint8_t        header_length:4;
    uint8_t        type;
    uint16_t       total_length;
    uint16_t       id;
    uint16_t       offset;
    uint8_t        ttl;
    uint8_t        protocol;
    uint16_t       checksum;
    struct in_addr source;
    struct in_addr destination;
} ip_header_t;

char* ipv4_uint32_to_dot_split(uint32_t ipv4_uint32);
//uint32_t ipv4_dot_split_to_uint32(char* ip_dot_split);

#endif //COMMON_IP_H

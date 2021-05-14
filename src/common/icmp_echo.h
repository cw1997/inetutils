//
// Created by cw1997 on 2021/5/9.
//

#ifndef COMMON_ICMP_ECHO_H
#define COMMON_ICMP_ECHO_H

#include <stdint.h>

#define MTU_LENGTH 1500

uint64_t get_timestamp_us();

int ping(const char *ip, const char *name, uint32_t count, uint32_t timeout, uint16_t ttl, uint16_t body_length);

#endif //COMMON_ICMP_ECHO_H

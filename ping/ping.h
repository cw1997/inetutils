//
// Created by cw1997 on 2021/5/9.
//

#ifndef PING_C_PING_H
#define PING_C_PING_H

#include <stdint.h>

#define MTU_LENGTH 1500

uint64_t get_timestamp_us();

ping(const char *host, uint32_t count, uint32_t timeout);

#endif //PING_C_PING_H

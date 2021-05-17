//
// Created by 86759 on 2021/5/17.
//

#ifndef INETUTILS_UTILS_H
#define INETUTILS_UTILS_H

#include <stdint.h>

struct timeval get_timeval();
uint64_t get_timestamp_us();
uint64_t get_timestamp_ms();
uint64_t get_timestamp_s();

#endif //INETUTILS_UTILS_H

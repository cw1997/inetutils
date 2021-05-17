//
// Created by 86759 on 2021/5/17.
//

#include <sys/time.h>
#include <stdio.h>
#include <memory.h>

#include "utils.h"

struct timeval get_timeval() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}

uint64_t get_timestamp_us() {
    struct timeval tv = get_timeval();
    return (uint64_t)(tv.tv_sec * 1000000 + tv.tv_usec); // us
}

uint64_t get_timestamp_ms() {
    struct timeval tv = get_timeval();
    return (uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000); // ms
}

uint64_t get_timestamp_s() {
    struct timeval tv = get_timeval();
    return tv.tv_sec;
}

uint64_t get_timestamp_mock() {
    uint64_t ts64;
    uint8_t ts8[8] = {0x67, 0x23, 0xa2, 0x60, 0x00, 0x00, 0x00, 0x00};
    memcpy(&ts64, ts8, sizeof(uint64_t));
    return ts64;
}


//
// Created by cw1997 on 2021/5/9.
//

#ifndef COMMON_ICMP_ECHO_H
#define COMMON_ICMP_ECHO_H

#include <stdint.h>

#define MTU_LENGTH 1500

uint64_t get_timestamp_us();
int send_echo_request(int sock, const struct sockaddr_in* addr, uint16_t identifier, uint16_t sequence_number, void* body, uint16_t body_length);
int recv_echo_reply(int sock, char* buffer, uint16_t buffer_length);

#endif //COMMON_ICMP_ECHO_H

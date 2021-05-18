//
// Created by cw1997 on 2021/5/9.
//

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <malloc.h>

#include "icmp_echo.h"
#include "icmp.h"

int send_echo_request(const int sock, const struct sockaddr_in* addr, const uint16_t identifier, const uint16_t sequence_number, void* body, uint16_t body_length) {
    uint16_t buffer_length = sizeof(icmp_t) + body_length;
    void* buffer = malloc(buffer_length);
    uint8_t* icmp_packet = build_icmp_packet(buffer, buffer_length, ICMP_TYPE_ECHO_REQUEST, 0, identifier, sequence_number, body, body_length);
    int length = sendto(sock, icmp_packet, buffer_length, 0, (struct sockaddr *)addr, sizeof(*addr));
    return length;
}

int recv_echo_reply(const int sock, char* buffer, uint16_t buffer_length) {
    struct sockaddr_in peer_addr;

    socklen_t addr_len = sizeof(peer_addr);
    int length = recvfrom(sock, buffer, buffer_length, 0, (struct sockaddr *)&peer_addr, &addr_len);
    if (length == -1) {
        perror("timeout.\n");
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
//            perror("EAGAIN or EWOULDBLOCK.\n");
            return 0;
        }
        return -1;
    }

    return length;
}

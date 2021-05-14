//
// Created by cw1997 on 2021/5/9.
//

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>

#include "ping.h"

#include "ip.h"
#include "icmp.h"

uint64_t last_timestamp_us;

uint64_t get_timestamp_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
//    return (uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000); // ms
    return (uint64_t)(tv.tv_sec * 1000000 + tv.tv_usec); // us
}

int send_echo_request(const int sock, const struct sockaddr_in* addr, const uint16_t identifier, const uint16_t sequence_number, void* body, uint16_t body_length) {
    last_timestamp_us = get_timestamp_us();
    uint8_t* icmp_packet = build_icmp_packet(ICMP_TYPE_ECHO_REQUEST, 0, identifier, sequence_number, body, body_length);
    uint16_t packet_length = sizeof(icmp_header_t) + body_length;
    int length = sendto(sock, icmp_packet, packet_length, 0, (struct sockaddr *)addr, sizeof(*addr));
    return length;
}

int recv_echo_reply(const int sock, const int identifier, const char* ip, const char* name) {
    char buffer[MTU_LENGTH];
    struct sockaddr_in peer_addr;

    socklen_t addr_len = sizeof(peer_addr);
    int length = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&peer_addr, &addr_len);
    uint64_t now_timestamp_us = get_timestamp_us();
    if (length == -1) {
        perror("timeout.\n");
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
//            perror("EAGAIN or EWOULDBLOCK.\n");
            return 0;
        }
        return -1;
    }

    ip_header_t* ip_header = (ip_header_t *)(buffer);
    icmp_header_t* icmp_header = (icmp_header_t *)(buffer + sizeof(ip_header_t));
    void* icmp_body_recv = (void *)(buffer + sizeof(ip_header_t) + sizeof(icmp_header_t));

    if (ntohs(icmp_header->identifier) != identifier) {
        printf("identifier is not match.\n");
        return -1;
    }

    uint16_t bytes = length - sizeof(ip_header_t) - sizeof(icmp_header_t);
//    if (memcmp(icmp_body_send, icmp_body_recv, ))
//    uint64_t last_timestamp_us = (*(uint64_t *)icmp_body);
    double duration_ms = (double)(now_timestamp_us - last_timestamp_us) / 1000;
//    printf("duration %lu, now_timestamp_us %lu, last_timestamp_us %lu \n", duration, now_timestamp_us, last_timestamp_us);
    struct in_addr in_address = ip_header->source;
    char* address = inet_ntoa(in_address);
    uint16_t sequence_number = ntohs(icmp_header->sequence_number);
    uint16_t ttl = ip_header->ttl;
    printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%5.3f ms\n", bytes, name, address, sequence_number, ttl, duration_ms);

    return 0;
}

int ping(const char *ip, const char *name, uint32_t count, uint32_t timeout, uint16_t ttl, uint16_t body_length) {
    printf("PING %s (%s) %d bytes of data.\n", name, ip, 64);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    if (inet_aton(ip, (struct in_addr*)&addr.sin_addr.s_addr) == 0) {
        return -1;
    }

    struct protoent *protocol;
    if ((protocol = getprotobyname("icmp")) == NULL) {
        perror("getprotobyname(icmp) error.\n");
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_RAW, protocol->p_proto/*IPPROTO_ICMP*/);
    if (sock == -1) {
        perror("create socket error.\n");
        return -1;
    }

    int ret;

    if (timeout < 1) {
        timeout = 1;
    }
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret == -1) {
        perror("set socket timeout option error.\n");
        return -1;
    }

    if (ttl > 0) {
        ret = setsockopt(sock, IPPROTO_IP,IP_TTL, &ttl, sizeof(ttl));
        if (ret == -1) {
            perror("set socket TTL option error.\n");
            return -1;
        }
    }

    if (body_length <= 0) {
        body_length = 32;
    }
    char body[body_length];

    uint16_t identifier = getpid();
    for (uint16_t sequence_number = 1; sequence_number <= count; ++sequence_number) {
        ret = send_echo_request(sock, &addr, identifier, sequence_number, body, body_length);
        if (ret == -1) {
            perror("send failed.\n");
//            continue;
        }

        ret = recv_echo_reply(sock, identifier, ip, name);
        if (ret == -1) {
            perror("receive failed.\n");
        }

        sleep(1);
    }

    return 0;
}

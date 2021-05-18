#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "../common/ip.h"
#include "../common/icmp_echo.h"
#include "../common/resolve.h"
#include "../common/utils.h"

int traceroute(uint32_t ip_uint32, uint32_t timeout, uint16_t first_ttl, uint16_t max_hops, uint16_t body_length) {
    char* ip_dot_split = ipv4_uint32_to_dot_split(ip_uint32);
    char* name = get_name_by_ip(ip_uint32);
    printf("traceroute to %s (%s), %u hops max, %u byte packets\n", name, ip_dot_split, max_hops, body_length);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = ip_uint32;

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

    uint16_t identifier = getpid();

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

    if (body_length <= 0) {
        body_length = 32;
    }
    char body[body_length];

    uint32_t last_ip = 0;
    uint32_t ret_ip = 0;
    for (int ttl = first_ttl; ttl <= first_ttl + max_hops; ++ttl) {
        ret = setsockopt(sock, IPPROTO_IP,IP_TTL, (char *)&ttl, sizeof(ttl));
        if (ret == -1) {
            perror("set socket TTL option error.\n");
            return -1;
        }

        for (uint32_t sequence_number = 1; sequence_number <= 3; ++sequence_number) {
            uint64_t last_timestamp_us = get_timestamp_us();
            ret = send_echo_request(sock, &addr, identifier, sequence_number, body, body_length);
            if (ret == -1) {
                perror("send failed.\n");
//            continue;
            }

            char* buffer = malloc(MTU_LENGTH);
            ret = recv_echo_reply(sock, buffer, MTU_LENGTH);
            uint64_t now_timestamp_us = get_timestamp_us();
            if (ret == -1) {
                perror("receive failed.\n");
            }

            struct iphdr* ip_header = (struct iphdr *)(buffer);
            struct icmphdr* icmp_header = (struct icmphdr *)(buffer + sizeof(struct iphdr));
            void* icmp_body_recv = (void *)(buffer + sizeof(struct iphdr) + sizeof(struct icmphdr));

            ret_ip = ip_header->saddr;
            if (last_ip == ret_ip) {
                printf("\n");
                return 0;
            }

            if (sequence_number == 1) {
                printf("%d\t%s(%s)\t", ttl, get_name_by_ip(ret_ip), ipv4_uint32_to_dot_split(ret_ip));
            }

            double duration_ms = (double)(now_timestamp_us - last_timestamp_us) / 1000;
            printf(" %5.3f ms", duration_ms);

//            sleep(1);
        }
        last_ip = ret_ip;

        printf("\n");
    }
    return 0;
}

void print_help() {
    printf("cw1997/inetutils/traceroute | code by cw1997 | [changwei1006@gmail.com, https://changwei.me]\n");
    printf("Usage:  traceroute target_name [-h] [-f first TTL] [-m max hops] [-l size] [-w timeout]\n");
    printf("\t-h\tDisplays help at the command prompt.\n");
    printf("\t-f\tSpecifies with what TTL to start. Defaults to 1.\n");
    printf("\t-m\tSpecifies the maximum number of hops (max time-to-live value) traceroute will probe. The default is 30.\n");
    printf("\t-l\tSpecifies the length, in bytes, of the Data field in the echo Request messages. The default is 32. The maximum size is 65,527.\n");
    printf("\t-w\tSpecifies the amount of time, in milliseconds, to wait for the echo Reply message corresponding to a given echo Request message. If the echo Reply message is not received within the time-out, the \"Request timed out\" error message is displayed. The default time-out is 4000 (4 seconds).\n");
}

int main(int argc, const char* argv[]) {
    char c;

    if (argc < 2) {
        print_help();
        exit(0);
    }
    const char* name = argv[1];
    uint16_t timeout = 1;
    uint16_t first_ttl = 1;
    uint16_t max_hops = 30;
    uint16_t length = 64;

    while(EOF != (c = getopt(argc, (char *const *) argv, "hf:m:l:w:"))) {
        switch(c) {
            case 'h':
                print_help();
                break;
            case 'f': // first ttl
                first_ttl = (uint16_t)atoi(optarg);
                break;
            case 'm': // size
                length = (uint16_t)atoi(optarg);
                break;
            case 'i': // max_hops
                max_hops = (uint16_t)atoi(optarg);
                break;
            case 'w': // timeout
                timeout = (uint32_t)atoi(optarg);
                break;
            case '?':
            default:
//                printf("Argument error.\n");
                print_help();
                exit(0);
                break;
        }
    }

    uint32_t ip_uint32 = get_ip_by_name(name);
    return traceroute(ip_uint32, timeout, first_ttl, max_hops, length);
}

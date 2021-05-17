#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "../common/icmp.h"
#include "../common/icmp_echo.h"
#include "../common/resolve.h"
#include "../common/ip.h"


int ping(uint32_t ip_uint32, uint32_t count, uint32_t timeout, uint16_t ttl, uint16_t body_length) {
    char* ip_dot_split = ipv4_uint32_to_dot_split(ip_uint32);
    char* name = get_name_by_ip(ip_uint32);
    printf("PING %s (%s) %hu(%lu) bytes of data.\n", name, ip_dot_split, body_length, body_length + sizeof(struct iphdr) + sizeof(icmp_header_t));

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

    if (ttl > 0) {
        ret = setsockopt(sock, IPPROTO_IP,IP_TTL, (char *)&ttl, sizeof(ttl));
        if (ret == -1) {
            perror("set socket TTL option error.\n");
            return -1;
        }
    }

    for (uint32_t sequence_number = 1; sequence_number <= count; ++sequence_number) {
        uint64_t last_timestamp_us = get_timestamp_us();
        ret = send_echo_request(sock, &addr, identifier, sequence_number, body, body_length);
        if (ret == -1) {
            perror("send failed.\n");
//            continue;
        }

        char* buffer = malloc(MTU_LENGTH);;
        ret = recv_echo_reply(sock, buffer, MTU_LENGTH);
        uint64_t now_timestamp_us = get_timestamp_us();
        if (ret == -1) {
            perror("receive failed.\n");
        }
        int length = ret;

        struct iphdr* ip_header = (struct iphdr *)(buffer);
        struct icmphdr* icmp_header = (struct icmphdr *)(buffer + sizeof(struct iphdr));
        void* icmp_body_recv = (void *)(buffer + sizeof(struct iphdr) + sizeof(struct icmphdr));

//    if (ntohs(icmp_header->identifier) != identifier) {
//        printf("identifier is not match, ntohs(icmp_header->identifier): %u identifier: %u\n", ntohs(icmp_header->identifier), identifier);
//        return -1;
//    }

        uint16_t bytes = length - sizeof(struct iphdr) - sizeof(icmp_t);
        double duration_ms = (double)(now_timestamp_us - last_timestamp_us) / 1000;
//    printf("duration %f, now_timestamp_us %lu, last_timestamp_us %lu \n", duration_ms, now_timestamp_us, last_timestamp_us);
        char* ret_ip = ipv4_uint32_to_dot_split(ip_header->saddr);
        char* ret_name = get_name_by_ip(ip_header->saddr);
        uint16_t ret_sequence_number = ntohs(icmp_header->un.echo.sequence);
        uint8_t ret_ttl = ip_header->ttl;
        printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%5.3f ms\n", bytes, ret_name, ret_ip, ret_sequence_number, ret_ttl, duration_ms);

        if (sequence_number <= count) {
            sleep(1);
        }
    }

    return 0;
}

void print_help() {
    printf("cw1997/inetutils/traceroute | code by cw1997 | [changwei1006@gmail.com, https://changwei.me]\n");
    printf("Usage:  traceroute target_name [-h] [-t] [-n count] [-l size] [-i TTL] [-w timeout]\n");
    printf("\t-h\tDisplays help at the command prompt.\n");
    printf("\t-t\tSpecifies ping continue sending echo Request messages to the destination until interrupted. To interrupt and display statistics, press CTRL+ENTER. To interrupt and quit this command, press CTRL+C.\n");
    printf("\t-n\tSpecifies the number of echo Request messages be sent. The default is 4.\n");
    printf("\t-l\tSpecifies the length, in bytes, of the Data field in the echo Request messages. The default is 32. The maximum size is 65,527.\n");
    printf("\t-i\tSpecifies the value of the Time To Live (TTL) field in the IP header for echo Request messages sent. The default is the default TTL value for the host. The maximum TTL is 255.\n");
    printf("\t-w\tSpecifies the amount of time, in milliseconds, to wait for the echo Reply message corresponding to a given echo Request message. If the echo Reply message is not received within the time-out, the \"Request timed out\" error message is displayed. The default time-out is 4000 (4 seconds).\n");
}

int main(int argc, const char* argv[]) {
    char c;

    if (argc < 2) {
        print_help();
        exit(0);
    }
    const char* name = argv[1];
    uint32_t count = 4;
    uint16_t timeout = 1;
    uint16_t ttl = 30;
    uint16_t length = 64;

    while(EOF != (c = getopt(argc, (char *const *) argv, "htn:l:i:w:"))) {
        switch(c) {
            case 'h':
                print_help();
                break;
            case 't': // stop Manual
                count = (uint32_t)(-1);
                break;
            case 'n': // ping count
                count = (uint16_t)atoi(optarg);
                break;
            case 'l': // size
                length = (uint16_t)atoi(optarg);
                break;
            case 'i': // ttl
                ttl = (uint16_t)atoi(optarg);
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
    return ping(ip_uint32, count, timeout, ttl, length);
}

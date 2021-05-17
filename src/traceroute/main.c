#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../common/icmp_echo.h"
#include "../common/resolve.h"

int traceroute(const char *ip, const char *name, uint32_t timeout, uint16_t max_hops, uint16_t body_length) {
    printf("traceroute to %s (%s), %u hops max, %u byte packets\n", name, ip, max_hops, body_length);

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

    for (int ttl = 1; ttl <= max_hops; ++ttl) {
        ret = setsockopt(sock, IPPROTO_IP,IP_TTL, (char *)&ttl, sizeof(ttl));
        if (ret == -1) {
            perror("set socket TTL option error.\n");
            return -1;
        }

        for (uint32_t sequence_number = 1; sequence_number <= 3; ++sequence_number) {
            ret = send_echo_request(sock, &addr, identifier, sequence_number, body, body_length);
            if (ret == -1) {
                perror("send failed.\n");
//            continue;
            }

            char *buffer = NULL;
            ret = recv_echo_reply(sock, buffer);
            if (ret == -1) {
                perror("receive failed.\n");
            }

//            sleep(1);
        }

    }
}

void print_help() {
    printf("cw1997/inetutils/ping | code by cw1997 | [changwei1006@gmail.com, https://changwei.me]\n");
    printf("Usage:  ping target_name [-h] [-t] [-n count] [-l size] [-i TTL] [-w timeout]\n");
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
    uint16_t timeout = 1;
    uint16_t max_hops = 30;
    uint16_t length = 64;

    while(EOF != (c = getopt(argc, (char *const *) argv, "htn:l:i:w:"))) {
        switch(c) {
            case 'h':
                print_help();
                break;
            case 'l': // size
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

    char* ip = get_ip_by_name(name);
//    printf("ip: %s\n", ip);
    return traceroute(ip, name, timeout, max_hops, length);
}

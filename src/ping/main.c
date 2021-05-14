#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../common/icmp_echo.h"
#include "../common/resolve.h"

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

    char* ip = get_ip_by_name(name);
//    printf("ip: %s\n", ip);
    return ping(ip, name, count, timeout, ttl, length);
}

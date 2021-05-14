//
// Created by cw1997 on 2021/5/14.
//

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <memory.h>

#include "resolve.h"

char* get_ip_by_name(const char* name) {
    struct hostent *host = gethostbyname(name);
    if (host == NULL) {
        perror("gethostbyname error");
    }
    printf("host->h_name: %s\n", host->h_name);
    printf("host->h_length: %d\n", host->h_length);
    for (int i = 0; i < host->h_length; ++i) {
        printf("host->h_addr_list[i]: %s\n", host->h_addr_list[i]);
    }
//    printf("host->h_addr: %s\n", host->h_addr);
    struct in_addr in;
    struct sockaddr_in addr_in;
    memcpy(&addr_in.sin_addr.s_addr, host->h_addr, 4);
    in.s_addr=addr_in.sin_addr.s_addr;
    char* ip = inet_ntoa(in);
    return ip;
}

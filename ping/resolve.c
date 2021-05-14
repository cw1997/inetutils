//
// Created by cw1997 on 2021/5/14.
//

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <memory.h>

#include "resolve.h"

char* address2str(char* address) {
    struct in_addr in;
    struct sockaddr_in addr_in;
    memcpy(&addr_in.sin_addr.s_addr, address, 4);
    in.s_addr=addr_in.sin_addr.s_addr;
    char* ip = inet_ntoa(in);
    return ip;
}

char* get_ip_by_name(const char* name) {
    struct hostent *host = gethostbyname(name);
    if (host == NULL) {
        printf("gethostbyname error.\n");
    }
    char* ip = address2str(host->h_addr);
    return ip;
}

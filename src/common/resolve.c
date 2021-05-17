//
// Created by cw1997 on 2021/5/14.
//

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <memory.h>

#include "resolve.h"
#include "ip.h"

uint32_t get_ip_by_name(const char* name) {
    struct hostent *host = gethostbyname(name);
    if (host == NULL) {
        printf("gethostbyname error.\n");
    }
//    char* ip = host_addr_to_str_addr(host->h_addr);
    struct sockaddr_in addr_in;
    memcpy(&addr_in.sin_addr.s_addr, host->h_addr, 4);
    uint32_t ip = addr_in.sin_addr.s_addr;
    return ip;
}

char* get_name_by_ip(uint32_t ipv4_uint32) {
    struct hostent *host;
    struct in_addr* in_addr = (struct in_addr *)&ipv4_uint32;
    if((host = gethostbyaddr(in_addr, 4, AF_INET) ) == NULL) {
//        h_errno;
//        printf("gethostbyaddr error %u\n", ipv4_uint32);
//        switch(h_errno) {
//            case HOST_NOT_FOUND:printf("111\n");break;
////            case NO_ADDRESS:
//            case NO_DATA:printf("112\n");break;
//            case NO_RECOVERY:printf("113\n");break;
//            case TRY_AGAIN:printf("115\n");break;
//        }
//        return "[Invalid Domain Name.]";
        return ipv4_uint32_to_dot_split(ipv4_uint32);
    }

    return host->h_name;
}

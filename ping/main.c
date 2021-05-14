#include <stdio.h>

#include "ping.h"
#include "resolve.h"

int main(int argc, const char* argv[]) {
//    const char host[] = "8.8.8.8";
    const char name[] = "google.com";
    char* ip = get_ip_by_name(name);
    printf("ip: %s\n", ip);
    return ping(ip, name, 4, 1, 30, 64);
}

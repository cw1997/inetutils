#include <stdio.h>

#include "ping.h"

int main(int argc, const char* argv[]) {
    const char host[] = "8.8.8.8";
    return ping(host, 4, 1);
}

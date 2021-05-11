//
// Created by cw1997 on 2021/5/10.
//

#include <arpa/inet.h>
#include <stdlib.h>
#include <memory.h>

#include "icmp.h"

uint16_t calculate_checksum(uint8_t* buffer, int length) {
    uint32_t checksum = 0;
    unsigned char* end = buffer + length;

    // odd bytes add last byte and reset end
    if (length % 2 == 1) {
        end = buffer + length - 1;
        checksum += (*end) << 8;
    }

    // add words of two bytes, one by one
    while (buffer < end) {
        checksum += buffer[0] << 8;
        checksum += buffer[1];
        buffer += 2;
    }

    // add carry if any
    uint32_t carry = checksum >> 16;
    while (carry) {
        checksum = (checksum & 0xffff) + carry;
        carry = checksum >> 16;
    }

    // negate it
    checksum = ~checksum;

    return checksum & 0xffff;
}

uint8_t* build_icmp_packet(uint8_t type, uint8_t code, uint16_t identifier, uint16_t sequence_number, uint8_t *body,
                           uint16_t body_length) {
    uint16_t buffer_length = sizeof(icmp_header_t) + body_length;
    void* buffer = malloc(buffer_length);

    icmp_header_t icmp_header;
    icmp_header.type = type;
    icmp_header.code = code;
    icmp_header.checksum = 0;
    icmp_header.identifier = htons(identifier);
    icmp_header.sequence_number = htons(sequence_number);
//    TODO: improving performance: reduce memcpy count
    memcpy(buffer, &icmp_header, sizeof(icmp_header_t));
    memcpy(buffer + sizeof(icmp_header_t), body, body_length);

    icmp_header.checksum = htons(calculate_checksum(buffer, buffer_length));
    memcpy(buffer, &icmp_header, sizeof(icmp_header_t));

    return buffer;
};

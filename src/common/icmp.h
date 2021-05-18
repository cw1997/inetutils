//
// Created by cw1997 on 2021/5/10.
//

#ifndef COMMON_ICMP_H
#define COMMON_ICMP_H

#include <stdint.h>
#include <netinet/ip_icmp.h>

#define Network_Unreachable  0 // （無法到達目的網路）
#define Host_Unreachable  1 // （無法到達目的主機）
#define Protocol_Unreachable  2 // （通訊協定不存在）
#define Port_Unreachable  3 // （無法到達連接埠）
#define Fragmentation_Needed_and_DF_set  4 // （資料需分割並設定不可分割位元）
#define Source_Route_Failed  5 // （來源路徑選擇失敗）
#define Destination_Network Unknown  6 // （無法識別目的地網路）
#define Destination_Host_Unknown  7 // （無法識別目的地主機）
#define Source_Host_Isolated  8 // （來源主機被隔離）
#define Communication_with_Destination_Network_Administratively_Prohibited  9 // （管理上禁止和目的地網路通訊）
#define Communication_with_Destination_Host_Administratively_Prohibited  10// （管理上禁止和目的地主機通訊）
#define Network_Unreachable_for_Type_of_Service  11 // （無法到達此型態的網路服務）
#define Host Unreachable_for_Type_of_Service  12 // （無法到達此型態的主機服務）


#define ICMP_TYPE_ECHO_REQUEST 8

typedef struct {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;

    uint16_t identifier;
    uint16_t sequence_number;
} icmp_header_t;

typedef struct {
    struct timeval timeval;
} icmp_body_t;

typedef struct {
    icmp_header_t icmp_header;
    icmp_body_t icmp_body;
} icmp_t;

//typedef struct {
//    icmp_header_t header;
//    uint8_t body[];
//} icmp_t;

uint8_t* build_icmp_packet(void* buffer, uint16_t buffer_length, uint8_t type, uint8_t code, uint16_t identifier, uint16_t sequence_number, void* body,
                           uint16_t body_length);

#endif //COMMON_ICMP_H

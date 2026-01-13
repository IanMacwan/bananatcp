#ifndef UDP_H
#define UDP_H

#include "packet.h"

typedef struct {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
} __attribute__((packed)) udp_hdr_t;

void udp_handle(packet_t *pkt, void *ip_hdr);

#endif

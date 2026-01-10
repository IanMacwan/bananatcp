#include "../include/tcp.h"
#include <stdio.h>

typedef struct {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq;
    uint32_t ack;
    uint8_t  offset;
    uint8_t  flags;
    uint16_t window;
    uint16_t checksum;
    uint16_t urg_ptr;
} __attribute__((packed)) tcp_hdr_t;

void tcp_handle(packet_t *pkt, void *ip_hdr) {
    return;
}

#ifndef TCP_H
#define TCP_H

#include "packet.h"
#include "ipv4.h"

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

typedef enum {
    TCP_CLOSED,
    TCP_LISTEN,
    TCP_SYN_RECEIVED,
    TCP_ESTABLISHED,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_IGNORE // TIME_WAIT implementation
} tcp_state_t;

void tcp_init(void);
void tcp_handle(packet_t *pkt, void *ip_hdr);
void tcp_send(packet_t *pkt, ipv4_hdr_t *ip, tcp_hdr_t *tcp);

#endif

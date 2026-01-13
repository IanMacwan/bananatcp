#ifndef ICMP_H
#define ICMP_H

#include "packet.h"

typedef struct {
  uint8_t type;
  uint8_t code;
  uint16_t checksum;
  uint16_t id;
  uint16_t seq;
} __attribute__((packed)) icmp_hdr_t;

void icmp_handle(packet_t *pkt, void *ip_hdr);

#endif

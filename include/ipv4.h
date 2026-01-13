#ifndef IPV4_H
#define IPV4_H

#include "packet.h"

typedef struct {
  uint8_t ver_ihl; // Verion and header lenght
  uint8_t tos; // DSCP / ECN
  uint16_t total_length;
  uint16_t id;
  uint16_t flags_frag;
  
  uint8_t ttl;
  uint8_t protocol;
  uint16_t checksum;
  uint32_t src_ip;
  uint32_t dst_ip;
} __attribute__((packed)) ipv4_hdr_t;


void ipv4_handle(packet_t *pkt);
void ipv4_send(packet_t *pkt, void *hdr);

#endif

#include "../include/udp.h"
#include <arpa/inet.h>
#include <stdio.h>

typedef struct {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
} __attribute__((packed)) udp_hdr_t;

void udp_handle(packet_t *pkt, void *ip_hdr) {
  
  udp_hdr_t *udp = packet_pull(pkt, sizeof(udp_hdr_t));
  if (!udp) return;

  uint16_t port = ntohs(udp->dst_port);
  printf("🙊 udp: packet to port %u (%zu bytes payload)\n", port, packet_remaining(pkt));
}

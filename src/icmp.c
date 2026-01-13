#include "../include/icmp.h"
#include "../include/checksum.h"
#include "../include/ipv4.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

typedef struct {
  uint8_t type;
  uint8_t code;
  uint16_t checksum;
  uint16_t id;
  uint16_t seq;
} __attribute__((packed)) icmp_hdr_t;

void icmp_handle(packet_t *pkt, void *ip_hdr) {

  icmp_hdr_t *icmp = packet_pull(pkt, sizeof(icmp_hdr_t));
  if (!icmp) return;

  if (icmp->type != 8) return; // echo req
  
  icmp->type = 0; // echo reply (not yet sent back)
  icmp->checksum = 0;
  icmp->checksum = checksum16(icmp, sizeof(icmp_hdr_t) + packet_remaining(pkt));

  printf("🙊 icmp: echo request received id=%u seq=%u\n", ntohs(icmp->id), ntohs(icmp->seq));

  ipv4_send(pkt, ip_hdr);
}

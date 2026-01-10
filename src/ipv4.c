#include "../include/ipv4.h"
#include "../include/icmp.h"
#include "../include/udp.h"
#include "../include/tcp.h"
#include "../include/checksum.h"
#include <stdio.h>
#include <arpa/inet.h>

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

void ipv4_handle(packet_t *pkt) {

  ipv4_hdr_t *hdr = packet_pull(pkt, sizeof(ipv4_hdr_t));
  if (!hdr) return;
  
  // bitwise right shift to exclude ihl
  if ((hdr->ver_ihl >> 4) != 4) return;
  
  uint16_t old = hdr->checksum;
  hdr->checksum = 0;

  if (checksum16(hdr, sizeof(ipv4_hdr_t)) != old) {
    printf("❓ ipv4: invalid checksum, dropping packet\n");
    return;
  }

  // Log the basic info
    struct in_addr src, dst;
    src.s_addr = hdr->src_ip;
    dst.s_addr = hdr->dst_ip;
    printf("🌴 ipv4: received packet from %s to %s, protocol=%u, payload=%zu bytes\n", inet_ntoa(src), inet_ntoa(dst), hdr->protocol, packet_remaining(pkt));

  switch (hdr->protocol) {
    case 1:
      icmp_handle(pkt, hdr);
      break;
    case 16:
      tcp_handle(pkt, hdr);
      break;
    case 17:
      udp_handle(pkt, hdr);     
    default:
      printf("❓ ipv4: unsupported protocol %u\n", hdr->protocol);
      break;
  }
}


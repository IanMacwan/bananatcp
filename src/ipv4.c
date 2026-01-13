#include "../include/ipv4.h"
#include "../include/icmp.h"
#include "../include/udp.h"
#include "../include/tcp.h"
#include "../include/checksum.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

extern int tun_fd;

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

  switch (hdr->protocol) {
    case 1:
      icmp_handle(pkt, hdr);
      break;
    case 6:
      tcp_handle(pkt, hdr);
      break;
    case 17:
      udp_handle(pkt, hdr);
      break;
    default:
      printf("❓ ipv4: unsupported protocol %u\n", hdr->protocol);
      break;
  }
}

void ipv4_send(packet_t *pkt, void *hdr_ptr) {

  ipv4_hdr_t *hdr = (ipv4_hdr_t *)hdr_ptr;
  
  // Swap source and destination IPs
  uint32_t tmp = hdr->src_ip;
  hdr->src_ip = hdr->dst_ip;
  hdr->dst_ip = tmp;

  // Reset TTL and recalc checksum
  hdr->ttl = 64;
  hdr->checksum = 0;
  hdr->checksum = checksum16(hdr, sizeof(ipv4_hdr_t));

  ssize_t n = write(tun_fd, pkt->buffer, pkt->len);
  if (n < 0) {
    perror("ipv4_send: write");
  } else {
    printf("🌴 ipv4: sent reply (%zd bytes)\n", n);
  }
}

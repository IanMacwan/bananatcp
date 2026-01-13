#include "../include/checksum.h"
#include "../include/ipv4.h"
#include "../include/tcp.h"
#include <arpa/inet.h>

uint16_t checksum16(void *data, size_t len) {
  uint32_t sum = 0;
  uint16_t *ptr = data;
  
  while (len > 1) {
    sum += *ptr++;
    len -= 2;
  }
  
  // Add remaining byte if data is odd
  if (len) {
    sum += *(uint8_t *)ptr;
  }
  
  // Fold carry
  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  
  // One's comp
  return ~sum;
}

uint16_t tcp_checksum(ipv4_hdr_t *ip, tcp_hdr_t *tcp, size_t tcp_len) {
    uint32_t sum = 0;
    uint16_t *ptr;

    // Pseudo-header
    sum += (ip->src_ip >> 16) & 0xFFFF;
    sum += (ip->src_ip) & 0xFFFF;
    sum += (ip->dst_ip >> 16) & 0xFFFF;
    sum += (ip->dst_ip) & 0xFFFF;
    sum += htons(IPPROTO_TCP);
    sum += htons(tcp_len);

    // TCP header + payload
    ptr = (uint16_t *)tcp;
    while (tcp_len > 1) {
        sum += *ptr++;
        tcp_len -= 2;
    }

    if (tcp_len) {
        sum += *(uint8_t *)ptr;
    }

    // Fold carries
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ~sum;
}

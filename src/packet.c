#include "../include/packet.h"
#include <string.h>

void packet_init(packet_t *pkt, size_t len) {
  pkt->len = len;
  pkt->offset = 0;
}

void *packet_pull(packet_t *pkt, size_t size) {
  if (pkt->offset + size > pkt->len) {
    return NULL;
  }

  void *ptr = pkt->buffer + pkt->offset;
  pkt->offset += size;
  return ptr;
}

void *packet_data(packet_t *pkt) {
  return pkt->buffer + pkt->offset;
}

size_t packet_remaining(packet_t *pkt) {
  return pkt->len - pkt->offset;
}

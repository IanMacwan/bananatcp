#include "../include/checksum.h"

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

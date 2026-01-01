#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stddef.h>

#define PACKET_MAX_SIZE 2048

// Mutable packet buffer
typedef struct {
  uint8_t buffer[PACKET_MAX_SIZE];
  size_t len;
  size_t offset; // current parse position
} packet_t;

void packet_init(packet_t *pkt, size_t len);

// Consume bytes from the front
void *packet_pull(packet_t *pkt, size_t size);

// Get pointer to remaining payload
void *packet_data(packet_t *pkt); 

// Remaining bytes
size_t packet_remaining(packet_t *pkt);

#endif

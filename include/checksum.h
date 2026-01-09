#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stddef.h>
#include <stdint.h>

uint16_t checksum16(void *data, size_t len);

#endif

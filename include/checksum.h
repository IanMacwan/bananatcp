#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stddef.h>
#include <stdint.h>
#include "ipv4.h"
#include "tcp.h"

uint16_t checksum16(void *data, size_t len);
uint16_t tcp_checksum(ipv4_hdr_t *ip, tcp_hdr_t *tcp, size_t tcp_len);

#endif

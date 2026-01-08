#ifndef TCP_H
#define TCP_H

#include "packet.h"

void tcp_handle(packet_t *pkt, void *ip_hdr);

#endif

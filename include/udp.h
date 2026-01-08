#ifndef UDP_H
#define UDP_H

#include "packet.h"

void udp_handle(packet_t *pkt, void *ip_hdr);

#endif

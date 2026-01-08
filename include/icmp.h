#ifndef ICMP_H
#define ICMP_H

#include "packet.h"

void icmp_handle(packet_t *pkt, void *ip_hdr);

#endif

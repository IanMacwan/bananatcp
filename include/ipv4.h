#ifndef IPV4_H
#define IPV4_H

#include "packet.h"

void ipv4_handle(packet_t *pkt);
void ipv4_send(packet_t *pkt, void *hdr);

#endif

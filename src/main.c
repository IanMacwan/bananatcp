#include "../include/tun.h"
#include "../include/packet.h"
#include "../include/ipv4.h"
#include <unistd.h>
#include <stdio.h>

int main() {

  int tun = tun_create("bananatun0");
  if (tun < 0) {
    perror("tun error");
    return 1;
  }

  packet_t pkt;

  while (1) {

    ssize_t n = read(tun, pkt.buffer, PACKET_MAX_SIZE);

    if (n <= 0) continue;

    packet_init(&pkt, n);
    ipv4_handle(&pkt);
  }
}

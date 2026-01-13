#include "../include/tun.h"
#include "../include/packet.h"
#include "../include/ipv4.h"
#include <unistd.h>
#include <stdio.h>

int tun_fd;

int main() {
  printf("\033[33m");
  printf(
    " _\n"
    "//\\\n"
    "V  \\\n"
    " \\  \\_\n"
    "  \\,'.`-."
  );
  printf("\033[31m");
  printf("           bananatcp\n");
  printf("\033[33m");
  printf(
    "   |\\ `. `."
  );
  printf("\033[31m");
  printf("   userspace tcp/ip stack\n");
  printf("\033[33m");
  printf(
    "   ( \\  `. `-.                        _,.-:\\\n"
    "    \\ \\   `.  `-._             __..--' ,-';/\n"
    "     \\ `.   `-.   `-..___..---'   _.--' ,'/\n"
    "      `. `.    `-._        __..--'    ,' /\n"
    "        `. `-_     ``--..''       _.-' ,'\n"
    "          `-_ `-.___        __,--'   ,'\n"
    "             `-.__  `----\"\"\"    __.-'\n"
    "                  `--..____..--'\n\n"
  );
  printf("\033[0m");

  tun_fd = tun_create("bananatun0");
  if (tun_fd < 0) {
    perror("tun error");
    return 1;
  }

  packet_t pkt;

  while (1) {

    ssize_t n = read(tun_fd, pkt.buffer, PACKET_MAX_SIZE);

    if (n <= 0) continue;

    packet_init(&pkt, n);
    ipv4_handle(&pkt);
  }
}

#include "../include/tun.h"
#include "../include/packet.h"
#include "../include/ipv4.h"
#include "../include/tcp.h"
#include <unistd.h>
#include <stdio.h>
#include <linux/if.h>

int tun_fd;

int main(int argc, char *argv[]) {
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
  printf("   |\\ `. `.");
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

  net_config cfg;
  char dev_name[IFNAMSIZ] = "bananatun0";

  if (argc == 1) {
    printf("🍌 Auto selecting subnet...\n");
    find_free_subnet(&cfg);
  } else if (argc == 3) {
    use_user_config(&cfg, argv[1], argv[2]);
  } else {
    printf("Usage:\n");
    printf("./prog             (auto)\n");
    printf("./prog <tun_ip> <peer_ip>\n");
    return 1;
  }

  printf("TUN IP : %s\n", cfg.tun_ip);
  printf("Peer   : %s\n", cfg.peer_ip);

  tun_fd = tun_create(dev_name, &cfg);
  if (tun_fd < 0) {
    perror("tun error");
    return 1;
  }

  configure_interface(dev_name, &cfg);

  packet_t pkt;
  tcp_init();

  while (1) {
    ssize_t n = read(tun_fd, pkt.buffer, PACKET_MAX_SIZE);
    if (n <= 0) continue;

    packet_init(&pkt, n);
    ipv4_handle(&pkt);
  }
}

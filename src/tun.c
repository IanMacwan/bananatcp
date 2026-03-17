#include "../include/tun.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <string.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

void use_user_config(net_config *cfg, char *tun, char *peer) {
  snprintf(cfg->tun_ip, sizeof(cfg->tun_ip), "%s/24", tun);
  snprintf(cfg->peer_ip, sizeof(cfg->peer_ip), "%s/32", peer);
}

int tun_create(const char *name, net_config *cfg) {
  struct ifreq ifr;
  int fd = open("/dev/net/tun", O_RDWR);
  if (fd < 0) return -1;

  // Clear ifr to avoid unexpected behavior due to union
  memset(&ifr, 0, sizeof(ifr));
    
  // Create TUN interface and give clean IP packets
  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

  if (*name) {
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
  }
  
  // Send control req to kernel
  if (ioctl(fd, TUNSETIFF, &ifr) < 0) {
    close(fd);
    return -1;
  }

  printf("🍌 created tun device: %s\n", ifr.ifr_name);
  return fd;
}

#include "../include/tun.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <string.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/wait.h>


int subnet_used(int candidate) {
 
  struct ifaddrs *ifaddr, *ifa;
  getifaddrs(&ifaddr);

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr) continue;

    if (ifa->ifa_addr->sa_family == AF_INET) {
      struct sockaddr_in *sa = (struct sockaddr_in*)ifa->ifa_addr;
      unsigned char *ip = (unsigned char*)&sa->sin_addr.s_addr;

      if (ip[0] == 10 && ip[1] == 0 && ip[2] == candidate) { 
        freeifaddrs(ifaddr);
        return 1;
      }
    }
  }

  freeifaddrs(ifaddr);
  return 0;
}

void find_free_subnet(net_config *cfg) {
    for (int i = 1; i < 255; i++) {
        if (!subnet_used(i)){
            sprintf(cfg->subnet, "10.0.%d.0/24", i);
            sprintf(cfg->tun_ip, "10.0.%d.1/24", i);
            sprintf(cfg->peer_ip, "10.0.%d.2/32", i);
            return;
        }
    }

    printf("no free subnet found\n");
    exit(1);
}

void use_user_config(net_config *cfg, char *tun, char *peer) {
  snprintf(cfg->tun_ip, sizeof(cfg->tun_ip), "%s/24", tun);
  snprintf(cfg->peer_ip, sizeof(cfg->peer_ip), "%s/32", peer);
}

int run_cmd(char *argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        execvp(argv[0], argv);
        perror("exec failed");
        _exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
    return status;
}

void configure_interface(const char *dev, net_config *cfg) {
    char *cmd1[] = {
        "ip", "addr", "add",
        cfg->tun_ip,
        "dev", (char *)dev,
        NULL
    };

    run_cmd(cmd1);

    char *cmd2[] = {
        "ip", "link", "set",
        (char *)dev,
        "up",
        NULL
    };

    run_cmd(cmd2);

    char *cmd3[] = {
        "ip", "route", "add",
        cfg->peer_ip,
        "dev", (char *)dev,
        NULL
    };

    run_cmd(cmd3);
}

int tun_create(char *name, net_config *cfg) {
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

#ifndef TUN_H
#define TUN_H

typedef struct {
  char tun_ip[34];
  char peer_ip[32];
  char subnet[32];
} net_config;

void use_user_config(net_config *cfg, char *tun, char *peer);

static int subnet_used(int candidate);

void find_free_subnet(net_config *cfg);

int run_cmd(char *argv[]);

int tun_create(const char *name, net_config *cfg);

#endif

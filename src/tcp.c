#include "../include/tcp.h"
#include "../include/ipv4.h"
#include "../include/checksum.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_ACK 0x10

typedef struct {
    tcp_state_t state;
    uint32_t snd_nxt;
    uint32_t rcv_nxt;
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t src_ip;
    uint32_t dst_ip;
} tcp_conn_t;

static tcp_conn_t conn;

void tcp_init(void) {
    memset(&conn, 0, sizeof(conn));
    conn.state = TCP_LISTEN;
}

void tcp_send(packet_t *pkt, ipv4_hdr_t *ip_hdr, tcp_hdr_t *tcp) {

  ipv4_hdr_t *ip = (ipv4_hdr_t *)ip_hdr;

  uint16_t tmp = tcp->src_port;
  tcp->src_port = tcp->dst_port;
  tcp->dst_port = tmp;

  tcp->offset = (sizeof(tcp_hdr_t) / 4) << 4;

  ip->total_length = htons(pkt->len);

  tcp->checksum = 0;
  tcp->checksum = tcp_checksum(ip, tcp, pkt->len - sizeof(ipv4_hdr_t));

  ipv4_send(pkt, ip_hdr);
}

void tcp_handle(packet_t *pkt, void *ip_hdr) {
    tcp_hdr_t *tcp = packet_pull(pkt, sizeof(tcp_hdr_t));
    if (!tcp) return;

    uint16_t src_port = ntohs(tcp->src_port);
    uint16_t dst_port = ntohs(tcp->dst_port);
    uint32_t seq = ntohl(tcp->seq);
    uint32_t ack = ntohl(tcp->ack);

    printf("🙉 tcp: state=%d flags=0x%x\n", conn.state, tcp->flags);

    switch (conn.state) {

    case TCP_LISTEN:
        if (tcp->flags & TCP_SYN) {
            ipv4_hdr_t *ip = (ipv4_hdr_t *)ip_hdr;

            conn.src_ip   = ip->dst_ip;
            conn.dst_ip   = ip->src_ip;
            conn.src_port = dst_port;
            conn.dst_port = src_port;

            conn.state = TCP_SYN_RECEIVED;
            conn.rcv_nxt = seq + 1;
            conn.snd_nxt = 1000; // arbitrary initial seq

            tcp->flags = TCP_SYN | TCP_ACK;
            tcp->ack = htonl(conn.rcv_nxt);
            tcp->seq = htonl(conn.snd_nxt);

            tcp_send(pkt, ip_hdr, tcp);

            conn.snd_nxt += 1;
        }
        break;

    case TCP_SYN_RECEIVED:
        if (tcp->flags & TCP_ACK) {
            conn.state = TCP_ESTABLISHED;
            printf("🙉 tcp: connection established\n");
        }
        break;

    case TCP_ESTABLISHED:
        if (packet_remaining(pkt) > 0) {
            printf("🙉 tcp: data received (%zu bytes)\n",
                   packet_remaining(pkt));

            conn.rcv_nxt += packet_remaining(pkt);

            tcp->flags = TCP_ACK;
            tcp->ack = htonl(conn.rcv_nxt);
            tcp->seq = htonl(conn.snd_nxt);

            tcp_send(pkt, ip_hdr, tcp);
        }

        if (tcp->flags & TCP_FIN) {
            conn.state = TCP_CLOSE_WAIT;
            conn.rcv_nxt += 1;

            tcp->flags = TCP_ACK;
            tcp->ack = htonl(conn.rcv_nxt);
            tcp_send(pkt, ip_hdr, tcp);
        }
        break;

    case TCP_CLOSE_WAIT:
        tcp->flags = TCP_FIN | TCP_ACK;
        tcp->seq = htonl(conn.snd_nxt++);
        tcp_send(pkt, ip_hdr, tcp);
        conn.state = TCP_LAST_ACK;
        break;

    case TCP_LAST_ACK:
        if (tcp->flags & TCP_ACK) {
            conn.state = TCP_CLOSED;
            printf("🙉 tcp: connection closed\n");
        }
        break;

    default:
        break;
    }
}

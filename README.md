# 🍌 bananatcp

> A minimal userspace TCP/IP stack built from scratch in C, featuring IPv4 parsing, ICMP echo replies, and a functional TCP state machine running over a Linux TUN interface.

![BananaTCP startup demo](images/start.png)

---
## Project Overview
bananatcp is a **learning-focused** but **technically rigorous** implementation of core **Internet protocols** in userspace, without relying on the kernel networking stack.

The project manually handles:
- Raw packet ingestion via a TUN interface
- IPv4 header parsing and checksum validation
- ICMP echo request handling and echo replies
- A simplified but correct TCP connection lifecycle, including state transitions, sequencing, acknowledgments, and teardown

This project was built to deeply understand how packets move from:
**wire** → **IPv4** → **transport protocol**, and how TCP behaves internally beyond high-level abstractions.

---
## Key Features
### IPv4 Layer
- Parses raw IPv4 headers from TUN
- Validates IPv4 header checksums
- Extracts and dispatches packets based on protocol field
- Forwards payloads to ICMP, TCP, or UDP handlers
### ICMP (Echo Request / Reply)
- Handles ICMPv4 Echo Requests (`ping`)
- Constructs and sends valid Echo Replies
- Recalculates ICMP and IPv4 checksums correctly
- Allows `ping` to succeed entirely in userspace
### TCP (Implemented From Scratch)
- Implements a **stateful TCP connection manager**
- Full TCP connection lifecycle support:
    - `LISTEN`
    - `SYN_RECEIVED`
    - `ESTABLISHED`
    - `CLOSE_WAIT`
    - `LAST_ACK`
    - `IGNORE`
- Handles:
    - SYN / SYN-ACK / ACK handshake
    - Sequence and acknowledgment number tracking
    - Data acknowledgment
    - FIN-based graceful connection teardown
- Successfully interoperates with real clients such as `nc`

---
## What I Learned
- How IPv4, ICMP, and TCP interact at the packet level
- Why TCP state machines are complex and error-prone
- How sequence numbers and ACKs advance in real traffic
- How userspace networking tools (VPNs, proxies, firewalls) work internally
- How to debug protocol behavior using Wireshark and raw packet inspection

---
## Testing & Usage
1. Build the project:
```bash
make
```
2. Run as root (required for TUN):
```bash
sudo ./bananatcp
```
3. Configure the interface (example):
```bash
sudo ip addr add 10.0.1.1/24 dev bananatun0
sudo ip link set tun0 up
sudo ip route add 10.0.1.2/32 dev bananatun0
```

The stack was tested using standard Linux networking tools:
- `ping` — verifies ICMP echo replies
- `nc` (netcat) — validates TCP handshake, data transfer, and clean shutdown
- `tcpdump` / Wireshark — confirms packet correctness on the wire
All packets observed match expected protocol behavior and header semantics.

> ⚠️ **Disclaimer**: This project is educational and not intended for production use (probably obvious). It prioritizes correctness, clarity, and learning over performance or security hardening.
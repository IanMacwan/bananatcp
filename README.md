# 🍌 bananatcp
**A linux userspace tcp/ip stack made with C and TUN using linux kernel**

## Overview
This project implements a **minimal tcp/ip stack** in **userspace**. That means it does the work that your operating system usually does parsing packets, checking headers, and responding to requests, but does it **all inside a single C program**.  

Why? Because sometimes you need to **peek under the hood** of networking, and doing it in Linux kernel space is scary. 😱

It supports:
- IPv4 parsing
- ICMP (ping)
- UDP
- TCP skeleton (yes, it’s like the scaffolding of a TCP handshake which is still under construction 🏗️)
- TUN device integration (virtual network interface, so no real cables needed)

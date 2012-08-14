/*****************************************************************************
* Product: lwIP consolidated file for QP-lwIP integration
* Last Updated for Version: lwIPv1.3.1 / QPv4.1.00
* Date of the Last Update:  Oct 12, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "lwip/opt.h"                            /* lwIP options come first */
#include "lwip/api.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/mem.h"

/* NOTE: all the following modules contain conditional compilation protection,
* so tht code will be actually generated only if the appropriate options are
* set in the lwipopts.h header file included from lwip/opt.h.
*/

/* Core lwIP TCP/IP stack ..................................................*/
#include "core/def.c"
#include "core/dhcp.c"
#include "core/dns.c"
#include "core/init.c"
#include "core/mem.c"
#include "core/memp.c"
#include "core/netif.c"
#include "core/pbuf.c"
#include "core/raw.c"
#include "core/stats.c"
#include "core/sys.c"
#include "core/tcp.c"
#include "core/tcp_in.c"
#include "core/tcp_out.c"
#include "core/timers.c"
#include "core/udp.c"

/* IPv4 ....................................................................*/
#include "core/ipv4/autoip.c"
#include "core/ipv4/icmp.c"
#include "core/ipv4/igmp.c"
#include "core/ipv4/inet.c"
#include "core/ipv4/inet_chksum.c"
#include "core/ipv4/ip.c"
#include "core/ipv4/ip_addr.c"
#include "core/ipv4/ip_frag.c"

/* SNMP ....................................................................*/
#include "core/snmp/asn1_dec.c"
#include "core/snmp/asn1_enc.c"
#include "core/snmp/mib2.c"
#include "core/snmp/mib_structs.c"
#include "core/snmp/msg_in.c"
#include "core/snmp/msg_out.c"

/* Ethernet interface ......................................................*/
#include "netif/etharp.c"
//???#include "netif/loopif.c"

/* PPP .....................................................................*/
#if PPP_SUPPORT
    #include "netif/ppp/auth.c"
    #include "netif/ppp/chap.c"
    #include "netif/ppp/chpms.c"
    #include "netif/ppp/fsm.c"
    #include "netif/ppp/ipcp.c"
    #include "netif/ppp/lcp.c"
    #include "netif/ppp/magic.c"
    #include "netif/ppp/md5.c"
    #include "netif/ppp/pap.c"
    #include "netif/ppp/ppp.c"
    #include "netif/ppp/ppp_oe.c"
    #include "netif/ppp/randm.c"
    #include "netif/ppp/vj.c"
#endif                                                       /* PPP_SUPPORT */

/* utilities added by QL ...................................................*/
/**
* Allocate a transport-layer pbuf and copies the provided data buffer 'data'
* of length 'len' bytes into the payload(s) of the pbuf. The function takes
* care of splitting the data into successive pbuf payloads, if necessary.
*
* The function returns the newly created pbuf or NULL if the pbuf cannot
* be allocated.
*/
struct pbuf *pbuf_new(u8_t *data, u16_t len) {
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    struct pbuf *q = p;
    while ((q != (struct pbuf *)0) && (len >= q->len)) {
        memcpy(q->payload, data, q->len);         /* copy data into payload */
        len  -= q->len;                                 /* remaining length */
        data += q->len;                              /* remainig data chunk */
        q = q->next;                                       /* get next pbuf */
    }
    return p;
}

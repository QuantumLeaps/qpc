/*****************************************************************************
* Product: lwIP-Manager Active Object
* Last updated for version 6.9.1
* Last updated on  2020-09-11
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#define LWIP_ALLOWED

#include "qpc.h"   /* QP/C API */
#include "dpp.h"   /* application events and active objects */
#include "bsp.h"   /* Board Support Package */

#include "lwip.h"  /* lwIP stack */
#include "httpd.h" /* lwIP application */

#include <stdio.h>  /* for snprintf() */
#include <string.h> /* for strlen() */

Q_DEFINE_THIS_FILE

      /* the LwIP driver signal group must fit the actual number of signals */
Q_ASSERT_COMPILE((LWIP_DRIVER_END - LWIP_DRIVER_GROUP) >= LWIP_MAX_OFFSET);

#define FLASH_USERREG0          (*(uint32_t const *)0x400FE1E0)
#define FLASH_USERREG1          (*(uint32_t const *)0x400FE1E4)
#define LWIP_SLOW_TICK_MS       TCP_TMR_INTERVAL

/* Active object class -----------------------------------------------------*/
typedef struct LwIPMgrTag {
    QActive super;  /* inherit QActive */

    QTimeEvt te_LWIP_SLOW_TICK;
    struct netif   *netif;
    struct udp_pcb *upcb;
    uint32_t        ip_addr; /* IP address in the native host byte order */

#if LWIP_TCP
    uint32_t tcp_tmr;
#endif
#if LWIP_ARP
    uint32_t arp_tmr;
#endif
#if LWIP_DHCP
    uint32_t dhcp_fine_tmr;
    uint32_t dhcp_coarse_tmr;
#endif
#if LWIP_AUTOIP
    uint32_t auto_ip_tmr;
#endif
} LwIPMgr;

static QState LwIPMgr_initial(LwIPMgr *me, QEvt const *e);
static QState LwIPMgr_running(LwIPMgr *me, QEvt const *e);

/* Local objects -----------------------------------------------------------*/
static LwIPMgr l_lwIPMgr; /* the single instance of LwIPMgr AO */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_LwIPMgr = (QActive *)&l_lwIPMgr; /* "opaque" pointer */

/* Server-Side Include (SSI) demo ..........................................*/
static char const * const ssi_tags[] = {
    "s_xmit",
    "s_recv",
    "s_fw",
    "s_drop",
    "s_chkerr",
    "s_lenerr",
    "s_memerr",
    "s_rterr",
    "s_proerr",
    "s_opterr",
    "s_err",
};
static int ssi_handler(int iIndex, char *pcInsert, int iInsertLen);

/* Common Gateway Iinterface (CG) demo .....................................*/
static char const *cgi_display(int index, int numParams,
                               char const *param[],
                               char const *value[]);
static tCGI const cgi_handlers[] = {
    { "/display.cgi", &cgi_display },
};

/* UDP handler .............................................................*/
static void udp_rx_handler(void *arg, struct udp_pcb *upcb,
                           struct pbuf *p, struct ip_addr *addr, u16_t port);
/*..........................................................................*/
void LwIPMgr_ctor(void) {
    LwIPMgr *me = &l_lwIPMgr;

    QActive_ctor(&me->super, (QStateHandler)&LwIPMgr_initial);
    QTimeEvt_ctorX(&me->te_LWIP_SLOW_TICK, &me->super,
                   LWIP_SLOW_TICK_SIG, 0U);
}
/*..........................................................................*/
QState LwIPMgr_initial(LwIPMgr *me, QEvt const *e) {
    uint32_t user0, user1;
    uint8_t  macaddr[NETIF_MAX_HWADDR_LEN];

    (void)e;        /* suppress the compiler warning about unused parameter */

    /* Configure the hardware MAC address for the Ethernet Controller
    *
    * For the Stellaris Eval Kits, the MAC address will be stored in the
    * non-volatile USER0 and USER1 registers.  These registers can be read
    * using the FlashUserGet function, as illustrated below.
    */
    user0 = FLASH_USERREG0;
    user1 = FLASH_USERREG1;

                              /* the MAC address must have been programmed! */
    Q_ASSERT((user0 != 0xFFFFFFFF) && (user1 != 0xFFFFFFFF));

    /*
    * Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
    * address needed to program the hardware registers, then program the MAC
    * address into the Ethernet Controller registers.
    */
    macaddr[0] = (uint8_t)user0; user0 >>= 8;
    macaddr[1] = (uint8_t)user0; user0 >>= 8;
    macaddr[2] = (uint8_t)user0; user0 >>= 8;
    macaddr[3] = (uint8_t)user1; user1 >>= 8;
    macaddr[4] = (uint8_t)user1; user1 >>= 8;
    macaddr[5] = (uint8_t)user1;

                                          /* initialize the Ethernet Driver */
    me->netif = eth_driver_init((QActive *)me, LWIP_DRIVER_GROUP, macaddr);

    me->ip_addr = 0xFFFFFFFF;             /* initialize to impossible value */

                                     /* initialize the lwIP applications... */
    httpd_init();         /* initialize the simple HTTP-Deamon (web server) */
    http_set_ssi_handler(&ssi_handler, ssi_tags, Q_DIM(ssi_tags));
    http_set_cgi_handlers(cgi_handlers, Q_DIM(cgi_handlers));

    me->upcb = udp_new();
    udp_bind(me->upcb, IP_ADDR_ANY, 777);           /* use port 777 for UDP */
    udp_recv(me->upcb, &udp_rx_handler, me);

    QS_OBJ_DICTIONARY(&l_lwIPMgr);
    QS_OBJ_DICTIONARY(&l_lwIPMgr.te_LWIP_SLOW_TICK);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&LwIPMgr_initial);
    QS_FUN_DICTIONARY(&LwIPMgr_running);

    QS_SIG_DICTIONARY(SEND_UDP_SIG,       me);
    QS_SIG_DICTIONARY(LWIP_SLOW_TICK_SIG, me);
    QS_SIG_DICTIONARY(LWIP_DRIVER_GROUP + LWIP_RX_READY_OFFSET,   me);
    QS_SIG_DICTIONARY(LWIP_DRIVER_GROUP + LWIP_TX_READY_OFFSET,   me);
    QS_SIG_DICTIONARY(LWIP_DRIVER_GROUP + LWIP_RX_OVERRUN_OFFSET, me);

    return Q_TRAN(&LwIPMgr_running);
}
/*..........................................................................*/
QState LwIPMgr_running(LwIPMgr *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->te_LWIP_SLOW_TICK,
                          (LWIP_SLOW_TICK_MS * BSP_TICKS_PER_SEC) / 1000U,
                          (LWIP_SLOW_TICK_MS * BSP_TICKS_PER_SEC) / 1000U);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->te_LWIP_SLOW_TICK);
            return Q_HANDLED();
        }

        case SEND_UDP_SIG: {
            if (me->upcb->remote_port != (uint16_t)0) {
                struct pbuf *p = pbuf_new((u8_t *)((TextEvt const *)e)->text,
                                      strlen(((TextEvt const *)e)->text) + 1);
                if (p != (struct pbuf *)0) {
                    udp_send(me->upcb, p);
                    pbuf_free(p);                   /* don't leak the pbuf! */
                }
            }
            return Q_HANDLED();
        }

        case LWIP_DRIVER_GROUP + LWIP_RX_READY_OFFSET: {
            eth_driver_read();
            return Q_HANDLED();
        }
        case LWIP_DRIVER_GROUP + LWIP_TX_READY_OFFSET: {
            eth_driver_write();
            return Q_HANDLED();
        }
        case LWIP_SLOW_TICK_SIG: {
                                                 /* has IP address changed? */
            if (me->ip_addr != me->netif->ip_addr.addr) {
                TextEvt *te;
                uint32_t ip_net;    /* IP address in the network byte order */

                me->ip_addr = me->netif->ip_addr.addr; /* save the IP addr. */
                ip_net  = ntohl(me->ip_addr);
                    /* publish the text event to display the new IP address */
                te = Q_NEW(TextEvt, DISPLAY_IPADDR_SIG);
                snprintf(te->text, sizeof(te->text), "%d.%d.%d.%d",
                          (int)(((ip_net) >> 24) & 0xFFU),
                          (int)(((ip_net) >> 16) & 0xFFU),
                          (int)(((ip_net) >> 8)  & 0xFFU),
                          (int)(ip_net           & 0xFFU));
                QF_PUBLISH((QEvt *)te, me);
            }

#if LWIP_TCP
            me->tcp_tmr += LWIP_SLOW_TICK_MS;
            if (me->tcp_tmr >= TCP_TMR_INTERVAL) {
                me->tcp_tmr = 0;
                tcp_tmr();
            }
#endif
#if LWIP_ARP
            me->arp_tmr += LWIP_SLOW_TICK_MS;
            if (me->arp_tmr >= ARP_TMR_INTERVAL) {
                me->arp_tmr = 0;
                etharp_tmr();
            }
#endif
#if LWIP_DHCP
            me->dhcp_fine_tmr += LWIP_SLOW_TICK_MS;
            if (me->dhcp_fine_tmr >= DHCP_FINE_TIMER_MSECS) {
                me->dhcp_fine_tmr = 0;
                dhcp_fine_tmr();
            }
            me->dhcp_coarse_tmr += LWIP_SLOW_TICK_MS;
            if (me->dhcp_coarse_tmr >= DHCP_COARSE_TIMER_MSECS) {
                me->dhcp_coarse_tmr = 0;
                dhcp_coarse_tmr();
            }
#endif
#if LWIP_AUTOIP
            me->auto_ip_tmr += LWIP_SLOW_TICK_MS;
            if (me->auto_ip_tmr >= AUTOIP_TMR_INTERVAL) {
                me->auto_ip_tmr = 0;
                autoip_tmr();
            }
#endif
            return Q_HANDLED();
        }
        case LWIP_DRIVER_GROUP + LWIP_RX_OVERRUN_OFFSET: {
            LINK_STATS_INC(link.err);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

/* HTTPD customizations ----------------------------------------------------*/

/* Server-Side Include (SSI) handler .......................................*/
static int ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
    struct stats_proto *stats = &lwip_stats.link;
    STAT_COUNTER value = 0;

    switch (iIndex) {
        case 0: /* s_xmit   */
            value = stats->xmit;
            break;
        case 1: /* s_recv   */
            value = stats->recv;
            break;
        case 2: /* s_fw     */
            value = stats->fw;
            break;
        case 3: /* s_drop   */
            value = stats->drop;
            break;
        case 4: /* s_chkerr */
            value = stats->chkerr;
            break;
        case 5: /* s_lenerr */
            value = stats->lenerr;
            break;
        case 6: /* s_memerr */
            value = stats->memerr;
            break;
        case 7: /* s_rterr  */
            value = stats->rterr;
            break;
        case 8: /* s_proerr */
            value = stats->proterr;
            break;
        case 9: /* s_opterr */
            value = stats->opterr;
            break;
        case 10: /* s_err    */
            value = stats->err;
            break;
    }

    return snprintf(pcInsert, MAX_TAG_INSERT_LEN, "%d", value);
}

/* Common Gateway Iinterface (CG) handler ..................................*/
static char const *cgi_display(int index, int numParams,
                               char const *param[],
                               char const *value[])
{
    int i;
    for (i = 0; i < numParams; ++i) {
        if (strstr(param[i], "text") != (char *)0) { /* param text found? */
            TextEvt *te = Q_NEW(TextEvt, DISPLAY_CGI_SIG);
            strncpy(te->text, value[i], Q_DIM(te->text));
            QF_PUBLISH((QEvt *)te, AO_LwIPMgr);
            return "/thank_you.htm";
        }
    }
    return (char *)0;/*no URI, HTTPD will send 404 error page to the browser*/
}

/* UDP receive handler -----------------------------------------------------*/
static void udp_rx_handler(void *arg, struct udp_pcb *upcb,
                           struct pbuf *p, struct ip_addr *addr, u16_t port)
{
    TextEvt *te = Q_NEW(TextEvt, DISPLAY_UDP_SIG);
    strncpy(te->text, (char *)p->payload, Q_DIM(te->text));
    QF_PUBLISH((QEvt *)te, AO_LwIPMgr);

    udp_connect(upcb, addr, port); /* connect to the remote host */
    pbuf_free(p); /* don't leak the pbuf! */
}


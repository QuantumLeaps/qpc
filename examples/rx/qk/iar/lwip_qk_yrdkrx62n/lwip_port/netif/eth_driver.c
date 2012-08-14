/*
 * lwIP Ethernet driver for Renesas RX MCUs
 * Copyright (c) 2012 Quantum Leaps, LLC, www.state-machine.com
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 */

#include "qp_port.h"

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"

#include "netif/etharp.h"
#include "netif/eth_driver.h"

#include "iorx62n.h"
#include "phy.h"

#include <string.h>                                         /* for memcpy() */

Q_DEFINE_THIS_FILE

/* Sanity Check: This Ethernet driver will NOT work if the following defines
* are incorrect.
*/
#if (MEM_ALIGNMENT != 32)
#error "MEM_ALIGNMENT must be 32 for this Ethernet driver!"
#endif

/**
* EDMAC (Ethernet DMA Controller) descriptor structure
* for transmit/receive descriptiors
*/
typedef struct EDMAC_DescTag {
    __evenaccess uint32_t status;

#if __LITTLE_ENDIAN__
    __evenaccess uint16_t size;
    __evenaccess uint16_t bufsize;
#else      /* Big endian */
    __evenaccess uint16_t bufsize;
    __evenaccess uint16_t size;
#endif

    uint8_t     *bufaddr;
    struct pbuf *pbuf;
} EDMAC_Desc;

/* Helper queue of EDMAC descriptors */
typedef struct QueueTag {
    uint8_t head;               /* index for adding new descriptors (empty) */
    uint8_t tail;               /* index for removing old desriptors (full) */
    uint8_t is_full;                                  /* is the queue full? */
    uint16_t n_err;     /* number of errors (overflows in TX, errors in RX) */
} Queue;

#define NEXT(i_, e_)              (((i_) < ((e_) - 1U)) ? ((i_) + 1) : 0U)

#define EDMAC_TX_END_INTERRUPT    (1U << 21)
#define EDMAC_RX_END_INTERRUPT    (1U << 18)

/* EDMAC Descriptor status bits */
#define ACT_STAT  0x80000000U
#define DL_STAT   0x40000000U
#define FP1_STAT  0x20000000U
#define FP0_STAT  0x10000000U
#define FE_STAT   0x08000000U

/**
 * Setup processing for PTP (IEEE-1588).
 */
#if LWIP_PTPD
void lwIPHostGetTime(u32_t *time_s, u32_t *time_ns);
#endif

/*..........................................................................*/
#pragma data_alignment=16
static EDMAC_Desc l_txDesc[4];                  /* TX EDMAC descriptor ring */
#pragma data_alignment=16
static EDMAC_Desc l_rxDesc[4];                 /* RX  EDMAC descriptor ring */

static Queue l_rxQueue;                    /* queue of RX descriptors/pbufs */
static Queue l_txQueue;                    /* queue of TX descriptors/pbufs */

static struct netif l_netif;                /* the single network interface */
static QActive *l_active;      /* active object associated with this driver */

static err_t ethernetif_init(struct netif *netif);
static err_t ethernetif_output(struct netif *netif, struct pbuf *p);

static struct pbuf *low_level_receive(void);
static err_t low_level_transmit(struct pbuf *p);

#ifdef Q_SPY
    static uint8_t l_Ethernet_IRQHandler;
#endif


/*..........................................................................*/
#pragma vector=VECT_ETHER_EINT
static __interrupt void EDMAC_isr(void) {
    uint32_t eth_stat;

#ifdef QK_ISR_ENTRY                           /* preemptive QK kernel used? */
    QK_ISR_ENTRY();                      /* inform QK about entering an ISR */
#endif

    eth_stat = EDMAC.EESR.LONG;

    if ((eth_stat & EDMAC_RX_END_INTERRUPT) != 0U) {
        static QEvt const evt_eth_rx = { LWIP_RX_READY_SIG, 0U, 0U };
        QACTIVE_POST(l_active, &evt_eth_rx, &l_Ethernet_IRQHandler);
        EDMAC.EESR.LONG = EDMAC_RX_END_INTERRUPT;
        //EDMAC.EESIPR.LONG &= ~EDMAC_RX_END_INTERRUPT; /* disable further RX */
    }
    /*
    if ((eth_stat & EDMAC_TX_END_INTERRUPT) != 0U) {
        static QEvt const evt_eth_tx = { LWIP_TX_READY_SIG, 0U, 0U };
        QACTIVE_POST(l_active, &evt_eth_tx, &l_Ethernet_IRQHandler);
        EDMAC.EESR.LONG = EDMAC_TX_END_INTERRUPT;
    }
    */

#ifdef QK_ISR_EXIT                            /* preemptive QK kernel used? */
    QK_ISR_EXIT();                        /* inform QK about exiting an ISR */
#endif
}

/*..........................................................................*/
struct netif *eth_driver_init(QActive *active,
                              u8_t macaddr[NETIF_MAX_HWADDR_LEN])
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    lwip_init();                                /* nitialize the lwIP stack */

                             /* set MAC address in the network interface... */
    l_netif.hwaddr_len = NETIF_MAX_HWADDR_LEN;
    memcpy(&l_netif.hwaddr[0], macaddr, NETIF_MAX_HWADDR_LEN);

    l_active = active; /*save the active object associated with this driver */

#if LWIP_NETIF_HOSTNAME
    l_netif.hostname = "lwIP";             /* initialize interface hostname */
#endif
    l_netif.name[0] = 'Q';
    l_netif.name[1] = 'P';

    /*
    * Initialize the snmp variables and counters inside the struct netif.
    * The last argument should be replaced with your link speed, in units
    * of bits per second.
    */
    NETIF_INIT_SNMP(&l_netif, snmp_ifType_ethernet_csmacd, 1000000);

    /* We directly use etharp_output() here to save a function call.
    * You can instead declare your own function an call etharp_output()
    * from it if you have to do some checks before sending (e.g. if link
    * is available...) */
    l_netif.output     = &etharp_output;

    l_netif.linkoutput = &ethernetif_output;

#if (LWIP_DHCP == 0) && (LWIP_AUTOIP == 0)
          /* No mechanism of obtaining IP address specified, use static IP: */
    IP4_ADDR(&ipaddr,  STATIC_IPADDR0,    STATIC_IPADDR1,
                       STATIC_IPADDR2,    STATIC_IPADDR3);
    IP4_ADDR(&netmask, STATIC_NET_MASK0,  STATIC_NET_MASK1,
                       STATIC_NET_MASK2,  STATIC_NET_MASK3);
    IP4_ADDR(&gwaddr,  STATIC_GW_IPADDR0, STATIC_GW_IPADDR1,
                       STATIC_GW_IPADDR2, STATIC_GW_IPADDR3);
#else
     /* either DHCP or AUTOIP are configured, start with zero IP addresses: */
    IP4_ADDR(&ipaddr,  0, 0, 0, 0);
    IP4_ADDR(&netmask, 0, 0, 0, 0);
    IP4_ADDR(&gw,      0, 0, 0, 0);
#endif
          /* add and configure the Ethernet interface with default settings */
    netif_add(&l_netif,
              &ipaddr, &netmask, &gw,            /* configured IP addresses */
              active,                /* use this active object as the state */
              &ethernetif_init,        /* Ethernet interface initialization */
              &ip_input);                   /* standard IP input processing */

    netif_set_default(&l_netif);

    netif_set_up(&l_netif);                       /* bring the interface up */

    QS_OBJ_DICTIONARY(&l_Ethernet_IRQHandler);

    return &l_netif;
}
/*..........................................................................*/
void eth_driver_read(void) {
    struct pbuf *p;
    if ((p = low_level_receive()) != NULL) {
        if (ethernet_input(p, &l_netif) != ERR_OK) {   /* pbuf not handled? */
            LWIP_DEBUGF(NETIF_DEBUG, ("eth_driver_input: input error\n"));
            pbuf_free(p);              /* free the pbuf, lwIP did not do it */
        }
    }

    //???ETH->IM |= ETH_INT_RX;                    /* re-enable the RX interrupt */
}
/*..........................................................................*/
void eth_driver_write(void) {
}

/*..........................................................................*/
/*
 * This function outputs a pbuf 'p' to the hardware.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the pbuf to send
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 */
static err_t ethernetif_output(struct netif *netif, struct pbuf *p) {
    Q_REQUIRE(p != NULL);
    (void)netif;       /* to avoid compiler complain about unused parameter */
    return low_level_transmit(p);
}

/*==========================================================================*/

static err_t ethernetif_init(struct netif *netif) {
    uint32_t i;
    uint32_t volatile dly;
    EDMAC_Desc *desc;

    //SYSTEM.MSTPCRB.BIT.MSTPB15 = 0;          /* enable the EtherC and EDMAC */
    //for (dly = 100000U; dly > 0; --dly) {                    /* crude delay */
    //}

    /* Ethernet settings */
    IOPORT.PFENET.BYTE = 0x82U;      /* enable Ethernet, RMII mode, LINKSTA */
    PORTA.ICR.BIT.B5 = 1U;                                   /* ET_LINKSTA  */
    PORTA.ICR.BIT.B3 = 1U;                                   /* ET_MDIO     */
    PORTB.ICR.BIT.B0 = 1U;                                   /* RMII_RXD1   */
    PORTB.ICR.BIT.B1 = 1U;                                   /* RMII_RXD0   */
    PORTB.ICR.BIT.B2 = 1U;                                   /* REF50CLK    */
    PORTB.ICR.BIT.B3 = 1U;                                   /* RMII_RX_ER  */
    PORTB.ICR.BIT.B7 = 1U;                                   /* RMII_CRS_DV */


    for (i = 0U; i < Q_DIM(l_rxDesc); ++i) {   /* initialize RX descriptors */
        desc = &l_rxDesc[i];

        desc->pbuf = pbuf_alloc(PBUF_RAW,
                                LWIP_MEM_ALIGN_SIZE(PBUF_POOL_BUFSIZE),
                                PBUF_POOL);
        Q_ASSERT(desc->pbuf != NULL);

        desc->size    = 0U;
        desc->bufsize = desc->pbuf->len;
        desc->bufaddr = desc->pbuf->payload;
        desc->status  = ACT_STAT;
    }
    desc->status |= DL_STAT;   /* set the last descriptor to close the ring */
    l_rxQueue.head    = 0U;
    l_rxQueue.tail    = 0U;
    l_rxQueue.is_full = 0U;
    l_rxQueue.n_err   = 0U;

    for (i = 0U; i < Q_DIM(l_txDesc); ++i) {   /* initialize TX descriptors */
        desc = &l_txDesc[i];

        desc->pbuf    = NULL;
        desc->size    = 0U;
        desc->bufsize = 0U;
        desc->bufaddr = (void *)0;
        desc->status  = 0U;
    }
    desc->status |= DL_STAT;   /* set the last descriptor to close the ring */
    l_txQueue.head    = 0U;
    l_txQueue.tail    = 0U;
    l_txQueue.is_full = 0U;
    l_txQueue.n_err   = 0U;

    EDMAC.EDMR.BIT.SWR = 1U;                             /* reset the EDMAC */
    for (dly = 100000U; dly > 0; --dly) {                    /* crude delay */
    }

    ETHERC.MAHR      = (  (uint32_t)netif->hwaddr[0] << 24)
                       | ((uint32_t)netif->hwaddr[1] << 16)
                       | ((uint32_t)netif->hwaddr[2] << 8)
                       |  (uint32_t)netif->hwaddr[3];
    ETHERC.MALR.BIT.MA = ((uint32_t)netif->hwaddr[4] << 8)
                       |  (uint32_t)netif->hwaddr[5];

    netif->mtu   = 1518U;               /* maximum transfer unit 1500 + CRC */
    netif->flags = (NETIF_FLAG_BROADCAST         /* set device capabilities */
                    | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP);

    /* ETHERC */
    ETHERC.ECSR.LONG   = 0x00000037U;  /* clear BFR, PSRTO, LCHNG, MPD, ICD */
    ETHERC.RFLR.LONG   = netif->mtu;       /* ethernet payload is 1500+ CRC */
    ETHERC.IPGR.LONG   = 0x00000014U;            /* Intergap is 96-bit time */
    ETHERC.ECMR.BIT.PRM = 0x00000000U;   /* ensure promiscuous mode is off. */

    /* EDMAC */
    EDMAC.EESR.LONG   = 0x47FF0F9FU;  /* clear ETHERC and EDMAC status bits */
#if __LITTLE_ENDIAN__
    EDMAC.EDMR.BIT.DE = 1U;                      /* little endian operation */
#endif
    EDMAC.RDLAR       = &l_rxDesc[0];     /* the Rx Descriptor List Address */
    EDMAC.TDLAR       = &l_txDesc[0];     /* the Tx Descriptor List Address */
    EDMAC.TRSCER.LONG = 0x00000000U;  /* copy-back status is RFE & TFE only */
    EDMAC.TFTR.LONG   = 0x00000000U;    /* TX FIFO (store and forward mode) */
    //EDMAC.FDR.LONG    = 0x00000000U;         /* TX & RX FIFOs are 256 bytes */
    EDMAC.FDR.LONG    = 0x00000707U;        /* TX & RX FIFOs are 2048 bytes */
    EDMAC.RMCR.LONG   = 0x00000003U;  /* receive is normal mode (continued) */
    EDMAC.EESIPR.LONG = EDMAC_RX_END_INTERRUPT;

    if (phy_init() != R_PHY_OK) {                     /* initialize the PHY */
        return ERR_IF;              /* return low-level netif error to lwIP */
    }

    /* NOTE: the configuration continues in eth_driver_connect()
    *  AFTER the PHY setup.
    */

    return ERR_OK;
}
/*..........................................................................*/
uint8_t eth_driver_connect(void) {
    uint8_t success = 1U;                                 /* assume success */
    switch (phy_set_autonegotiate()) {
        case PHY_LINK_100H:                /* half duplex link, 100 Mbits/s */
            ETHERC.ECMR.BIT.DM  = 0U;
            ETHERC.ECMR.BIT.RTM = 1U;
            break;
        case PHY_LINK_10H:                  /* half duplex link, 10 Mbits/s */
            ETHERC.ECMR.BIT.DM  = 0U;
            ETHERC.ECMR.BIT.RTM = 0U;
            break;
        case PHY_LINK_100F:                /* full duplex link, 100 Mbits/s */
            ETHERC.ECMR.BIT.DM  = 1U;
            ETHERC.ECMR.BIT.RTM = 1U;
            break;
        case PHY_LINK_10F:                  /* full duplex link, 10 Mbits/s */
            ETHERC.ECMR.BIT.DM  = 1U;
            ETHERC.ECMR.BIT.RTM = 0U;
            break;
        default:
            success = 0U;                 /* negotiations are still ongoing */
            break;
    }

    if (success) {
        ETHERC.ECMR.BIT.RE = 1U;                               /* enable RX */
        ETHERC.ECMR.BIT.TE = 1U;                               /* enable TX */
        EDMAC.EDRRR.LONG   = 1U;                         /* enable EDMAC RX */
        IEN(ETHER, EINT)   = 1U;        /* enable interrupt source (11.2.2) */
    }

    return success;
}

/*..........................................................................*/
/*
* This function first frees all sent pbufs and next it enqueus the provided
* pbuf for transmission.
*
* NOTE: the pbuf is assumed to be NOT chained.
*
* @param p the MAC packet to send (e.g. IP packet including MAC addr and type)
* @return ERR_OK if the packet could be sent
*         an err_t value if the packet couldn't be sent
*/
static err_t low_level_transmit(struct pbuf *p) {
    uint8_t i;
    EDMAC_Desc *desc;
    err_t ret = ERR_OK;                                   /* assume success */

    Q_REQUIRE((p != NULL)
              && (p->next == NULL));      /* only non-chained pbufs for now */

              /* remove all the sent pbufs from the tail of the TX queue... */
    for (i = l_txQueue.tail;
         (i != l_txQueue.head) || l_txQueue.is_full;
         i = NEXT(i, Q_DIM(l_txDesc)))
    {
        desc = &l_txDesc[i];
        if ((desc->status & ACT_STAT) != 0U) {  /* descriptor still active? */
            break;
        }
        else {  /* descriptor no longer active, which means that is is sent */
            pbuf_free(desc->pbuf);             /* free the transmitted pbuf */
            desc->pbuf = NULL;
            l_txQueue.is_full = 0U;   /* the queue certainly no longer full */
        }
    }
    l_txQueue.tail = i;                           /* advance the tail index */

                         /* add the new pbuf to the head of the TX queue... */
    if (!l_txQueue.is_full) {                     /* is the queue NOT full? */
        i = l_txQueue.head;                         /* store the head index */
        l_txQueue.head = NEXT(i, Q_DIM(l_txDesc));/* advance the head index */
        l_txQueue.is_full = (l_txQueue.head == l_txQueue.tail);

        desc = &l_txDesc[i];
        Q_ASSERT(((desc->status & ACT_STAT) == 0U)
                   && (desc->pbuf == NULL));

        desc->status &= DL_STAT;      /* clear the status except the DL bit */
        desc->bufsize = p->len;
        desc->bufaddr = p->payload;
        desc->pbuf    = p;
        desc->status |= (FP1_STAT | FP0_STAT | ACT_STAT);
    }
    else {
        pbuf_free(p);                                /* don't leak the pbuf */

        ++l_txQueue.n_err;           /* count the number of overflow errors */
        if (l_txQueue.n_err > 100U) {          /* too many overflow errors? */
            l_txQueue.head      = 0U;
            l_txQueue.tail      = 0U;
            l_txQueue.n_err     = 0U;
            l_txQueue.is_full   = 0U;
            l_txDesc[0].status &= DL_STAT;              /* clear the status */
        }

        ret = ERR_BUF;    /* signal a buffer problem (not critical) to lwIP */
    }

    EDMAC.EDTRR.LONG   = 1U;                            /* restart EDMAC TX */

    return ret;
}

/*..........................................................................*/
/*
 * This function reads a single packet from the Ethernet interface.
 *
 * @return pointer to pbuf packet if available, NULL otherswise.
 */
static struct pbuf *low_level_receive(void) {
    struct pbuf *p   = NULL;
    EDMAC_Desc *desc = &l_rxDesc[l_rxQueue.tail];

    if ((desc->status & ACT_STAT) != 0U) {
        /* packet not ready yet */
    }
    else {
        if ((desc->status & FE_STAT) != 0U) {
            LINK_STATS_INC(link.memerr);
            LINK_STATS_INC(link.drop);
            ++l_rxQueue.n_err;                     /* account for the error */
        }
        else {
            if ((desc->status & FP0_STAT) == 0U) {
                return NULL;
            }

            p = desc->pbuf;
            p->len     = desc->size;

            Q_ASSERT(p->len < p->tot_len);
            p->tot_len = p->len;

                                 /* allocate new buffer for this descriptor */
            desc->pbuf = pbuf_alloc(PBUF_RAW,
                                    LWIP_MEM_ALIGN_SIZE(PBUF_POOL_BUFSIZE),
                                    PBUF_POOL);
            Q_ASSERT(desc->pbuf != NULL);


#if LWIP_PTPD
            lwIPHostGetTime(&p->time_s, &p->time_ns);  /* add the timestamp */
#endif
        }
                                                  /* advance the tail index */
        l_rxQueue.tail = NEXT(l_rxQueue.tail, Q_DIM(l_rxDesc));

        desc->size    = 0U;
        desc->bufaddr = desc->pbuf->payload;
        desc->bufsize = desc->pbuf->len;

        if (desc == &l_rxDesc[Q_DIM(l_rxDesc) - 1U]) {     /* the last one? */
            desc->status = DL_STAT | ACT_STAT;
        }
        else {
            desc->status = ACT_STAT;
        }

        EDMAC.EDRRR.LONG = 1U;                          /* restart EDMAC RX */
    }

    return p;
}
/*..........................................................................*/
u32_t sys_now(void) {
    return 0U; //???
}

/*..........................................................................*/
#if NETIF_DEBUG
/* Print an IP header by using LWIP_DEBUGF
 * @param p an IP packet, p->payload pointing to the IP header
 */
void eth_driver_debug_print(struct pbuf *p) {
    struct eth_hdr *ethhdr = (struct eth_hdr *)p->payload;
    u16_t *plen = (u16_t *)p->payload;

    LWIP_DEBUGF(NETIF_DEBUG, ("ETH header:\n"));
    LWIP_DEBUGF(NETIF_DEBUG, ("Packet Length:%5"U16_F" \n",*plen));
    LWIP_DEBUGF(NETIF_DEBUG, ("Destination: %02"X8_F"-%02"X8_F"-%02"X8_F
        "-%02"X8_F"-%02"X8_F"-%02"X8_F"\n",
        ethhdr->dest.addr[0],
        ethhdr->dest.addr[1],
        ethhdr->dest.addr[2],
        ethhdr->dest.addr[3],
        ethhdr->dest.addr[4],
        ethhdr->dest.addr[5]));
    LWIP_DEBUGF(NETIF_DEBUG, ("Source: %02"X8_F"-%02"X8_F"-%02"X8_F
        "-%02"X8_F"-%02"X8_F"-%02"X8_F"\n",
        ethhdr->src.addr[0],
        ethhdr->src.addr[1],
        ethhdr->src.addr[2],
        ethhdr->src.addr[3],
        ethhdr->src.addr[4],
        ethhdr->src.addr[5]));
    LWIP_DEBUGF(NETIF_DEBUG, ("Packet Type:0x%04"U16_F" \n", ethhdr->type));
}
#endif /* NETIF_DEBUG */

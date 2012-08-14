/*****************************************************************************
* Product: lwIP consolidated file for QP-lwIP integration
* Last Updated for Version: lwIPv1.3.0 / QPv4.0.03
* Date of the Last Update:  Mar 16, 2009
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
#ifndef lwip_h
#define lwip_h

               /* protection against including lwIP in unauthorized modules */
#ifndef LWIP_ALLOWED
#error "The lwIP code is not reentrant and is not allowed in this module."
#endif

#include "lwip/opt.h"                            /* lwIP options come first */
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/mem.h"
#include "lwip/autoip.h"
#include "lwip/dhcp.h"

#include "netif/etharp.h"
#include "netif/eth_driver.h"
                                                /* utilities added by QL... */
#ifdef __cplusplus
extern "C" {
#endif

/**
* Allocate a transport-layer pbuf and copies the provided data buffer 'data'
* of length 'len' bytes into the payload(s) of the pbuf. The function takes
* care of splitting the data into successive pbuf payloads, if necessary.
*
* The function returns the newly created pbuf or NULL if the pbuf cannot
* be allocated.
*/
struct pbuf *pbuf_new(u8_t *data, u16_t len);

#ifdef __cplusplus
}
#endif

#endif                                                            /* lwip_h */

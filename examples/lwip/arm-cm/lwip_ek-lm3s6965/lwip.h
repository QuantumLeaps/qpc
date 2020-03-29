/*****************************************************************************
* Product: lwIP consolidated file for QP-lwIP integration
* Last Updated for Version: LwIP 1.4.1/QP 5.1.1
* Date of the Last Update:  Oct 21, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          <www.state-machine.com/licensing>
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef LWIP_H
#define LWIP_H

               /* protection against including lwIP in unauthorized modules */
#ifndef LWIP_ALLOWED
#error "The lwIP code is not reentrant and is not allowed in this module."
#endif

#include "lwip/opt.h"                            /* lwIP options come first */
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
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

#endif /* LWIP_H */

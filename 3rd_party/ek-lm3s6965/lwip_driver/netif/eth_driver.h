/*****************************************************************************
* Product: Ethernet driver for lwIP with QP
* Last Updated for Version: 5.1.1
* Date of the Last Update:  Oct 20, 2013
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          https://state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef eth_driver_h
#define eth_driver_h

struct netif *eth_driver_init(QActive *active,
                              enum_t base_sig,
                              u8_t macaddr[NETIF_MAX_HWADDR_LEN]);
void eth_driver_read(void);
void eth_driver_write(void);

enum EthDriverSignals {
    LWIP_RX_READY_OFFSET,
    LWIP_TX_READY_OFFSET,
    LWIP_RX_OVERRUN_OFFSET,
    LWIP_MAX_OFFSET
};

#endif                                                      /* eth_driver_h */

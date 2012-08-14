/*****************************************************************************
* Product: FreeRTOS example
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Jul 01, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2007 Quantum Leaps, LLC. All rights reserved.
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
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "freertos_task.h"

portTASK_FUNCTION(vTaskFunction, pvParameters) {
     for (;;) {
         BSP_led_toggle(15); /* toggle LED 15 */
         vTaskDelay(BSP_TICKS_PER_SEC/2);         /* wait for half a second */
     }
}
/*****************************************************************************
* Product: "QHsmTst" example
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Mar 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#include "qp_port.h"
#include "qhsmtst.h"
#include "bsp.h"

#include "lm3s_cmsis.h"
#include <yfuns.h>               /* for terminal I/O __read() and __write() */
#include <string.h>                                         /* for strlen() */

/*Q_DEFINE_THIS_FILE*/

/* Local-scope objects -----------------------------------------------------*/

#define PUSH_BUTTON             (1 << 4)
#define USER_LED                (1 << 5)

/*..........................................................................*/
void BSP_init(void) {
    /* set the system clock as specified in lm3s_config.h (20MHz from PLL)  */
    SystemInit();

    /* enable clock to the peripherals used by the application */
    SYSCTL->RCGC2 |= (1 <<  0) | (1 <<  2);   /* enable clock to GPIOA & C  */
    __NOP();                                  /* wait after enabling clocks */
    __NOP();
    __NOP();

    /* configure the LED and push button */
    GPIOC->DIR |= USER_LED;                        /* set direction: output */
    GPIOC->DEN |= USER_LED;                               /* digital enable */
    GPIOC->DATA_Bits[USER_LED] = 0;                /* turn the User LED off */

    GPIOC->DIR &= ~PUSH_BUTTON;                    /*  set direction: input */
    GPIOC->DEN |= PUSH_BUTTON;                            /* digital enable */
}
/*..........................................................................*/
void BSP_exit(void) {
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    __write(1, (uint8_t const *)msg, strlen(msg));
}
/*..........................................................................*/
void BSP_LED_on(void) {
    GPIOC->DATA_Bits[USER_LED] = USER_LED;         /* turn the User LED on  */
}
/*..........................................................................*/
void BSP_LED_off(void) {
    GPIOC->DATA_Bits[USER_LED] = 0;                /* turn the User LED off */
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}

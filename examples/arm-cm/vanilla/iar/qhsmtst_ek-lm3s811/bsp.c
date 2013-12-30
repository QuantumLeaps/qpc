/*****************************************************************************
* Product: "QHsmTst" example
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 17, 2013
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
*                          http://www.state-machine.com
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

#define PUSH_BUTTON             (1U << 4)
#define USER_LED                (1U << 5)

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
void Q_onAssert(char const Q_ROM * const file, int_t line) {
    assert_failed(file, line);
}
/*..........................................................................*/
/* error routine that is called if the CMSIS library encounters an error    */
void assert_failed(char const *file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    NVIC_SystemReset();                             /* perform system reset */
}

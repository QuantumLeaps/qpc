/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-08-24
* @version Last updated for: Zephyr 3.1.99 and @ref qpc_7_1_0
*
* @file
* @brief BSP for Zephyr, Blinky example
*/
#include "qpc.h"
#include "bsp.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/reboot.h>
/* other drivers, if necessary ... */

Q_DEFINE_THIS_FILE

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

static struct gpio_dt_spec const l_led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static struct k_timer QF_tick_timer;
static void QF_tick_function(struct k_timer *tid) {
    (void)tid; /* unused parameter */
    QTIMEEVT_TICK_X(0U, (void *)0);
}

void BSP_init(void) {
    int ret = gpio_pin_configure_dt(&l_led0, GPIO_OUTPUT_ACTIVE);
    Q_ASSERT(ret >= 0);

    k_timer_init(&QF_tick_timer, &QF_tick_function, NULL);
}

void BSP_ledOn(void) {
    printk("BSP_ledOn\n");
    gpio_pin_set_dt(&l_led0, true);
}

void BSP_ledOff(void) {
    printk("BSP_ledOff\n");
    gpio_pin_set_dt(&l_led0, false);
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    k_timer_start(&QF_tick_timer, K_MSEC(1), K_MSEC(1));
    printk("QF_onStartup\n");
}
/*..........................................................................*/
void QF_onCleanup(void) {
    printk("QF_onCleanup\n");
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    printk("\nASSERTION in %s:%d\n", module, loc);
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
#ifndef NDEBUG
    k_panic(); /* debug build: halt the system for error search... */
#else
    sys_reboot(SYS_REBOOT_COLD); /* release build: reboot the system */
#endif
}


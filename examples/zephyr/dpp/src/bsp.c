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
* @brief BSP for Zephyr, DPP example
*/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/reboot.h>
/* other drivers, if necessary ... */

Q_DEFINE_THIS_FILE

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/* Local-scope objects -----------------------------------------------------*/
static struct gpio_dt_spec const l_led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static struct k_timer QF_tick_timer;
static uint32_t l_rnd; /* random seed */

#ifdef Q_SPY

    /* QSpy source IDs */
    static QSpyId const timerID = { 0U };

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
        COMMAND_STAT,
        TEST_MSG
    };

#endif

/*..........................................................................*/
static void QF_tick_function(struct k_timer *tid) {
    (void)tid; /* unused parameter */
    QTIMEEVT_TICK_X(0U, &timerID);
}
/*..........................................................................*/
void BSP_init(void) {
    int ret = gpio_pin_configure_dt(&l_led0, GPIO_OUTPUT_ACTIVE);
    Q_ASSERT(ret >= 0);

    k_timer_init(&QF_tick_timer, &QF_tick_function, NULL);

    //...
    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }

    /* object dictionaries... */
    QS_OBJ_DICTIONARY(AO_Table);
    for (int n = 0; n < N_PHILO; ++n) {
        QS_OBJ_ARR_DICTIONARY(AO_Philo[n], n);
    }

    QS_OBJ_DICTIONARY(&timerID);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);
    QS_USR_DICTIONARY(COMMAND_STAT);
    QS_USR_DICTIONARY(TEST_MSG);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS); /* state machine records */
    QS_GLB_FILTER(QS_AO_RECORDS); /* active object records */
    QS_GLB_FILTER(QS_UA_RECORDS); /* all user records */
    QS_GLB_FILTER(TEST_MSG);
}
/*..........................................................................*/
void BSP_ledOn(void) {
    gpio_pin_set_dt(&l_led0, true);
}
/*..........................................................................*/
void BSP_ledOff(void) {
    gpio_pin_set_dt(&l_led0, false);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'e') {
        BSP_ledOn();
    }
    else {
        BSP_ledOff();
    }
    printk("Philo[%d]->%s\n", n, stat);

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()          /* application-specific record end */
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused != 0U) {
        //BSP_LED_On(LED2);
    }
    else {
        //BSP_LED_Off(LED2);
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;

    /* Some flating point code is to exercise the VFP... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */

    return (rnd >> 8);
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    k_timer_start(&QF_tick_timer, K_MSEC(1), K_MSEC(1));
    Q_PRINTK("QF_onStartup\n");
}
/*..........................................................................*/
void QF_onCleanup(void) {
    Q_PRINTK("QF_onCleanup\n");
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

/* QS callbacks ============================================================*/
#ifdef Q_SPY

#include <zephyr/drivers/uart.h>

/* select the Zephyr shell UART
* NOTE: you can change this to other UART peripheral if desired
*/
static struct device const *uart_dev =
     DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));

/*..........................................................................*/
static void uart_cb(const struct device *dev, void *user_data) {
    if (!uart_irq_update(uart_dev)) {
        return;
    }

    if (uart_irq_rx_ready(uart_dev)) {
        uint8_t buf[32];
        int n = uart_fifo_read(uart_dev, buf, sizeof(buf));
        for (uint8_t const *p = buf; n > 0; --n, ++p) {
            QS_RX_PUT(*p);
        }
    }
}
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsTxBuf[1024]; /* buffer for QS-TX channel */
    static uint8_t qsRxBuf[256];  /* buffer for QS-RX channel */

    Q_REQUIRE(uart_dev != (struct device *)0);

    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // configure interrupt and callback to receive data
    uart_irq_callback_user_data_set(uart_dev, &uart_cb, (void *)0);
    uart_irq_rx_enable(uart_dev);

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {  /* NOTE: invoked with interrupts DISABLED */
    return k_cycle_get_32();
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t len = 0xFFFFU; /* to get as many bytes as available */
    uint8_t const *buf;
    while ((buf = QS_getBlock(&len)) != (uint8_t*)0) {
        for (; len != 0U; --len, ++buf) {
            uart_poll_out(uart_dev, *buf);
        }
        len = 0xFFFFU; /* to get as many bytes as available */
    }
}
/*..........................................................................*/
void QS_doOutput(void) {
    uint16_t len = 0xFFFFU; /* big number to get all available bytes */

    QS_CRIT_STAT_
    QS_CRIT_E_();
    uint8_t const *buf = QS_getBlock(&len);
    QS_CRIT_X_();

    /* transmit the bytes via the UART... */
    for (; len != 0U; --len, ++buf) {
        uart_poll_out(uart_dev, *buf);
    }
}
/*..........................................................................*/
/*! callback function to reset the target (to be implemented in the BSP) */
void QS_onReset(void) {
    sys_reboot(SYS_REBOOT_COLD);
}
/*..........................................................................*/
/*! callback function to execute a user command (to be implemented in BSP) */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif /* Q_SPY */

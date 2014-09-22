/*****************************************************************************
* Product: "Dining Philosophers Problem" example, embOS
* Last updated for version 5.3.0
* Last updated on  2014-06-27
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#include "stm32f4xx.h"

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
static unsigned  l_rnd; /* random seed */


#define GPIOD_BASE_ADDR   0x40020C00U

#define GPIOD_MODER       (*(uint32_t volatile *)(GPIOD_BASE_ADDR + 0x00U))
#define GPIOD_ODR         (*(uint32_t volatile *)(GPIOD_BASE_ADDR + 0x14U))
#define GPIOD_BSRR        (*(uint32_t volatile *)(GPIOD_BASE_ADDR + 0x18U))

#define RCC_BASE_ADDR     0x40023800U
#define RCC_AHB1RSTR      (*(uint32_t volatile *)(RCC_BASE_ADDR + 0x10U))
#define RCC_AHBENR        (*(uint32_t volatile *)(RCC_BASE_ADDR + 0x30U))

#define RCC_LEDPORT_RSTR  RCC_AHB1RSTR
#define RCC_LEDPORT_ENR   RCC_AHBENR
#define RCC_LEDPORT_BIT   3U

#define LED_PORT_MODER    GPIOD_MODER
#define LED_PORT_ODR      GPIOD_ODR
#define LED_PORT_BSRR     GPIOD_BSRR
#define LED0_BIT          12U
#define LED1_BIT          13U
#define LED2_BIT          14U
#define LED3_BIT          15U

void assert_failed(char const *file, int line);

/*..........................................................................*/
static void tick_handler(void) {  /* signature of embOS tick hook routine */
    static uint_fast8_t ctr = 1U;
    if (--ctr == 0U) {
        ctr = 1000U/BSP_TICKS_PER_SEC;
        QF_tickX_(0U);
    }
}

/*..........................................................................*/
void BSP_init(void) {

    /* Initialize thr port for LEDs */
    RCC_LEDPORT_ENR  &= ~(1U << RCC_LEDPORT_BIT);
    RCC_LEDPORT_RSTR &= ~(1U << RCC_LEDPORT_BIT);
    RCC_LEDPORT_ENR  |=  (1U << RCC_LEDPORT_BIT);

    LED_PORT_MODER &= ~(3U << (LED0_BIT * 2U)) | (3U << (LED1_BIT * 2U))
                       | (3U << (LED2_BIT * 2U)) | (3U << (LED3_BIT * 2U));
    /* set LED pins to output */
    LED_PORT_MODER |=  (1U << (LED0_BIT * 2U)) | (1U << (LED1_BIT * 2U))
                       | (1U << (LED2_BIT * 2)) | (1U << (LED3_BIT * 2));
    /* turn all LEDs off */
    LED_PORT_BSRR   =  (0x10000U << LED0_BIT) | (0x10000U << LED1_BIT)
                       | (0x10000U << LED2_BIT) | (0x10000U << LED3_BIT);

    BSP_randomSeed(1234U);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    Q_REQUIRE(n < N_PHILO);

    static uint32_t const led_bit[] = {
        LED0_BIT, /* green  LED */
        LED1_BIT, /* orange LED */
        LED2_BIT, /* red    LED */
        LED3_BIT, /* blue   LED */
        LED0_BIT  /* for the 5th Philo */
    };
    if (stat[0] == 'e') {
        LED_PORT_BSRR = (1U << led_bit[n]);       /* switch on LED */
    }
    else {
        LED_PORT_BSRR = (0x10000U << led_bit[n]); /* switch off LED */
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                   /* Philosopher number */
        QS_STR(stat);                  /* Philosopher status */
    QS_END()                           /* application-specific record end */
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused) {
        /* switch all LEDs */
        LED_PORT_BSRR = (1U << LED0_BIT) | (1U << LED1_BIT)
                        | (1U << LED2_BIT) | (1U << LED3_BIT);
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * (3U*7U*11U*13U*23U);

    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}

/*..........................................................................*/
void QF_onStartup(void) {
    static OS_TICK_HOOK tick_hook;
    OS_TICK_AddHook(&tick_hook, &tick_handler);
}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int_t line) {
    assert_failed(file, line);
}
/*..........................................................................*/
/* error routine that is called if the CMSIS library encounters an error    */
void assert_failed(char const *file, int line) {
    (void)file;        /* avoid compiler warning */
    (void)line;        /* avoid compiler warning */
    OS_INTERRUPT_MaskGlobal(); /* make sure that all interrupts are disabled */
    for (;;) {}  /* loop forever */
}


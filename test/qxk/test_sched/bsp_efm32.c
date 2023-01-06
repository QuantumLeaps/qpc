/*============================================================================
* Product: BSP for system-testing QXK kernel, EFM32-SLSTK3401A board
* Last updated for version 7.2.0
* Last updated on  2022-12-13
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
* <www.state-machine.com/licensing>
* <info@state-machine.com>
============================================================================*/
#include "qpc.h"
#include "bsp.h"

#include "em_device.h"  /* the device specific header (SiLabs) */
#include "em_cmu.h"     /* Clock Management Unit (SiLabs) */
#include "em_gpio.h"    /* GPIO (SiLabs) */
#include "em_usart.h"   /* USART (SiLabs) */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);
void GPIO_EVEN_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
#define LED_PORT    gpioPortF
#define LED0_PIN    4
#define LED1_PIN    5

#define PB_PORT     gpioPortF
#define PB0_PIN     6
#define PB1_PIN     7

#ifdef Q_SPY

    /* QSpy source IDs */
    static QSpyId const l_SysTick_Handler = { 100U };
    static QSpyId const l_test_ISR = { 101U };

    enum AppRecords { /* application-specific trace records */
        CONTEXT_SW = QS_USER1,
        TRACE_MSG
    };

#endif

/*..........................................................................*/
void SysTick_Handler(void); /* prototype */
void SysTick_Handler(void) {
    QXK_ISR_ENTRY(); /* inform QXK kernel about entering an ISR */

    /* process time events for rate 0 */
    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler);
    //QACTIVE_POST(the_Ticker0, 0, &l_SysTick_Handler);

    QXK_ISR_EXIT();  /* inform QXK kernel about exiting an ISR */
}
/*..........................................................................*/
void GPIO_EVEN_IRQHandler(void); /* prototype */
void GPIO_EVEN_IRQHandler(void) { /* for testing, NOTE03 */
    QXK_ISR_ENTRY(); /* inform QXK kernel about entering an ISR */

    /* for testing... */
    static QEvt const t1 = { TEST1_SIG, 0U, 0U };
    QACTIVE_PUBLISH(&t1, &l_test_ISR);

    QXK_ISR_EXIT();  /* inform QXK kernel about exiting an ISR */
}

/*..........................................................................*/
/* MPU setup for EFM32PG1B200F256GM48 MCU */
static void EFM32PG182_MPU_setup(void) {
    /* The following MPU configuration contains the general EFM32PG1 memory
    * map described in the EFM32PG1 Data Sheet Figure 3.2. EFM32PG1 Memory Map
    *
    * Please note that the actual STM32 MCUs provide much less Flash and SRAM
    * than the maximums configured here. This means that actual MCUs have
    * unmapped memory regions (e.g., beyond the actual SRAM). Attempts to
    * access these regions causes the HardFault exception, which is the
    * desired behavior.
    */
    static struct {
        uint32_t rbar;
        uint32_t rasr;
    } const mpu_setup[] = {

        { /* region #0: Flash: base=0x0000'0000, size=512M=2^(28+1) */
          0x00000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 0U), /* region #0 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(18+1) region */
              | (0x6U << MPU_RASR_AP_Pos)   /* PA:ro/UA:ro */
              | (1U << MPU_RASR_C_Pos)      /* C=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #1: SRAM: base=0x2000'0000, size=512M=2^(28+1) */
          0x20000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 1U), /* region #1 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_C_Pos)      /* C=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        /* region #3: (not configured) */
        { MPU_RBAR_VALID_Msk | (MPU_RBAR_REGION_Msk & 2U), 0U },

        { /* region #3: Peripherals: base=0x4000'0000, size=512M=2^(28+1) */
          0x40000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 3U), /* region #3 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #4: Priv. Periph: base=0xE000'0000, size=512M=2^(28+1) */
          0xE0000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 4U), /* region #4 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #5: Ext RAM: base=0x6000'0000, size=1G=2^(29+1) */
          0x60000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 5U), /* region #5 */
          (29U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #6: Ext Dev: base=0xA000'0000, size=1G=2^(29+1) */
          0xA0000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 6U), /* region #6 */
          (29U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #7: NULL-pointer: base=0x000'0000, size=256B, NOTE0 */
          0x00000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 7U), /* region #7 */
          (7U << MPU_RASR_SIZE_Pos)         /* 2^(7+1)=256B region */
              | (0x0U << MPU_RASR_AP_Pos)   /* PA:na/UA:na */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },
    };

    /* enable the MemManage_Handler for MPU exception */
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    __DSB();
    MPU->CTRL = 0U; /* disable the MPU */
    for (uint_fast8_t n = 0U; n < Q_DIM(mpu_setup); ++n) {
        MPU->RBAR = mpu_setup[n].rbar;
        MPU->RASR = mpu_setup[n].rasr;
    }
    MPU->CTRL = MPU_CTRL_ENABLE_Msk;        /* enable the MPU */
    __ISB();
    __DSB();
}

/*..........................................................................*/
void BSP_init(void) {
    /* setup the MPU... */
    EFM32PG182_MPU_setup();

    /* NOTE: SystemInit() has been already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* NOTE: The VFP (hardware Floating Point) unit is configured by QK */

    /* enable clock for to the peripherals used by this application... */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO,  true);
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO,  true);

    /* configure the LEDs */
    GPIO_PinModeSet(LED_PORT, LED0_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(LED_PORT, LED1_PIN, gpioModePushPull, 0);
    GPIO_PinOutClear(LED_PORT, LED0_PIN);
    GPIO_PinOutClear(LED_PORT, LED1_PIN);

    /* configure the Buttons */
    GPIO_PinModeSet(PB_PORT, PB0_PIN, gpioModeInputPull, 1);
    GPIO_PinModeSet(PB_PORT, PB1_PIN, gpioModeInputPull, 1);

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_test_ISR);

    QS_USR_DICTIONARY(CONTEXT_SW);
    QS_USR_DICTIONARY(TRACE_MSG);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    Q_UNUSED_PAR(result);
}
/*..........................................................................*/
void BSP_ledOn(void) {
    GPIO->P[LED_PORT].DOUT |=  (1U << LED0_PIN);
}
/*..........................................................................*/
void BSP_ledOff(void) {
    GPIO->P[LED_PORT].DOUT &= ~(1U << LED0_PIN);
}
/*..........................................................................*/
void BSP_trigISR(void) {
    NVIC_SetPendingIRQ(GPIO_EVEN_IRQn);
}
/*..........................................................................*/
void BSP_trace(QActive const *thr, char const *msg) {
    QS_BEGIN(TRACE_MSG, 0U)
        QS_OBJ(thr);
        QS_STR(msg);
    QS_END()
}
/*..........................................................................*/
uint32_t BSP_romRead(int32_t offset, uint32_t fromEnd) {
    int32_t const rom_base = (fromEnd == 0U)
                             ? 0x100 /* don't include the NULL-ptr region */
                             : 0x40000 - 4;
    return *(uint32_t volatile *)(rom_base + offset);
}
/*..........................................................................*/
void BSP_romWrite(int32_t offset, uint32_t fromEnd, uint32_t value) {
    int32_t const rom_base = (fromEnd == 0U)
                             ? 0x100 /* don't include the NULL-ptr region */
                             : 0x40000 - 4;
    *(uint32_t volatile *)(rom_base + offset) = value;
}

/*..........................................................................*/
uint32_t BSP_ramRead(int32_t offset, uint32_t fromEnd) {
    int32_t const ram_base = (fromEnd == 0U)
                             ? 0x20000000
                             : 0x20008000 - 4;
    return *(uint32_t volatile *)(ram_base + offset);
}
/*..........................................................................*/
void BSP_ramWrite(int32_t offset, uint32_t fromEnd, uint32_t value) {
    int32_t const ram_base = (fromEnd == 0U)
                             ? 0x20000000
                             : 0x20008000 - 4;
    *(uint32_t volatile *)(ram_base + offset) = value;
}

/*..........................................................................*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    //SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assign all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system */
    NVIC_SetPriority(GPIO_EVEN_IRQn, QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* NOTE: priority of UART IRQ used for QS-RX is set in qutest_port.c */
    /* ... */

    /* enable IRQs... */
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
#ifdef QF_ON_CONTEXT_SW
/* NOTE: the context-switch callback is called with interrupts DISABLED */
void QF_onContextSw(QActive *prev, QActive *next) {
    QS_BEGIN_NOCRIT(CONTEXT_SW, 0U) /* no critical section! */
        QS_OBJ(prev);
        QS_OBJ(next);
    QS_END_NOCRIT()
}
#endif /* QF_ON_CONTEXT_SW */
/*..........................................................................*/
void QXK_onIdle(void) {

    GPIO->P[LED_PORT].DOUT |=  (1U << LED1_PIN);
    GPIO->P[LED_PORT].DOUT &= ~(1U << LED1_PIN);

#ifdef Q_SPY
    QS_rxParse();  /* parse all the received bytes */
    QS_doOutput();
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
    */
    __WFI(); /* Wait-For-Interrupt */
#endif
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY

/*..........................................................................*/
void QTimeEvt_tick1_(
    uint_fast8_t const tickRate,
    void const * const sender)
{
    QF_INT_DISABLE();
    /* TODO pend the SysTick */
    *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 26U);
    QF_INT_ENABLE();
}

#endif /* Q_SPY */

/*============================================================================
* NOTE0:
* The MPU protection against NULL-pointer dereferencing sets up a no-access
* MPU region #7 around the NULL address (0x0). This works even though the
* Vector Table also resides at address 0x0. However, the *size* of the
* no-access region should not exceed the size of the Vector Table. In this
* case, the size is set to 2**(7+1)==256 bytes, which does not contain any
* data that the CPU would legitimately read with the LDR instruction.
*/

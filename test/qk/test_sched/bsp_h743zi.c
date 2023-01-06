/*============================================================================
* Product: BSP for system-testing of QK kernel, NUCLEO-H743ZI board
* Last updated for version 7.2.0
* Last updated on  2022-12-15
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

/* STM32CubeH7 include files */
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

#ifdef Q_SPY

    /* QSpy source IDs */
    static QSpyId const l_SysTick_Handler = { 100U };
    static QSpyId const l_test_ISR = { 101U };

    enum AppRecords { /* application-specific trace records */
        CONTEXT_SW = QS_USER1,
        TRACE_MSG
    };

#endif

/* ISRs used in this project ===============================================*/
void SysTick_Handler(void); /* prototype */
void SysTick_Handler(void) {
    QK_ISR_ENTRY(); /* inform QXK kernel about entering an ISR */

    /* process time events for rate 0 */
    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler);

    QK_ISR_EXIT();  /* inform QXK kernel about exiting an ISR */
}
/*..........................................................................*/
void EXTI0_IRQHandler(void); /* prototype */
void EXTI0_IRQHandler(void) { /* for testing, NOTE03 */
    QK_ISR_ENTRY(); /* inform QXK kernel about entering an ISR */

    /* for testing... */
    static QEvt const t1 = { TEST1_SIG, 0U, 0U };
    QACTIVE_PUBLISH(&t1, &l_test_ISR);

    QK_ISR_EXIT();  /* inform QXK kernel about exiting an ISR */
}

/* BSP functions ===========================================================*/
/*..........................................................................*/
/* MPU setup for STM32H743ZI MCU */
static void STM32H743ZI_MPU_setup(void) {
    /* The following MPU configuration contains just a generic ROM
    * region (with read-only access) and NULL-pointer protection region.
    * Otherwise, the MPU will fall back on the background region (PRIVDEFENA).
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

        { /* region #7: NULL-pointer: base=0x000'0000, size=128M=2^(26+1) */
          /* NOTE: this region extends to  0x080'0000, which is where
          * the ROM is re-mapped by STM32
          */
          0x00000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 7U), /* region #7 */
          (26U << MPU_RASR_SIZE_Pos)        /* 2^(26+1)=128M region */
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
    MPU->CTRL = MPU_CTRL_ENABLE_Msk         /* enable the MPU */
                | MPU_CTRL_PRIVDEFENA_Msk;  /* enable background region */
    __ISB();
    __DSB();
}

/*..........................................................................*/
void BSP_init(void) {
    /* setup the MPU... */
    STM32H743ZI_MPU_setup();

    /* NOTE: SystemInit() has been already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    SCB_EnableICache(); /* Enable I-Cache */
    SCB_EnableDCache(); /* Enable D-Cache */

    /* Configure Flash prefetch and Instr. cache through ART accelerator */
#if (ART_ACCLERATOR_ENABLE != 0)
    __HAL_FLASH_ART_ENABLE();
#endif /* ART_ACCLERATOR_ENABLE */

    /* Configure the LEDs */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Configure the User Button in GPIO Mode */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }

    /* dictionaries... */
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
    BSP_LED_On(LED1);
}
/*..........................................................................*/
void BSP_ledOff(void) {
    BSP_LED_Off(LED1);
}
/*..........................................................................*/
void BSP_trigISR(void) {
    NVIC_SetPendingIRQ(EXTI0_IRQn);
}
/*..........................................................................*/
void BSP_trace(QActive const *thr, char const *msg) {
    QS_BEGIN_ID(TRACE_MSG, 0U)
        QS_OBJ(thr);
        QS_STR(msg);
    QS_END()
}
/*..........................................................................*/
uint32_t BSP_romRead(int32_t offset, uint32_t fromEnd) {
    int32_t const rom_base = (fromEnd == 0U)
                             ? 0x08000000
                             : 0x08200000 - 4;
    return *(uint32_t volatile *)(rom_base + offset);
}
/*..........................................................................*/
void BSP_romWrite(int32_t offset, uint32_t fromEnd, uint32_t value) {
    int32_t const rom_base = (fromEnd == 0U)
                             ? 0x08000000
                             : 0x08200000 - 4;
    *(uint32_t volatile *)(rom_base + offset) = value;
}

/*..........................................................................*/
uint32_t BSP_ramRead(int32_t offset, uint32_t fromEnd) {
    int32_t const ram_base = (fromEnd == 0U)
                             ? 0x20000000
                             : 0x20020000 - 4;
    return *(uint32_t volatile *)(ram_base + offset);
}
/*..........................................................................*/
void BSP_ramWrite(int32_t offset, uint32_t fromEnd, uint32_t value) {
    int32_t const ram_base = (fromEnd == 0U)
                             ? 0x20000000
                             : 0x20020000 - 4;
    *(uint32_t volatile *)(ram_base + offset) = value;
}

/*..........................................................................*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    //NOTE: don't start ticking for these tests
    //SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assign all priority bits for preemption-prio. and none to sub-prio.
    * NOTE: this might have been changed by STM32Cube.
    */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE1
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(SysTick_IRQn, QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(EXTI0_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs... */
    NVIC_EnableIRQ(EXTI0_IRQn);
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
void QK_onIdle(void) {
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
* MPU region #7 around the NULL address (0x0). The size of this region is set
* to 2**(26+1)==0x0800'0000, because that is the address of Flash in STM32.
*
* REMARK: STM32 MCUs automatically relocate the Flash memory and the Vector
* Table in it to address 0x0800'0000 at startup. However, even though the
* region 0..0x0800'0000 is un-mapped after the relocation, the read access
* is still allowed and causes no CPU exception. Therefore setting up the MPU
* to protect that region is necessary.
*/

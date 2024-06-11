//============================================================================
// Product: BSP for system-testing of QK kernel, NUCLEO-U545RE-Q board
// Last updated for version 7.4.0
// Last updated on  2024-06-24
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. <state-machine.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "bsp.h"

#include "stm32u545xx.h"  // CMSIS-compliant header file for the MCU used
// add other drivers if necessary...

Q_DEFINE_THIS_FILE  // define the name of this file for assertions

// Local-scope defines -----------------------------------------------------
// LED pins available on the board (just one user LED LD2--Green on PA.5)
#define LD2_PIN  5U

// Button pins available on the board (just one user Button B1 on PC.13)
#define B1_PIN   13U

// macros from STM32Cube LL:
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) \
    WRITE_REG((REG), ((READ_REG(REG) & (~(CLEARMASK))) | (SETMASK)))

#ifdef Q_SPY
// QSpy source IDs
static QSpyId const l_SysTick_Handler = { 100U };
static QSpyId const l_test_ISR = { 101U };

enum AppRecords { // application-specific trace records
    CONTEXT_SW = QS_USER1,
    TRACE_MSG
};

#endif

// ISRs used in this project =================================================
void SysTick_Handler(void); // prototype
void SysTick_Handler(void) {
    QK_ISR_ENTRY(); // inform QK kernel about entering an ISR

    // process time events for rate 0
    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler);

    QK_ISR_EXIT();  // inform QK kernel about exiting an ISR
}
//..........................................................................
void EXTI0_IRQHandler(void); // prototype
void EXTI0_IRQHandler(void) { // for testing, NOTE03
    QK_ISR_ENTRY(); // inform QXK kernel about entering an ISR

    // for testing...
    static QEvt const t1 = QEVT_INITIALIZER(TEST1_SIG);
    QACTIVE_PUBLISH(&t1, &l_test_ISR);

    QK_ISR_EXIT();  // inform QK kernel about exiting an ISR
}

// BSP functions ===========================================================
static void STM32U545RE_MPU_setup(void) {
    MPU->CTRL = 0U;  // disable the MPU

    MPU->RNR = 0U; // region 0 (for ROM: read-only, can-execute)
    MPU->RBAR = (0x08000000U & MPU_RBAR_BASE_Msk)  | (0x3U << MPU_RBAR_AP_Pos);
    MPU->RLAR = (0x08080000U & MPU_RLAR_LIMIT_Msk) | MPU_RLAR_EN_Msk;

    MPU->RNR = 1U; // region 1 (for RAM1: read-write, execute-never)
    MPU->RBAR = (0x20000000U & MPU_RBAR_BASE_Msk)  | (0x1U << MPU_RBAR_AP_Pos)
                | MPU_RBAR_XN_Msk;
    MPU->RLAR = (0x20040000U & MPU_RLAR_LIMIT_Msk) | MPU_RLAR_EN_Msk;

    MPU->RNR = 2U; // region 2 (for RAM2: read-write, execute-never)
    MPU->RBAR = (0x28000000U & MPU_RBAR_BASE_Msk)  | (0x1U << MPU_RBAR_AP_Pos)
                | MPU_RBAR_XN_Msk;
    MPU->RLAR = (0x28004000U & MPU_RLAR_LIMIT_Msk) | MPU_RLAR_EN_Msk;

    MPU->RNR  = 7U; // region 7 (no access: for NULL pointer protection)
    MPU->RBAR = (0x00000000U & MPU_RBAR_BASE_Msk)  | MPU_RBAR_XN_Msk;
    MPU->RLAR = (0x00080000U & MPU_RLAR_LIMIT_Msk) | MPU_RLAR_EN_Msk;
    __DMB();

    MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;

    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    __DSB();
    __ISB();
}
//..........................................................................
void BSP_init(void) {
    // setup the MPU...
    STM32U545RE_MPU_setup();

    // initialize I-CACHE
    MODIFY_REG(ICACHE->CR, ICACHE_CR_WAYSEL, 0U); // 1-way
    SET_BIT(ICACHE->CR, ICACHE_CR_EN); // enable

    // flash prefetch buffer enable
    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);

    // enable PWR clock interface
    SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_PWREN);

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    // enable GPIOA clock port for the LED LD4
    RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOAEN;

    // set all used GPIOA pins as push-pull output, no pull-up, pull-down
    MODIFY_REG(GPIOA->OSPEEDR,
               GPIO_OSPEEDR_OSPEED0 << (LD2_PIN * GPIO_OSPEEDR_OSPEED1_Pos),
               1U << (LD2_PIN * GPIO_OSPEEDR_OSPEED1_Pos)); // speed==1
    MODIFY_REG(GPIOA->OTYPER,
               1U << LD2_PIN,
               0U << LD2_PIN); // output
    MODIFY_REG(GPIOA->PUPDR,
               GPIO_PUPDR_PUPD0 << (LD2_PIN * GPIO_PUPDR_PUPD1_Pos),
               0U << (LD2_PIN * GPIO_PUPDR_PUPD1_Pos)); // PUSHPULL
     MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODE0 << (LD2_PIN * GPIO_MODER_MODE1_Pos),
               1U << (LD2_PIN * GPIO_MODER_MODE1_Pos)); // MODE_1

    // enable GPIOC clock port for the Button B1
    RCC->AHB2ENR1 |=  RCC_AHB2ENR1_GPIOCEN;

    // configure Button B1 pin on GPIOC as input, no pull-up, pull-down
    MODIFY_REG(GPIOC->PUPDR,
               GPIO_PUPDR_PUPD0 << (B1_PIN * GPIO_PUPDR_PUPD1_Pos),
               0U << (B1_PIN * GPIO_PUPDR_PUPD1_Pos)); // NO PULL
    MODIFY_REG(GPIOC->MODER,
               GPIO_MODER_MODE0 << (B1_PIN * GPIO_MODER_MODE1_Pos),
               0U << (B1_PIN * GPIO_MODER_MODE1_Pos)); // MODE_0

    // initialize the QS software tracing...
    if (!QS_INIT((void *)0)) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_test_ISR);

    QS_USR_DICTIONARY(CONTEXT_SW);
    QS_USR_DICTIONARY(TRACE_MSG);
}
//..........................................................................
void BSP_terminate(int16_t result) {
    Q_UNUSED_PAR(result);
}
//............................................................................
void BSP_ledOn(void) {
    GPIOA->BSRR = (1U << LD2_PIN);  // turn LED on
}
//............................................................................
void BSP_ledOff(void) {
    GPIOA->BRR = (1U << LD2_PIN);  // turn LED off
}
//..........................................................................
void BSP_trigISR(void) {
    NVIC_SetPendingIRQ(EXTI0_IRQn);
}
//..........................................................................
void BSP_trace(QActive const *thr, char const *msg) {
    QS_BEGIN_ID(TRACE_MSG, 0U)
        QS_OBJ(thr);
        QS_STR(msg);
    QS_END()
}
//..........................................................................
uint32_t BSP_romRead(int32_t offset, uint32_t fromEnd) {
    int32_t const rom_base = (fromEnd == 0U)
                             ? 0x08000000
                             : 0x08080000 - 4;
    return *(uint32_t volatile *)(rom_base + offset);
}
//..........................................................................
void BSP_romWrite(int32_t offset, uint32_t fromEnd, uint32_t value) {
    int32_t const rom_base = (fromEnd == 0U)
                             ? 0x08000000
                             : 0x08080000 - 4;
    *(uint32_t volatile *)(rom_base + offset) = value;
}

//..........................................................................
uint32_t BSP_ramRead(int32_t offset, uint32_t fromEnd) {
    int32_t const ram_base = (fromEnd == 0U)
                             ? 0x20000000
                             : 0x20040000 - 4;
    return *(uint32_t volatile *)(ram_base + offset);
}
//..........................................................................
void BSP_ramWrite(int32_t offset, uint32_t fromEnd, uint32_t value) {
    int32_t const ram_base = (fromEnd == 0U)
                             ? 0x20000000
                             : 0x20040000 - 4;
    *(uint32_t volatile *)(ram_base + offset) = value;
}

//============================================================================
// QF callbacks...
void QF_onStartup(void) {
    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    //NOTE: don't start ticking for these tests
    //SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    // assign all priority bits for preemption-prio. and none to sub-prio.
    // NOTE: this might have been changed by STM32Cube.
    NVIC_SetPriorityGrouping(0U);

    // set priorities of ALL ISRs used in the system
    NVIC_SetPriority(SysTick_IRQn, QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(EXTI0_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    // NOTE: priority of UART IRQ used for QS-RX is set in qutest_port.c
    // ...

    // enable IRQs...
    NVIC_EnableIRQ(EXTI0_IRQn);
}
//............................................................................
void QF_onCleanup(void) {
}
//..........................................................................
#ifdef QF_ON_CONTEXT_SW
// NOTE: the context-switch callback is called with interrupts DISABLED
void QF_onContextSw(QActive *prev, QActive *next) {
    QS_BEGIN_INCRIT(CONTEXT_SW, 0U) // in critical section!
        QS_OBJ(prev);
        QS_OBJ(next);
    QS_END_INCRIT()
}
#endif // QF_ON_CONTEXT_SW

//............................................................................
void QK_onIdle(void) {
#ifdef Q_SPY
    QS_rxParse();  // parse all the received bytes
    QS_doOutput();
#elif defined NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M3 MCU.
    //
    __WFI(); // Wait-For-Interrupt
#endif
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY

//............................................................................
void QTimeEvt_tick1_(
    uint_fast8_t const tickRate,
    void const * const sender)
{
    QF_INT_DISABLE();
    // TODO pend the SysTick
    *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 26U);
    QF_INT_ENABLE();
}

#endif // Q_SPY
//----------------------------------------------------------------------------

//============================================================================
// NOTE0:
// The MPU protection against NULL-pointer dereferencing sets up a no-access
// MPU region #7 around the NULL address (0x0). The size of this region is set
// to 2^(26+1)==0x0800'0000, because that is the address of Flash in STM32.
//
// REMARK: STM32 MCUs automatically relocate the Flash memory and the Vector
// Table in it to address 0x0800'0000 at startup. However, even though the
// region 0..0x0800'0000 is un-mapped after the relocation, the read access
// is still allowed and causes no CPU exception. Therefore setting up the MPU
// to protect that region is necessary.
//

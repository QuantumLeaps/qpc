//============================================================================
// QP/C Example
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"           // QP/C API
#include "bsp.h"           // Board Support Package interface
#include "blinky_button.h" // application shared interface

#include "stm32l0xx.h"  // CMSIS-compliant header file for the MCU used
// add other drivers if necessary...

Q_DEFINE_THIS_MODULE("bsp_nucleo-l053r8") // for DBC assertions

// Local-scope defines -----------------------------------------------------

// test pins on GPIO PA
#define TST1_PIN  7U
#define TST2_PIN  6U
#define TST3_PIN  4U
#define TST4_PIN  1U
#define TST5_PIN  0U
#define TST6_PIN  5U // LED LD2-Green

// buttons on GPIO PC
#define B1_PIN    13U

#ifdef Q_SPY
    // QSpy source IDs
    static QSpyId const l_SysTick_Handler = { 100U };

#endif

// ISRs used in the application ============================================
void SysTick_Handler(void);  // prototype

void SysTick_Handler(void) { // system clock tick ISR
    BSP_d1on();

    QK_ISR_ENTRY();   // inform QK about entering an ISR


    // process time events for rate 0
    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler);

    // Perform the debouncing of buttons. The algorithm for debouncing
    // adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    // and Michael Barr, page 71.
    //
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };
    uint32_t current = ~GPIOC->IDR; // read GPIO PortC
    uint32_t tmp = buttons.depressed; // save the debounced depressed
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    if ((tmp & (1U << B1_PIN)) != 0U) { // debounced B1 state changed?
        if ((buttons.depressed & (1U << B1_PIN)) != 0U) { // depressed?
            // immutable button-press event
            static ButtonWorkEvt const pressEvt = {
                QEVT_INITIALIZER(BUTTON_PRESSED_SIG),
                .toggles = 60U
            };
            // immutable forward-press event
            static ButtonWorkEvt const fPressEvt = {
                QEVT_INITIALIZER(FORWARD_PRESSED_SIG),
                .toggles = 60U
            };
            QACTIVE_POST(AO_Button2a, &fPressEvt.super, &l_SysTick_Handler);
            QACTIVE_POST(AO_Button2a, &pressEvt.super,  &l_SysTick_Handler);
        }
        else { // B1 is released
            // immutable button-release event
            static ButtonWorkEvt const releaseEvt = {
                QEVT_INITIALIZER(BUTTON_RELEASED_SIG),
                .toggles = 80U
            };
            // immutable forward-release event
            static ButtonWorkEvt const fReleaseEvt = {
                QEVT_INITIALIZER(FORWARD_RELEASED_SIG),
                .toggles = 80U
            };
            QACTIVE_POST(AO_Button2a, &fReleaseEvt.super, &l_SysTick_Handler);
            QACTIVE_POST(AO_Button2a, &releaseEvt.super,  &l_SysTick_Handler);
        }
    }

    QK_ISR_EXIT();             // inform QK about exiting an ISR

    BSP_d1off();
}

// BSP functions ===========================================================
void BSP_init(void) {
    // Configure the MPU to prevent NULL-pointer dereferencing
    // see: www.state-machine.com/null-pointer-protection-with-arm-cortex-m-mpu
    //
    MPU->RBAR = 0x0U                          // base address (NULL)
                | MPU_RBAR_VALID_Msk          // valid region
                | (MPU_RBAR_REGION_Msk & 7U); // region #7
    MPU->RASR = (7U << MPU_RASR_SIZE_Pos)     // 2^(7+1) region
                | (0x0U << MPU_RASR_AP_Pos)   // no-access region
                | MPU_RASR_ENABLE_Msk;        // region enable

    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk       // enable background region
                | MPU_CTRL_ENABLE_Msk;        // enable the MPU
    __ISB();
    __DSB();

    // enable GPIO port PA clock
    RCC->IOPENR |= (1U << 0U);

    // set all used GPIOA pins as push-pull output, no pull-up, pull-down
    GPIOA->MODER &=
        ~((3U << 2U*TST1_PIN) | (3U << 2U*TST2_PIN) | (3U << 2U*TST3_PIN) |
          (3U << 2U*TST4_PIN) | (3U << 2U*TST5_PIN) | (3U << 2U*TST6_PIN));
    GPIOA->MODER |=
         ((1U << 2U*TST1_PIN) | (1U << 2U*TST2_PIN) | (1U << 2U*TST3_PIN) |
          (1U << 2U*TST4_PIN) | (1U << 2U*TST5_PIN) | (1U << 2U*TST6_PIN));
    GPIOA->OTYPER &=
        ~((1U <<    TST1_PIN) | (1U <<    TST2_PIN) | (1U <<    TST3_PIN) |
          (1U <<    TST4_PIN) | (1U <<    TST5_PIN) | (1U <<    TST6_PIN));
    GPIOA->PUPDR &=
        ~((3U << 2U*TST1_PIN) | (3U << 2U*TST2_PIN) | (3U << 2U*TST3_PIN) |
          (3U << 2U*TST4_PIN) | (3U << 2U*TST5_PIN) | (3U << 2U*TST6_PIN));

    // enable GPIOC clock port for the Button B1
    RCC->IOPENR |=  (1U << 2U);

    // configure Button B1 pin on GPIOC as input, no pull-up, pull-down
    GPIOC->MODER &= ~(3U << 2U*B1_PIN);
    GPIOC->PUPDR &= ~(3U << 2U*B1_PIN);
}

//............................................................................
void BSP_d1on(void)  { GPIOA->BSRR = (1U << TST1_PIN);         }
void BSP_d1off(void) { GPIOA->BSRR = (1U << (TST1_PIN + 16U)); }
//............................................................................
void BSP_d2on(void)  { GPIOA->BSRR = (1U << TST2_PIN);         }
void BSP_d2off(void) { GPIOA->BSRR = (1U << (TST2_PIN + 16U)); }
//............................................................................
void BSP_d3on(void)  { GPIOA->BSRR = (1U << TST3_PIN);         }
void BSP_d3off(void) { GPIOA->BSRR = (1U << (TST3_PIN + 16U)); }
//............................................................................
void BSP_d4on(void)  { GPIOA->BSRR = (1U << TST4_PIN);         }
void BSP_d4off(void) { GPIOA->BSRR = (1U << (TST4_PIN + 16U)); }
//............................................................................
void BSP_d5on(void)  { GPIOA->BSRR = (1U << TST5_PIN);         }
void BSP_d5off(void) { GPIOA->BSRR = (1U << (TST5_PIN + 16U)); }
//............................................................................
void BSP_d6on(void)  { GPIOA->BSRR = (1U << TST6_PIN);         } // LED2
void BSP_d6off(void) { GPIOA->BSRR = (1U << (TST6_PIN + 16U)); }

//............................................................................
QEvt const *BSP_getWorkEvtBlinky1(uint8_t num) {
    // immutable work events for Blinky1
    static BlinkyWorkEvt const workBlinky1[] = {
        {
            QEVT_INITIALIZER(BLINKY_WORK_SIG),
            .toggles = 40U,
            .ticks = 5U,
        },
        {
            QEVT_INITIALIZER(BLINKY_WORK_SIG),
            .toggles = 30U,
            .ticks = 7U,
        }
    };
    Q_REQUIRE_ID(500, num < Q_DIM(workBlinky1)); // must be in range
    return &workBlinky1[num].super;
}
//............................................................................
QEvt const *BSP_getWorkEvtBlinky3(uint8_t num) {
    // immutable work events for Blinky3
    static BlinkyWorkEvt const workBlinky3[] = {
        {
            QEVT_INITIALIZER(BLINKY_WORK_SIG),
            .toggles = 20U,
            .ticks = 5U,
        },
        {
            QEVT_INITIALIZER(BLINKY_WORK_SIG),
            .toggles = 10U,
            .ticks = 3U,
        }
    };
    Q_REQUIRE_ID(600, num < Q_DIM(workBlinky3)); // must be in range
    return &workBlinky3[num].super;
}

// QF callbacks ============================================================
void QF_onStartup(void) {
    SystemCoreClockUpdate();

    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config((SystemCoreClock / BSP_TICKS_PER_SEC) + 1U);

    // set priorities of ISRs used in the system
    NVIC_SetPriority(SysTick_IRQn, 0U);
    // ...
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QK_onIdle(void) {
    BSP_d6on();  // turn LED2 on
#ifdef NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    //
    BSP_d6off(); // turn LED2 off
    __WFI(); // Wait-For-Interrupt
    BSP_d6on();  // turn LED2 on
#endif
    BSP_d6off(); // turn LED2 off
}

//............................................................................
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    //
    // NOTE: add here your application-specific error handling
    //
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);

    QS_ASSERTION(module, id, 10000U); // report assertion to QS

#ifndef NDEBUG
    for (;;) {
    }
#endif
    NVIC_SystemReset();
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}


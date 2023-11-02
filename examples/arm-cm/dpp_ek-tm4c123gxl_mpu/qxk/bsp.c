//============================================================================
// Product: DPP example, EK-TM4C123GXL board, QXK kernel, MPU isolation
// Last updated for version 7.3.0
// Last updated on  2023-09-07
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. <state-machine.com>
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"                 // QP/C real-time embedded framework
#include "dpp.h"                 // DPP Application interface
#include "bsp.h"                 // Board Support Package

#include "TM4C123GH6PM.h"        // the device specific header (TI)
#include "rom.h"                 // the built-in ROM functions (TI)
#include "sysctl.h"              // system control driver (TI)
#include "gpio.h"                // GPIO driver (TI)
// add other drivers if necessary...

Q_DEFINE_THIS_FILE  // define the name of this file for assertions

// Local-scope objects -----------------------------------------------------
#define LED_RED     (1U << 1U)
#define LED_GREEN   (1U << 3U)
#define LED_BLUE    (1U << 2U)

#define BTN_SW1     (1U << 4U)
#define BTN_SW2     (1U << 0U)

#ifdef Q_SPY

    // QSpy source IDs
    static QSpyId const l_SysTick_Handler = { 0U };
    static QSpyId const l_GPIOPortA_IRQHandler = { 0U };

    #define UART_BAUD_RATE      115200U
    #define UART_FR_TXFE        (1U << 7U)
    #define UART_FR_RXFE        (1U << 4U)
    #define UART_TXFIFO_DEPTH   16U

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
    };

#endif

//----------------------------------------------------------------------------
// MPU storage and settings...
typedef struct {
    uint32_t RBAR;
    uint32_t RASR;
} MPU_Region;

//============================================================================
// Error handler and ISRs...

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the error handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U);

#ifndef NDEBUG
    // light up all LEDs
    GPIOF_AHB->DATA_Bits[LED_GREEN | LED_RED | LED_BLUE] = 0xFFU;
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#else
    NVIC_SystemReset();
    for (;;) { // explicitly "no-return"
    }
#endif
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// ISRs used in the application ==============================================

void SysTick_Handler(void); // prototype
void SysTick_Handler(void) {
    QXK_ISR_ENTRY();   // inform QXK about entering an ISR

    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); // time events at rate 0

    // Perform the debouncing of buttons. The algorithm for debouncing
    // adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    // and Michael Barr, page 71.
    static struct {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };

    QF_INT_DISABLE();
    QF_MEM_SYS();

    uint32_t current = ~GPIOF_AHB->DATA_Bits[BTN_SW1 | BTN_SW2];
    uint32_t tmp = buttons.depressed; // save the depressed buttons
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    current = buttons.depressed;

    QF_MEM_APP();
    QF_INT_ENABLE();

    if ((tmp & BTN_SW1) != 0U) {  // debounced SW1 state changed?
        if ((current & BTN_SW1) != 0U) { // is SW1 depressed?
            static QEvt const pauseEvt = QEVT_INITIALIZER(PAUSE_SIG);
            QACTIVE_PUBLISH(&pauseEvt, &l_SysTick_Handler);
        }
        else { // the button is released
            static QEvt const serveEvt = QEVT_INITIALIZER(SERVE_SIG);
            QACTIVE_PUBLISH(&serveEvt, &l_SysTick_Handler);
        }
    }

    QXK_ISR_EXIT();  // inform QXK about exiting an ISR
}
//............................................................................
// interrupt handler for testing preemptions
void GPIOPortA_IRQHandler(void); // prototype
void GPIOPortA_IRQHandler(void) {
    QXK_ISR_ENTRY();   // inform QXK about entering an ISR

    // for testing..
    static QEvt const testEvt = QEVT_INITIALIZER(TEST_SIG);
    QACTIVE_POST(AO_Table, &testEvt, &l_GPIOPortA_IRQHandler);

    QXK_ISR_EXIT();  // inform QXK about exiting an ISR
}

//............................................................................
#ifdef Q_SPY
// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "QF-unaware" meaning that it does not interact with
// the QF/QXK and is not disabled. Such ISRs don't need to call
// QXK_ISR_ENTRY/QXK_ISR_EXIT and they cannot post or publish events.

void UART0_IRQHandler(void); // prototype
void UART0_IRQHandler(void) {
    QF_MEM_SYS();

    uint32_t status = UART0->RIS; // get the raw interrupt status
    UART0->ICR = status;          // clear the asserted interrupts

    while ((UART0->FR & UART_FR_RXFE) == 0U) { // while RX FIFO NOT empty
        uint8_t b = (uint8_t)UART0->DR;
        QS_rxPut(b);
    }

    QXK_ARM_ERRATUM_838869();
}
#endif // Q_SPY

#ifdef QF_MEM_ISOLATE
//............................................................................
__attribute__(( used ))
void QF_onMemSys(void) {
    MPU->CTRL = MPU_CTRL_ENABLE_Msk        // enable the MPU
                | MPU_CTRL_PRIVDEFENA_Msk; // enable background region
    __ISB();
    __DSB();
}
//............................................................................
__attribute__(( used ))
void QF_onMemApp() {
    MPU->CTRL = MPU_CTRL_ENABLE_Msk; // enable the MPU
                // but do NOT enable background region
    __ISB();
    __DSB();
}
//............................................................................
#ifdef QF_ON_CONTEXT_SW
// NOTE: the context-switch callback is called with interrupts DISABLED
void QF_onContextSw(QActive *prev, QActive *next) {
    if (next != (QActive *)0) {
        MPU->CTRL = 0U; // disable the MPU

        MPU_Region const * const region =
            (MPU_Region const *)(next->thread);
        MPU->RBAR = region[0].RBAR;
        MPU->RASR = region[0].RASR;
        MPU->RBAR = region[1].RBAR;
        MPU->RASR = region[1].RASR;
        MPU->RBAR = region[2].RBAR;
        MPU->RASR = region[2].RASR;

        MPU->CTRL = MPU_CTRL_ENABLE_Msk        // enable the MPU
                    | MPU_CTRL_PRIVDEFENA_Msk; // enable background region
        __ISB();
        __DSB();
    }
}
#endif // QF_ON_CONTEXT_SW
#endif // QF_MEM_ISOLATE

//============================================================================

// Stack .....................................................................
// NOTE
// The main stack size (provided here as power of 2), MUST match the actual
// stack size defined in the linker-script/startup-code.
#define STACK_SIZE_POW2 ((uint32_t)11U)

// Table AO...................................................................
// size of Table instance, as power-of-2
#define TABLE_SIZE_POW2 ((uint32_t)7U)

__attribute__((aligned((1U << TABLE_SIZE_POW2))))
static uint8_t Table_sto[1U << TABLE_SIZE_POW2];

#ifdef QF_MEM_ISOLATE
static MPU_Region const MPU_Table[3] = {
    { (uint32_t)Table_sto + 0x10U,             //---- region #0
      ((TABLE_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)   // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { GPIOF_AHB_BASE + 0x11U,                  //---- region #1
      ((9U - 1U) << MPU_RASR_SIZE_Pos)                // 2^9=512B size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (0U << MPU_RASR_C_Pos)                       // C=0
       + (1U << MPU_RASR_B_Pos)                       // B=1
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U },
};
#endif

// Philo AOs..................................................................
// size of Philo instance, as power-of-2
#define PHILO_SIZE_POW2 ((uint32_t)7U)

__attribute__((aligned((1U << PHILO_SIZE_POW2))))
static uint8_t Philo_sto[N_PHILO][1U << PHILO_SIZE_POW2];

#define PHILO_SHARED_SIZE_POW2 ((uint32_t)5U)
__attribute__((aligned((1U << PHILO_SHARED_SIZE_POW2))))
static union {
    uint32_t rnd_seed;
    uint8_t byteso[1U << PHILO_SHARED_SIZE_POW2];
} Philo_shared_sto;

#ifdef QF_MEM_ISOLATE
static MPU_Region const MPU_Philo[N_PHILO][3] = {
    {{ (uint32_t)Philo_sto[0] + 0x10U,         //---- region #0
       ((PHILO_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)  // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { (uint32_t)&Philo_shared_sto + 0x11U,            //---- region #1
      ((PHILO_SHARED_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)   // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U }},
    {{ (uint32_t)Philo_sto[1] + 0x10U,         //---- region #0
       ((PHILO_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)  // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { (uint32_t)&Philo_shared_sto + 0x11U,            //---- region #1
      ((PHILO_SHARED_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)   // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U }},
    {{ (uint32_t)Philo_sto[2] + 0x10U,         //---- region #0
       ((PHILO_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)  // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { (uint32_t)&Philo_shared_sto + 0x11U,            //---- region #1
      ((PHILO_SHARED_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)   // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U }},
    {{ (uint32_t)Philo_sto[3] + 0x10U,         //---- region #0
       ((PHILO_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)  // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { (uint32_t)&Philo_shared_sto + 0x11U,            //---- region #1
      ((PHILO_SHARED_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)   // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U }},
    {{ (uint32_t)Philo_sto[4] + 0x10U,         //---- region #0
       ((PHILO_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)  // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { (uint32_t)&Philo_shared_sto + 0x11U,            //---- region #1
      ((PHILO_SHARED_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)   // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U }},
};
#endif

// XThread1 thread............................................................
#define XTHREAD1_SIZE_POW2  ((uint32_t)10U)   // XThread1 instance + stack
#define XTHREAD1_STACK_SIZE ((uint32_t)880U)  // Thread1 stack size

__attribute__((aligned((1U << XTHREAD1_SIZE_POW2))))
uint8_t XThread1_sto[1U << XTHREAD1_SIZE_POW2];

#ifdef QF_MEM_ISOLATE
static MPU_Region const MPU_XThread1[3] = {
    { (uint32_t)XThread1_sto + 0x10U,          //---- region #0
      ((XTHREAD1_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos) // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { GPIOF_AHB_BASE + 0x11U,                  //---- region #1
      ((9U - 1U) << MPU_RASR_SIZE_Pos)                // 2^9=512B size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (0U << MPU_RASR_C_Pos)                       // C=0
       + (1U << MPU_RASR_B_Pos)                       // B=1
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U },
};
#endif

// XThread2 thread............................................................
#define XTHREAD2_SIZE_POW2  ((uint32_t)10U)   // XThread2 instance + stack
#define XTHREAD2_STACK_SIZE ((uint32_t)880U)  // Thread2 stack size

__attribute__((aligned((1U << XTHREAD2_SIZE_POW2))))
uint8_t XThread2_sto[1U << XTHREAD2_SIZE_POW2];

#ifdef QF_MEM_ISOLATE
static MPU_Region const MPU_XThread2[3] = {
    { (uint32_t)XThread2_sto + 0x10U,          //---- region #0
      ((XTHREAD2_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos) // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { GPIOF_AHB_BASE + 0x11U,                  //---- region #1
      ((9U - 1U) << MPU_RASR_SIZE_Pos)                // 2^9=512B size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (0U << MPU_RASR_C_Pos)                       // C=0
       + (1U << MPU_RASR_B_Pos)                       // B=1
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk },                       // region enable
    { 0U + 0x12U,                              //---- region #2
      0U },
};
#endif

// Shared Event-pools......................................................
#define EPOOLS_SIZE_POW2 ((uint32_t)8U)

__attribute__((aligned((1U << EPOOLS_SIZE_POW2))))
static struct EPools {
    QF_MPOOL_EL(TableEvt) smlPool[2*N_PHILO];
    // ... other pools
} EPools_sto;
Q_ASSERT_STATIC(sizeof(EPools_sto) <= (1U << EPOOLS_SIZE_POW2));

//............................................................................
#ifdef QF_MEM_ISOLATE
static void TM4C123GXL_MPU_setup(void) {

    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    MPU->CTRL = 0U; // disable the MPU

    // region #7: NULL-pointer protection region
    MPU->RBAR = 0x00000000U + 0x17U;        // base address + region #7
    MPU->RASR = ((8U - 1U) << MPU_RASR_SIZE_Pos) // 2^8=256B size
       + (0U << MPU_RASR_AP_Pos)                      // PA:na/UA:na
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (0U << MPU_RASR_S_Pos)                       // S=0
       + (0U << MPU_RASR_C_Pos)                       // C=0
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk;                         // region enable

    // region #6: stack region
    MPU->RBAR = 0x20000000U + 0x16U;        // base address + region #6
    MPU->RASR = ((STACK_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)  // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk;                         // region enable

    // region #5: ROM region for TM4C123GXL, whole 256K
    MPU->RBAR = 0x00000000U + 0x15U;        // base address + region #5
    MPU->RASR = ((18U - 1U) << MPU_RASR_SIZE_Pos)     // 2^18=256K size
       + (6U << MPU_RASR_AP_Pos)                      // PA:ro/UA:ro
       + (0U << MPU_RASR_XN_Pos)                      // XN=0
       + (0U << MPU_RASR_S_Pos)                       // S=0
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk;                         // region enable

    // region #4: Event-pools region
    MPU->RBAR = (uint32_t)&EPools_sto + 0x14U;// base address + region #4
    MPU->RASR = ((EPOOLS_SIZE_POW2 - 1U) << MPU_RASR_SIZE_Pos)  // size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk;                         // region enable

#ifdef Q_SPY
    // region #3: QS-filters region
    MPU->RBAR = (uint32_t)&QS_filt_ + 0x13U;// base address + region #3
    MPU->RASR = ((5U - 1U) << MPU_RASR_SIZE_Pos)      // 2^5=32B size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk;                         // region enable
#endif

    // region #0: temporary 4G region for initial transient
    MPU->RBAR = 0x00000000U + 0x10U;        // base address + region #0
    MPU->RASR = ((32U - 1U) << MPU_RASR_SIZE_Pos) // 2^32=4G size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (0U << MPU_RASR_XN_Pos)                      // XN=0
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk;                         // region enable

    MPU->CTRL = MPU_CTRL_ENABLE_Msk; // enable the MPU
                // but do NOT enable background region
    __ISB();
    __DSB();
}
#endif

//============================================================================

// "opaque" pointer to AO
QActive * const AO_Table = (QActive *)Table_sto;

QActive * const AO_Philo[N_PHILO] = {
    (QActive *)Philo_sto[0], // "opaque" pointer
    (QActive *)Philo_sto[1], // "opaque" pointer
    (QActive *)Philo_sto[2], // "opaque" pointer
    (QActive *)Philo_sto[3], // "opaque" pointer
    (QActive *)Philo_sto[4]  // "opaque" pointer
};

static uint8_t * const XThread1_inst = &XThread1_sto[XTHREAD1_STACK_SIZE];
QXThread * const TH_XThread1 = (QXThread *)&XThread1_sto[XTHREAD1_STACK_SIZE];

static uint8_t * const XThread2_inst = &XThread2_sto[XTHREAD2_STACK_SIZE];
QXThread * const TH_XThread2 = (QXThread *)&XThread2_sto[XTHREAD2_STACK_SIZE];

// BSP functions ===========================================================
void BSP_init(void) {
#ifdef QF_MEM_ISOLATE
    // setup the MPU
    TM4C123GXL_MPU_setup();
#endif

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    // NOTE: The VFP (hardware Floating Point) unit is configured by QXK

    // enable clock for to the peripherals used by this application...
    SYSCTL->RCGCGPIO  |= (1U << 5U); // enable Run mode for GPIOF
    SYSCTL->GPIOHBCTL |= (1U << 5U); // enable AHB for GPIOF
    __ISB();
    __DSB();

    // configure LEDs (digital output)
    GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
    GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN);
    GPIOF_AHB->DATA_Bits[LED_RED | LED_BLUE | LED_GREEN] = 0U;

    // configure switches...

    // unlock access to the SW2 pin because it is PROTECTED
    GPIOF_AHB->LOCK = 0x4C4F434BU; // unlock GPIOCR register for SW2
    // commit the write (cast const away)
    *(uint32_t volatile *)&GPIOF_AHB->CR = 0x01U;

    GPIOF_AHB->DIR &= ~(BTN_SW1 | BTN_SW2); // input
    GPIOF_AHB->DEN |= (BTN_SW1 | BTN_SW2); // digital enable
    GPIOF_AHB->PUR |= (BTN_SW1 | BTN_SW2); // pull-up resistor enable

    *(uint32_t volatile *)&GPIOF_AHB->CR = 0x00U;
    GPIOF_AHB->LOCK = 0x0; // lock GPIOCR register for SW2

    BSP_randomSeed(1234U); // seed the random number generator

    // initialize the QS software tracing...
    if (!QS_INIT((void *)0)) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_GPIOPortA_IRQHandler);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);

    QS_ONLY(produce_sig_dict());

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS);   // all records
    QS_GLB_FILTER(-QS_QF_TICK);      // exclude the clock tick
}
//............................................................................
void BSP_start(void) {
    // initialize event pools
    QF_poolInit(EPools_sto.smlPool,
        sizeof(EPools_sto.smlPool),
        sizeof(EPools_sto.smlPool[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // instantiate and start AOs/threads...
    static QEvt const *xThread1QueueSto[5];
#ifdef QF_MEM_ISOLATE
    XThread1_ctor(XThread1_inst,
                  sizeof(XThread1_sto) - XTHREAD1_STACK_SIZE, MPU_XThread1);
#else
    XThread1_ctor(XThread1_inst,
                  sizeof(XThread1_sto) - XTHREAD1_STACK_SIZE, (void *)0);
#endif
    QXTHREAD_START(TH_XThread1,
        1U,                          // QP priority of the thread
        xThread1QueueSto,            // event queue storage
        Q_DIM(xThread1QueueSto),     // event length [events]
        &XThread1_sto[0],            // stack storage
        XTHREAD1_STACK_SIZE,         // stack size [bytes]
        (void *)0);                  // no initialization param

    static QEvt const *philoQueueSto[N_PHILO][10];
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
#ifdef QF_MEM_ISOLATE
        Philo_ctor(n, Philo_sto[n], sizeof(Philo_sto[n]), MPU_Philo[n]);
#else
        Philo_ctor(n, Philo_sto[n], sizeof(Philo_sto[n]), (void *)0);
#endif
        QACTIVE_START(AO_Philo[n],
            n + 3U,                  // QF-prio/pthre. see NOTE1
            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            (void *)0, 0U,           // no stack storage
            (void *)0);              // no initialization param
    }

    static QEvt const *xThread2QueueSto[5];
#ifdef QF_MEM_ISOLATE
    XThread2_ctor(XThread2_inst,
                  sizeof(XThread2_sto) - XTHREAD2_STACK_SIZE, MPU_XThread2);
#else
    XThread2_ctor(XThread2_inst,
                  sizeof(XThread2_sto) - XTHREAD2_STACK_SIZE, (void *)0);
#endif
    QXTHREAD_START(TH_XThread2,
        N_PHILO + 5U,                // QP priority of the thread
        xThread2QueueSto,            // event queue storage
        Q_DIM(xThread2QueueSto),     // event length [events]
        &XThread2_sto[0],            // stack storage
        XTHREAD2_STACK_SIZE,         // stack size [bytes]
        (void *)0);                  // no initialization param

    static QEvt const *tableQueueSto[N_PHILO];
#ifdef QF_MEM_ISOLATE
    Table_ctor(Table_sto, sizeof(Table_sto), MPU_Table);
#else
    Table_ctor(Table_sto, sizeof(Table_sto), (void *)0);
#endif
    QACTIVE_START(AO_Table,
        N_PHILO + 7U,                // QP prio. of the AO
        tableQueueSto,               // event queue storage
        Q_DIM(tableQueueSto),        // queue length [events]
        (void *)0, 0U,               // no stack storage
        (void *)0);                  // no initialization param
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    Q_UNUSED_PAR(n);

    GPIOF_AHB->DATA_Bits[LED_GREEN] = ((stat[0] == 'e') ? LED_GREEN : 0U);

    // app-specific trace record...
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    GPIOF_AHB->DATA_Bits[LED_BLUE] = ((paused != 0U) ? LED_BLUE : 0U);

    // application-specific trace record
    QS_BEGIN_ID(PAUSED_STAT, AO_Table->prio)
        QS_U8(1, paused);  // Paused status
    QS_END()
}
//............................................................................
void BSP_randomSeed(uint32_t seed) {
    Philo_shared_sto.rnd_seed = seed;
}
//............................................................................
uint32_t BSP_random() { // a very cheap pseudo-random-number generator
    // Some floating point code is to exercise the VFP...
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    QSchedStatus lockStat = QXK_schedLock(N_PHILO);
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    uint32_t rnd = Philo_shared_sto.rnd_seed * (3U*7U*11U*13U*23U);
    Philo_shared_sto.rnd_seed = rnd; // set for the next time
    QXK_schedUnlock(lockStat);

    return (rnd >> 8U);
}
//............................................................................
void BSP_ledOn() {
    GPIOF_AHB->DATA_Bits[LED_RED] = 0xFFU;
}
//............................................................................
void BSP_ledOff() {
    GPIOF_AHB->DATA_Bits[LED_RED] = 0x00U;
}
//............................................................................
void BSP_terminate(int16_t result) {
    Q_UNUSED_PAR(result);
}

//============================================================================
// QF callbacks...
void QF_onStartup(void) {
    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    // assign all priority bits for preemption-prio. and none to sub-prio.
    NVIC_SetPriorityGrouping(0U);

    // set priorities of ALL ISRs used in the system, see NOTE1
    NVIC_SetPriority(UART0_IRQn,     0U); // kernel unaware interrupt
    NVIC_SetPriority(GPIOA_IRQn,     QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    // ...

    // enable IRQs...
    NVIC_EnableIRQ(GPIOA_IRQn);

#ifdef Q_SPY
    NVIC_EnableIRQ(UART0_IRQn); // UART interrupt used for QS-RX
#endif
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QXK_onIdle(void) {
    // toggle the User LED on and then off, see NOTE3
    QF_INT_DISABLE();
    QF_MEM_SYS();
    GPIOF_AHB->DATA_Bits[LED_BLUE] = 0xFFU;  // turn the Blue LED on
    GPIOF_AHB->DATA_Bits[LED_BLUE] = 0U;     // turn the Blue LED off
    QF_MEM_APP();
    QF_INT_ENABLE();

    // Some floating point code is to exercise the VFP...
    float volatile x = 1.73205F;
    x = x * 1.73205F;

#ifdef Q_SPY
    QF_INT_DISABLE();
    QF_MEM_SYS();
    QS_rxParse();  // parse all the received bytes
    QF_MEM_APP();
    QF_INT_ENABLE();
    QF_CRIT_EXIT_NOP();

    QF_INT_DISABLE();
    QF_MEM_SYS();
    if ((UART0->FR & UART_FR_TXFE) != 0U) { // TX done?
        uint16_t fifo = UART_TXFIFO_DEPTH; // max bytes we can accept

        // try to get next contiguous block to transmit
        uint8_t const *block = QS_getBlock(&fifo);

        while (fifo-- != 0U) {    // any bytes in the block?
            UART0->DR = *block++; // put into the FIFO
        }
    }
    QF_MEM_APP();
    QF_INT_ENABLE();
#elif defined NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    //
    __WFI(); // Wait-For-Interrupt
#endif
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY

#ifdef QF_MEM_ISOLATE
// Shared QS filters...
__attribute__((aligned(32)))
QS_Filter QS_filt_;
#endif

//............................................................................
uint8_t QS_onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf(qsTxBuf, sizeof(qsTxBuf));

    static uint8_t qsRxBuf[100];    // buffer for QS-RX channel
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // enable clock for UART0 and GPIOA (used by UART0 pins)
    SYSCTL->RCGCUART |= (1U << 0U); // enable Run mode for UART0
    SYSCTL->RCGCGPIO |= (1U << 0U); // enable Run mode for GPIOA

    // configure UART0 pins for UART operation
    uint32_t tmp = (1U << 0U) | (1U << 1U);
    GPIOA->DIR   &= ~tmp;
    GPIOA->SLR   &= ~tmp;
    GPIOA->ODR   &= ~tmp;
    GPIOA->PUR   &= ~tmp;
    GPIOA->PDR   &= ~tmp;
    GPIOA->AMSEL &= ~tmp;  // disable analog function on the pins
    GPIOA->AFSEL |= tmp;   // enable ALT function on the pins
    GPIOA->DEN   |= tmp;   // enable digital I/O on the pins
    GPIOA->PCTL  &= ~0x00U;
    GPIOA->PCTL  |= 0x11U;

    // configure the UART for the desired baud rate, 8-N-1 operation
    tmp = (((SystemCoreClock * 8U) / UART_BAUD_RATE) + 1U) / 2U;
    UART0->IBRD   = tmp / 64U;
    UART0->FBRD   = tmp % 64U;
    UART0->LCRH   = (0x3U << 5U); // configure 8-N-1 operation
    UART0->LCRH  |= (0x1U << 4U); // enable FIFOs
    UART0->CTL    = (1U << 0U)    // UART enable
                    | (1U << 8U)  // UART TX enable
                    | (1U << 9U); // UART RX enable

    // configure UART interrupts (for the RX channel)
    UART0->IM   |= (1U << 4U) | (1U << 6U); // enable RX and RX-TO interrupt
    UART0->IFLS |= (0x2U << 2U);    // interrupt on RX FIFO half-full
    // NOTE: do not enable the UART0 interrupt yet. Wait till QF_onStartup()

    // configure TIMER5 to produce QS time stamp
    SYSCTL->RCGCTIMER |= (1U << 5U); // enable run mode for Timer5
    TIMER5->CTL  = 0U;               // disable Timer1 output
    TIMER5->CFG  = 0x0U;             // 32-bit configuration
    TIMER5->TAMR = (1U << 4U) | 0x02U; // up-counting periodic mode
    TIMER5->TAILR= 0xFFFFFFFFU;      // timer interval
    TIMER5->ICR  = 0x1U;             // TimerA timeout flag bit clears
    TIMER5->CTL |= (1U << 0U);       // enable TimerA module

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) { // NOTE: invoked with interrupts DISABLED
    return TIMER5->TAV;
}
//............................................................................
void QS_onFlush(void) {
    for (;;) {
        QF_INT_DISABLE();
        QF_MEM_SYS();
        uint16_t b = QS_getByte();
        if (b != QS_EOD) {
            while ((UART0->FR & UART_FR_TXFE) == 0U) { // while TXE not empty
                QF_MEM_APP();
                QF_INT_ENABLE();
                QF_CRIT_EXIT_NOP();

                QF_INT_DISABLE();
                QF_MEM_SYS();
            }
            UART0->DR = b; // put into the DR register
            QF_MEM_APP();
            QF_INT_ENABLE();
        }
        else {
            QF_MEM_APP();
            QF_INT_ENABLE();
            break;
        }
    }
}
//............................................................................
//! callback function to reset the target (to be implemented in the BSP)
void QS_onReset(void) {
    NVIC_SystemReset();
}
//............................................................................
//! callback function to execute a user command
void QS_onCommand(uint8_t cmdId, uint32_t param1,
                  uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

#endif // Q_SPY
//----------------------------------------------------------------------------

//============================================================================
// NOTE1:
// The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
// ISR priority that is disabled by the QF framework. The value is suitable
// for the NVIC_SetPriority() CMSIS function.
//
// Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
// with the numerical values of priorities equal or higher than
// QF_AWARE_ISR_CMSIS_PRI) are allowed to call the QK_ISR_ENTRY/
// QXK_ISR_ENTRY macros or any other QF/QXK services. These ISRs are
// "QF-aware".
//
// Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
// level (i.e., with the numerical values of priorities less than
// QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
// Such "QF-unaware" ISRs cannot call ANY QF/QXK services. In particular they
// can NOT call the macros QXK_ISR_ENTRY/QXK_ISR_ENTRY. The only mechanism
// by which a "QF-unaware" ISR can communicate with the QF framework is by
// triggering a "QF-aware" ISR, which can post/publish events.
//
// NOTE2:
// The User LED is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of the idle loop.
// Please note that the LED is toggled with interrupts locked, so no interrupt
// execution time contributes to the brightness of the User LED.
//

//============================================================================
// Product: DPP example, NUCLEO-C031C6 board, QV kernel, MPU isolation
// Last updated for version 7.3.2
// Last updated on  2023-12-13
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

#include "stm32c0xx.h"  // CMSIS-compliant header file for the MCU used
// add other drivers if necessary...

Q_DEFINE_THIS_FILE  // define the name of this file for assertions

// Local-scope defines -----------------------------------------------------
// LED pins available on the board (just one user LED LD4--Green on PA.5)
#define LD4_PIN  5U

// Button pins available on the board (just one user Button B1 on PC.13)
#define B1_PIN   13U

#ifdef Q_SPY
    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    // QSpy source IDs
    static QSpyId const l_SysTick_Handler = { 0U };
    static QSpyId const l_EXTI0_1_IRQHandler = { 0U };

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

Q_NORETURN Q_onError(char const *module, int_t const id) {
    // NOTE: this implementation of the assertion handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U);

#ifndef NDEBUG
    // light up the user LED
    GPIOA->BSRR = (1U << LD4_PIN);  // turn LED on
    // for debugging, hang on in an endless loop...
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

// ISRs used in the application ==========================================
void SysTick_Handler(void); // prototype
void SysTick_Handler(void) {
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

    uint32_t current = ~GPIOC->IDR; // read Port C with Button B1
    uint32_t tmp = buttons.depressed; // save the depressed buttons
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    current = buttons.depressed;

#ifdef Q_SPY
    tmp = SysTick->CTRL; // clear CTRL_COUNTFLAG
    QS_tickTime_ += QS_tickPeriod_; // account for the clock rollover
#endif

    QF_MEM_APP();
    QF_INT_ENABLE();

    if ((tmp & (1U << B1_PIN)) != 0U) { // debounced B1 state changed?
        if ((current & (1U << B1_PIN)) != 0U) { // is B1 depressed?
            static QEvt const pauseEvt = QEVT_INITIALIZER(PAUSE_SIG);
            QACTIVE_PUBLISH(&pauseEvt, &l_SysTick_Handler);
        }
        else { // the button is released
            static QEvt const serveEvt = QEVT_INITIALIZER(SERVE_SIG);
            QACTIVE_PUBLISH(&serveEvt, &l_SysTick_Handler);
        }
    }

    QV_ARM_ERRATUM_838869();
}
//............................................................................
// interrupt handler for testing preemptions in QV
void EXTI0_1_IRQHandler(void); // prototype
void EXTI0_1_IRQHandler(void) {
    // for testing..
    static QEvt const testEvt = QEVT_INITIALIZER(TEST_SIG);
    QACTIVE_POST(AO_Table, &testEvt, (void *)0);

    QV_ARM_ERRATUM_838869();
}

//............................................................................
#ifdef Q_SPY
// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "QF-unaware" meaning that it does not interact with
// the QF/QV and is not disabled. Such ISRs cannot post or publish events.

void USART2_IRQHandler(void); // prototype
void USART2_IRQHandler(void) { // used in QS-RX (kernel UNAWARE interrutp)
    // is RX register NOT empty?
    QF_MEM_SYS();
    if ((USART2->ISR & (1U << 5)) != 0) {
        uint32_t b = USART2->RDR;
        QS_RX_PUT(b);
    }

    QV_ARM_ERRATUM_838869();
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
#define TABLE_SIZE_POW2 ((uint32_t)6U)

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
    { GPIOA_BASE + 0x11U,                      //---- region #1
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

// Philo AOs................................................................
// size of Philo instance, as power-of-2
#define PHILO_SIZE_POW2 ((uint32_t)6U)

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
static void STM32C031C6_MPU_setup(void) {

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

    // region #5: ROM region for STM32C031C6, whole 32K
    MPU->RBAR = 0x08000000U + 0x15U;        // base address + region #5
    MPU->RASR = ((15U - 1U) << MPU_RASR_SIZE_Pos)     // 2^15=32K size
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
    MPU->RASR = ((8U - 1U) << MPU_RASR_SIZE_Pos)      // 2^8=256B size
       + (3U << MPU_RASR_AP_Pos)                      // PA:rw/UA:rw
       + (1U << MPU_RASR_XN_Pos)                      // XN=1
       + (1U << MPU_RASR_S_Pos)                       // S=1
       + (1U << MPU_RASR_C_Pos)                       // C=1
       + (0U << MPU_RASR_B_Pos)                       // B=0
       + (0U << MPU_RASR_TEX_Pos)                     // TEX=0
       + MPU_RASR_ENABLE_Msk;                         // region enable
#endif

    // region #0: temporary 4G region for the initial transient
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

// BSP functions ===========================================================
void BSP_init(void) {
#ifdef QF_MEM_ISOLATE
    // setup the MPU
    STM32C031C6_MPU_setup();
#endif

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    // enable GPIOA clock port for the LED LD4
    RCC->IOPENR |= (1U << 0U);

    // set all used GPIOA pins as push-pull output, no pull-up, pull-down
    GPIOA->MODER   &= ~(3U << 2U*LD4_PIN);
    GPIOA->MODER   |=  (1U << 2U*LD4_PIN);
    GPIOA->OTYPER  &= ~(1U <<    LD4_PIN);
    GPIOA->OSPEEDR &= ~(3U << 2U*LD4_PIN);
    GPIOA->OSPEEDR |=  (1U << 2U*LD4_PIN);
    GPIOA->PUPDR   &= ~(3U << 2U*LD4_PIN);

    // enable GPIOC clock port for the Button B1
    RCC->IOPENR |=  (1U << 2U);

    // configure Button B1 pin on GPIOC as input, no pull-up, pull-down
    GPIOC->MODER &= ~(3U << 2U*B1_PIN);
    GPIOC->PUPDR &= ~(3U << 2U*B1_PIN);

    BSP_randomSeed(1234U); // seed the random number generator

    // initialize the QS software tracing...
    if (!QS_INIT((void *)0)) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_EXTI0_1_IRQHandler);
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

    if (stat[0] == 'e') {
        GPIOA->BSRR = (1U << LD4_PIN);  // turn LED on
    }
    else {
        GPIOA->BSRR = (1U << (LD4_PIN + 16U));  // turn LED off
    }

    // app-specific trace record...
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    // not enough LEDs to implement this feature
    if (paused != 0U) {
        //GPIOA->BSRR = (1U << LD4_PIN);  // turn LED[n] on
    }
    else {
        //GPIOA->BSRR = (1U << (LD4_PIN + 16U));  // turn LED[n] off
    }

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
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    uint32_t rnd = Philo_shared_sto.rnd_seed * (3U*7U*11U*13U*23U);
    Philo_shared_sto.rnd_seed = rnd; // set for the next time

    return (rnd >> 8U);
}
//............................................................................
void BSP_ledOn() {
    GPIOA->BSRR = (1U << LD4_PIN);  // turn LED on
}
//............................................................................
void BSP_ledOff() {
    GPIOA->BSRR = (1U << (LD4_PIN + 16U));  // turn LED off
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
    // NOTE: this might have been changed by STM32Cube.
    NVIC_SetPriorityGrouping(0U);

    // set priorities of ALL ISRs used in the system, see NOTE1
    NVIC_SetPriority(USART2_IRQn,    0U); // kernel UNAWARE interrupt
    NVIC_SetPriority(EXTI0_1_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    // ...

    // enable IRQs...
    NVIC_EnableIRQ(EXTI0_1_IRQn);

#ifdef Q_SPY
    NVIC_EnableIRQ(USART2_IRQn); // UART2 interrupt used for QS-RX
#endif
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QV_onIdle(void) { // CAUTION: called with interrupts disabled, see NOTE0

    // toggle an LED on and then off (not enough LEDs, see NOTE02)
    QF_MEM_SYS();
    //GPIOA->BSRR = (1U << LD4_PIN);         // turn LED[n] on
    //GPIOA->BSRR = (1U << (LD4_PIN + 16U)); // turn LED[n] off

#ifdef Q_SPY
    // interrupts still disabled
    QS_rxParse();  // parse all the received bytes
    QF_MEM_APP();
    QF_INT_ENABLE();
    QF_CRIT_EXIT_NOP();

    QF_INT_DISABLE();
    QF_MEM_SYS();
    if ((USART2->ISR & (1U << 7U)) != 0U) {  // is TXE empty?
        uint16_t b = QS_getByte();
        if (b != QS_EOD) {   // not End-Of-Data?
            USART2->TDR = b; // put into the DR register
        }
    }
    QF_MEM_APP();
    QF_INT_ENABLE();
#elif defined NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    //
    // !!!CAUTION!!!
    // QV_CPU_SLEEP() contains the WFI instruction, which stops the CPU
    // clock, which unfortunately disables the JTAG port, so the ST-Link
    // debugger can no longer connect to the board. For that reason, the call
    // to QV_CPU_SLEEP() has to be used with CAUTION.
    //
    // NOTE: If you find your board "frozen" like this, strap BOOT0 to VDD and
    // reset the board, then connect with ST-Link Utilities and erase the part.
    // The trick with BOOT(0) is it gets the part to run the System Loader
    // instead of your broken code. When done disconnect BOOT0, and start over.
    //
    //QV_CPU_SLEEP(); // atomically go to sleep and enable interrupts
    QF_MEM_APP();
    QF_INT_ENABLE(); // for now, just enable interrupts
#else
    QF_MEM_APP();
    QF_INT_ENABLE(); // just enable interrupts
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
static uint16_t const UARTPrescTable[12] = {
    1U, 2U, 4U, 6U, 8U, 10U, 12U, 16U, 32U, 64U, 128U, 256U
};

#define UART_DIV_SAMPLING16(__PCLK__, __BAUD__, __CLOCKPRESCALER__) \
  ((((__PCLK__)/UARTPrescTable[(__CLOCKPRESCALER__)]) \
  + ((__BAUD__)/2U)) / (__BAUD__))

#define UART_PRESCALER_DIV1  0U

// USART2 pins PA.2 and PA.3
#define USART2_TX_PIN 2U
#define USART2_RX_PIN 3U

//............................................................................
uint8_t QS_onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf(qsTxBuf, sizeof(qsTxBuf));

    static uint8_t qsRxBuf[100];    // buffer for QS-RX channel
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // enable peripheral clock for USART2
    RCC->IOPENR  |= ( 1U <<  0U);  // Enable GPIOA clock for USART pins
    RCC->APBENR1 |= ( 1U << 17U);  // Enable USART#2 clock

    // Configure PA to USART2_RX, PA to USART2_TX
    GPIOA->AFR[0] &= ~((15U << 4U*USART2_RX_PIN) | (15U << 4U*USART2_TX_PIN));
    GPIOA->AFR[0] |=  (( 1U << 4U*USART2_RX_PIN) | ( 1U << 4U*USART2_TX_PIN));
    GPIOA->MODER  &= ~(( 3U << 2U*USART2_RX_PIN) | ( 3U << 2U*USART2_TX_PIN));
    GPIOA->MODER  |=  (( 2U << 2U*USART2_RX_PIN) | ( 2U << 2U*USART2_TX_PIN));

    // baud rate
    USART2->BRR  = UART_DIV_SAMPLING16(
                       SystemCoreClock, 115200U, UART_PRESCALER_DIV1);
    USART2->CR3  = 0x0000U |      // no flow control
                   (1U << 12U);   // disable overrun detection (OVRDIS)
    USART2->CR2  = 0x0000U;       // 1 stop bit
    USART2->CR1  = ((1U <<  2U) | // enable RX
                    (1U <<  3U) | // enable TX
                    (1U <<  5U) | // enable RX interrupt
                    (0U << 12U) | // 8 data bits
                    (0U << 28U) | // 8 data bits
                    (1U <<  0U)); // enable USART

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; // to start the timestamp at zero

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) { // NOTE: invoked with interrupts DISABLED
    if ((SysTick->CTRL & 0x00010000U) == 0U) { // not set?
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { // the rollover occured, but the SysTick_ISR did not run yet
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
//............................................................................
// NOTE:
// No critical section in QS_onFlush() to avoid nesting of critical sections
// in case QS_onFlush() is called from Q_onError().
void QS_onFlush(void) {
    for (;;) {
        uint16_t b = QS_getByte();
        if (b != QS_EOD) {
            while ((USART2->ISR & (1U << 7U)) == 0U) { // while TXE not empty
            }
            USART2->TDR = b; // put into the DR register
        }
        else {
            break;
        }
    }
}
//............................................................................
void QS_onReset(void) {
    NVIC_SystemReset();
}
//............................................................................
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

#endif // Q_SPY
//----------------------------------------------------------------------------

//============================================================================
// NOTE0:
// The QV_onIdle() callback is called with interrupts disabled, because the
// determination of the idle condition might change by any interrupt posting
// an event. QV_onIdle() must internally enable interrupts, ideally
// atomically with putting the CPU to the power-saving mode.
//
// NOTE1:
// The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
// ISR priority that is disabled by the QF framework. The value is suitable
// for the NVIC_SetPriority() CMSIS function.
//
// Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
// with the numerical values of priorities equal or higher than
// QF_AWARE_ISR_CMSIS_PRI) are allowed to call the QF/QV services. These ISRs
// are "QF-aware".
//
// Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
// level (i.e., with the numerical values of priorities less than
// QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
// Such "QF-unaware" ISRs cannot call ANY QF/QV services. In particular they
// can NOT call any QF/QV services. The only mechanism by which a "QF-unaware"
// ISR can communicate with the QF framework is by pending a "QF-aware" ISR,
// which can post/publish events.
//
// NOTE2:
// The User LED is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of the idle loop.
// Please note that the LED is toggled with interrupts locked, so no interrupt
// execution time contributes to the brightness of the User LED.
//

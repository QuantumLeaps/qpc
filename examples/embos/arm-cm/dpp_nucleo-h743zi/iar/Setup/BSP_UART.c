/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2021 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: V5.14.0.0                                        *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
Purpose : UART implementation for ST NUCLEO-H743ZI(2).
Additional information:

  Device : STM32H743
  Board  : ST NUCLEO-H743ZI
  Unit | UART    | Board connector
  =============================================================
  0    | USART1  | TTL Adapter (PB6: USART1_TX, PB15: USART1_RX)
*/

#include "BSP_UART.h"
#include "RTOS.h"
#include "stm32h7xx.h"   // Device specific header file, contains CMSIS

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       UART settings
*/
#define BSP_UART_BAUDRATE     (38400)
#define BSP_UART_CLOCK        (SystemCoreClock / 2 / 4)  // SystemCoreClock / RCC_D1CFGR_HPRE_DIV2 / RCC_D2CFGR_D2PPRE1_DIV4
#define BSP_UART_IRQHandler   (USART1_IRQHandler)

#ifndef   DEBUG
  #define DEBUG  (0)
#endif


#define ISR_ID_USART1         USART1_IRQn    // USART1 ID
#define USART_BASE_ADDR       (0x40011000u)  // USART1 base address

#define USART_ISR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x1Cu))
#define USART_ICR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x20u))
#define USART_RDR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x24u))
#define USART_TDR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x28u))
#define USART_BRR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x0Cu))
#define USART_CR1             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x00u))
#define USART_CR2             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x04u))

#define RCC_BASE_ADDR         (0x58024400u)
#define RCC_AHB4ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0xE0u))
#define RCC_APB2ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0xF0u))

#define GPIOB_BASE_ADDR       (0x58020400u)
#define GPIOB_MODER           (*(volatile unsigned long*)(GPIOB_BASE_ADDR + 0x00u))
#define GPIOB_OTYPER          (*(volatile unsigned long*)(GPIOB_BASE_ADDR + 0x04u))
#define GPIOB_OSPEEDR         (*(volatile unsigned long*)(GPIOB_BASE_ADDR + 0x08u))
#define GPIOB_PUPDR           (*(volatile unsigned long*)(GPIOB_BASE_ADDR + 0x0Cu))
#define GPIOB_AF_LOW          (*(volatile unsigned long*)(GPIOB_BASE_ADDR + 0x20u))
#define GPIOB_AF_HIGH         (*(volatile unsigned long*)(GPIOB_BASE_ADDR + 0x24u))

#define US_RXRDY              (0x20u)  // RXNE
#define USART_RX_ERROR_FLAGS  (0x0Fu)  // ORE/NE/FE/PE
#define US_TXEMPTY            (0x80u)  // TXE

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static BSP_UART_RX_CB* _pfOnRx;
static BSP_UART_TX_CB* _pfOnTx;

#if (DEBUG != 0)
//
// Variables used for debugging.
//
static volatile unsigned char _Send1Buffer;
static unsigned long          _ErrCnt;
static unsigned long          _RxCnt;
static unsigned char          _RxData[1024];
static unsigned long          _TxCnt;
static unsigned char          _TxData[1024];
#endif

/*********************************************************************
*
*       Prototypes
*
*  Declare ISR handler here to avoid "no prototype" warning.
*  They are not declared in any CMSIS header.
*
**********************************************************************
*/

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

void BSP_UART_IRQHandler(void);

#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _SetBaudrate()
*
*  Function description
*    Configures the UART baud rate.
*
*  Parameters
*    Unit    : Unit number (typically zero-based).
*    Baudrate: Baud rate to configure [Hz].
*/
static void _SetBaudrate(unsigned int Unit, unsigned long Baudrate) {
  unsigned short Baud;

  BSP_UART_USE_PARA(Unit);
  USART_CR1 &= ~((5u << 5)    // RX and TX interrupt disable
             |   (3u << 2)    // TX and RX disable
             |   (1u << 0));  // USART disable
  Baud       = BSP_UART_CLOCK / Baudrate;
  USART_BRR  = Baud;          // Set baudrate
  USART_CR1  = (3u << 2)      // TX and RX enable
             | (1u << 5)      // RX interrupt enable
             | (1u << 0);     // USART enable
}

/*********************************************************************
*
*       Global functions, IRQ handler
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_UART_IRQHandler()
*
* Function description
*    UART Rx & Tx interrupt handler.
*
*  Additional information
*    Needs to inform the OS that we are in interrupt context.
*/
void BSP_UART_IRQHandler(void) {
  volatile unsigned int Dummy;
  unsigned long         UsartStatus;

  OS_EnterNestableInterrupt();
  UsartStatus = USART_ISR;                       // Examine status register
  USART_ICR   = UsartStatus;                     // Clear flags.
  do {
    if (UsartStatus & US_RXRDY) {                // Data received?
      if (UsartStatus & USART_RX_ERROR_FLAGS) {  // Any error ?
#if (DEBUG != 0)
        _ErrCnt++;
#endif
        Dummy = USART_RDR;                       // => Discard data
        BSP_UART_USE_PARA(Dummy);
      } else {
        if (_pfOnRx) {
#if (DEBUG != 0)
          if (_RxCnt >= sizeof(_RxData)) {
            _RxCnt = 0;
          }
          _RxData[_RxCnt] = (USART_RDR & 0xFFu);
          _pfOnRx(0, (unsigned char)_RxData[_RxCnt]);
          _RxCnt++;
#else
          _pfOnRx(0, (unsigned char)(USART_RDR & 0xFFu));
#endif
        } else {
          Dummy = USART_RDR;                     // => Discard data
        }
      }
    }
    UsartStatus = USART_ISR;                     // Examine current status
  } while (UsartStatus & US_RXRDY);
  if ((UsartStatus & US_TXEMPTY) && ((USART_CR1 & 0x40u) != 0)) {
    if (_pfOnTx) {
      if (_pfOnTx(0)) {                          // No more characters to send ?
        USART_CR1 &= ~0x40u;                     // Disable further tx interrupts
      }
    }
  }
  OS_LeaveNestableInterrupt();
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_UART_Init()
*
* Function description
*    Initializes the UART peripheral.
*
* Parameters
*    Unit       : Unit number (typically zero-based).
*    Baudrate   : Baud rate to configure [Hz].
*    NumDataBits: Number of data bits to use.
*    Parity     : One of the following values:
*                   * BSP_UART_PARITY_NONE
*                   * BSP_UART_PARITY_ODD
*                   * BSP_UART_PARITY_EVEN
*    NumStopBits: Number of stop bits to use.
*/
void BSP_UART_Init(unsigned int Unit, unsigned long Baudrate, unsigned char NumDataBits, unsigned char Parity, unsigned char NumStopBits) {
  unsigned long v;

  BSP_UART_USE_PARA(Unit);
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  if (Baudrate == 0) {
    Baudrate = BSP_UART_BAUDRATE;
  }
  //
  // Initialize clock and port for USART, selection specific
  //
  OS_IncDI();
  RCC_AHB4ENR  |= (1u <<  1);     // GPIO CLK enable
  RCC_APB2ENR  |= (1u <<  4);     // Enable USART1 clock
                                  // GPIOB set alternate function for USART1
  GPIOB_AF_LOW  = (7u << 24);     // - Set PB6  to AF7 (USART1_TX)
  GPIOB_AF_HIGH = (4u << 28);     // - Set PB15 to AF4 (USART1_RX)
                                  // GPIOB alternate function mode
  v             = GPIOB_MODER;
  v            &= ~((3u << 12)
               |    (3u << 30));
  v            |=   (2u << 12)    // - PB6  -> AF
               |    (2u << 30);   // - PB15 -> AF
  GPIOB_MODER   = v;
                                  // GPIOC speed setting
  GPIOB_OSPEEDR = (3u << 12)      // - PB6  high speed
                | (3u << 30);     // - PB15 high speed
  GPIOB_PUPDR   = (1u << 12)      // - PB6  pull-up
                | (1u << 30);     // - PB15 pull-up
  //
  // USART settings
  //
  USART_CR1 = (1u << 2)           // Receiver enable
            | (1u << 3)           // Transmitter enable
            | (1u << 5);          // RX interrupt enable
  _SetBaudrate(Unit, Baudrate);   // Set baudrate
  USART_CR1 |= (1u << 0);         // Enable USART
  NVIC_SetPriority(ISR_ID_USART1, (1u << __NVIC_PRIO_BITS) - 4u);
  NVIC_EnableIRQ(ISR_ID_USART1);
  OS_DecRI();
}

/*********************************************************************
*
*       BSP_UART_DeInit()
*
* Function description
*    De-initializes the UART peripheral.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*/
void BSP_UART_DeInit(unsigned int Unit) {
  BSP_UART_USE_PARA(Unit);
  NVIC_DisableIRQ(ISR_ID_USART1);
  USART_CR1 = 0x00000000u;
  USART_BRR = 0x00000000u;
}

/*********************************************************************
*
*       BSP_UART_SetBaudrate()
*
* Function description
*    Configures/changes the UART baud rate.
*
*  Parameters
*    Unit    : Unit number (typically zero-based).
*    Baudrate: Baud rate to configure [Hz].
*/
void BSP_UART_SetBaudrate(unsigned int Unit, unsigned long Baudrate) {
  _SetBaudrate(Unit, Baudrate);
}

/*********************************************************************
*
*       BSP_UART_SetReadCallback()
*
* Function description
*    Sets the callback to execute upon an Rx interrupt.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    pf  : Callback to execute.
*/
void BSP_UART_SetReadCallback(unsigned Unit, BSP_UART_RX_CB* pf) {
  BSP_UART_USE_PARA(Unit);
  _pfOnRx = pf;
}

/*********************************************************************
*
*       BSP_UART_SetWriteCallback()
*
* Function description
*    Sets the callback to execute upon a Tx interrupt.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    pf  : Callback to execute.
*/
void BSP_UART_SetWriteCallback(unsigned int Unit, BSP_UART_TX_CB* pf) {
  BSP_UART_USE_PARA(Unit);
  _pfOnTx = pf;
}

/*********************************************************************
*
*       BSP_UART_Write1()
*
* Function description
*    Sends one byte via UART.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    Data: (First) data byte to send.
*
*  Additional information
*    The first byte of a transfer is typically sent from application
*    context. Further bytes of the transfer are then sent from the
*    Tx interrupt handler by also calling this function from interrupt
*    context.
*/
void BSP_UART_Write1(unsigned int Unit, unsigned char Data) {
  BSP_UART_USE_PARA(Unit);
#if (DEBUG != 0)
  if (_TxCnt >= sizeof(_TxData)) {
    _TxCnt = 0;
  }
  _TxData[_TxCnt] = Data;
  _Send1Buffer    = _TxData[_TxCnt++];
#endif
  USART_TDR  = Data;
  USART_CR1 |= 0x40;  // Enable TX interrupt
}

/*************************** End of file ****************************/

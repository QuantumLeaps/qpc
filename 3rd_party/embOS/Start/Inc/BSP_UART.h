/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2019 SEGGER Microcontroller GmbH                  *
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
*       OS version: 5.06.1                                           *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
Purpose : Header file for UART related BSP functions.
*/

#ifndef BSP_UART_H
#define BSP_UART_H

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// In order to avoid warnings for unused parameters.
//
#ifndef BSP_UART_USE_PARA
  #if defined(NC30) || defined(NC308)
    #define BSP_UART_USE_PARA(para)
  #else
    #define BSP_UART_USE_PARA(para) para=para;
  #endif
#endif

//
// Data bits used
//
#define BSP_UART_DATA_BITS_8  (8u)
#define BSP_UART_DATA_BITS_9  (9u)

//
// Parity mode used
//
#define BSP_UART_PARITY_NONE  (0u)
#define BSP_UART_PARITY_ODD   (1u)
#define BSP_UART_PARITY_EVEN  (2u)

//
// Stop bits used
//
#define BSP_UART_STOP_BITS_1  (1u)

//
// Compatibility macros for old names.
// Use BSP_UART_* defines in new code.
//
#define UART_PARITY_NONE  BSP_UART_PARITY_NONE
#define UART_PARITY_ODD   BSP_UART_PARITY_ODD
#define UART_PARITY_EVEN  BSP_UART_PARITY_EVEN

/*********************************************************************
*
*       Types, global
*
**********************************************************************
*/

typedef void BSP_UART_RX_CB(unsigned int Unit, unsigned char Data);
typedef int  BSP_UART_TX_CB(unsigned int Unit);

/*********************************************************************
*
*       API functions / Function prototypes
*
**********************************************************************
*/
#if defined(__cplusplus)
  extern "C" {
#endif

void BSP_UART_DeInit          (unsigned int Unit);
void BSP_UART_Init            (unsigned int Unit, unsigned long Baudrate, unsigned char NumDataBits, unsigned char Parity, unsigned char NumStopBits);
void BSP_UART_SetBaudrate     (unsigned int Unit, unsigned long Baudrate);
void BSP_UART_SetReadCallback (unsigned int Unit, void (*pf)(unsigned int Unit, unsigned char Data));
void BSP_UART_SetWriteCallback(unsigned int Unit, int (*pf)(unsigned int Unit));
void BSP_UART_Write1          (unsigned int Unit, unsigned char Data);

#if defined(__cplusplus)
}
#endif

#endif  // BSP_UART_H

/*************************** End of file ****************************/

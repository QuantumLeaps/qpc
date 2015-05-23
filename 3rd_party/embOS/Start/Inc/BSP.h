/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2014  SEGGER Microcontroller GmbH & Co KG         *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
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
*       OS version: 4.00                                             *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : BSP.h
Purpose : BSP (Board support package)
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef BSP_H                           /* avoid multiple inclusion */
#define BSP_H

/*********************************************************************
*
*       Defines, non-configurable
*
**********************************************************************
*/
#define KEY_STAT_UP       (1 << 0)
#define KEY_STAT_DOWN     (1 << 1)
#define KEY_STAT_LEFT     (1 << 2)
#define KEY_STAT_RIGHT    (1 << 3)
#define KEY_STAT_BUTTON1  (1 << 4)
#define KEY_STAT_BUTTON2  (1 << 5)

/* In order to avoid warnings for undefined parameters */
#ifndef BSP_USE_PARA
  #if defined(NC30) || defined(NC308)
    #define BSP_USE_PARA(para)
  #else
    #define BSP_USE_PARA(para) para=para;
  #endif
#endif

/*********************************************************************
*
*       Functions
*
**********************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       General
*/
void     BSP_Init       (void);
void     BSP_SetLED     (int Index);
void     BSP_ClrLED     (int Index);
void     BSP_ToggleLED  (int Index);
unsigned BSP_GetKeyStat (void);

/*********************************************************************
*
*       GUI
*/
void BSP_GUI_Init(void);

/*********************************************************************
*
*       USB
*/
void BSP_USB_Attach         (void);
void BSP_USB_InstallISR     (void (*pfISR)(void));
void BSP_USB_InstallISR_Ex  (int ISRIndex, void (*pfISR)(void), int Prio);
void BSP_USB_ISR_Handler    (void);

/*********************************************************************
*
*       USBH
*/
void BSP_USBH_InstallISR    (void (*pfISR)(void));
void BSP_USBH_Init          (void);

/*********************************************************************
*
*       ETH
*
*  Functions for ethernet controllers (as far as present)
*/
void BSP_ETH_Init          (unsigned Unit);
void BSP_ETH_InstallISR    (void (*pfISR)(void));
void BSP_ETH_InstallISR_Ex (int ISRIndex, void (*pfISR)(void), int Prio);
void BSP_ETH_ISR_Handler   (void);

/*********************************************************************
*
*       CACHE
*/
void BSP_CACHE_CleanInvalidateRange (void * p, unsigned NumBytes);
void BSP_CACHE_CleanRange           (void * p, unsigned NumBytes);
void BSP_CACHE_InvalidateRange      (void * p, unsigned NumBytes);

/********************************************************************/

#ifdef __cplusplus
}
#endif

/********************************************************************/

#endif                                  /* avoid multiple inclusion */

/****** End Of File *************************************************/

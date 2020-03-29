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
Purpose : BSP (Board support package)
*/

#ifndef BSP_H
#define BSP_H

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// In order to avoid warnings for undefined parameters
//
#ifndef BSP_USE_PARA
  #define BSP_USE_PARA(para)  (void) (para)
#endif

#if (defined(__ICCARM__) && (__CPU_MODE__ == 1))  // If IAR and THUMB mode
  #define INTERWORK  __interwork
#else
  #define INTERWORK
#endif

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif

void          BSP_Init        (void);
void          BSP_SetLED      (int Index);
void          BSP_ClrLED      (int Index);
void          BSP_ToggleLED   (int Index);
int           BSP_GetLEDState (int Index);
int           BSP_FPGA_Init   (void);

void          MemoryInit      (void);
INTERWORK int __low_level_init(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_H

/*************************** End of file ****************************/

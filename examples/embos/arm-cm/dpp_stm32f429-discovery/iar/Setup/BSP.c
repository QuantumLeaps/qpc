/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2017 SEGGER Microcontroller GmbH & Co. KG         *
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
*       OS version: 4.34.1                                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for the ST STM32F429 Discovery eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define RCC_BASE_ADDR             ((unsigned int)(0x40023800))
#define RCC_AHB1RSTR              (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x10))
#define RCC_AHBENR                (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30))
#define RCC_LEDPORT_BITS          ((1u << 0u) | (1u << 6u))

#define GPIOG_BASE_ADDR           ((unsigned int)0x40021800)
#define GPIOG_MODER               (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x00))
#define GPIOG_ODR                 (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x14))

#define LED0_BIT                  (13u)  // LED3 (green) - PG13
#define LED1_BIT                  (14u)  // LED4 (red)   - PG14

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  //
  // Initialize port for LEDs (sample application)
  //
  RCC_AHBENR   &= ~RCC_LEDPORT_BITS;
  RCC_AHB1RSTR &= ~RCC_LEDPORT_BITS;
  RCC_AHBENR   |=  RCC_LEDPORT_BITS;

  GPIOG_MODER &= ~(3u << (LED0_BIT * 2));   // Reset mode; sets port to input
  GPIOG_MODER |=  (1u << (LED0_BIT * 2));   // Set to output mode
  GPIOG_ODR   &=  ~(1u << LED0_BIT);        // Initially clear LEDs, low active

  GPIOG_MODER &= ~(3u << (LED1_BIT * 2));   // Reset mode; sets port to input
  GPIOG_MODER |=  (1u << (LED1_BIT * 2));   // Set to output mode
  GPIOG_ODR   &=  ~(1u << LED1_BIT);        // Initially clear LEDs, low active
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOG_ODR &= ~(1u << LED0_BIT);         // Switch on LED0
  } else if (Index == 1) {
    GPIOG_ODR &= ~(1u << LED1_BIT);         // Switch on LED1
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOG_ODR |= (1u << LED0_BIT);          // Switch off LED0
  } else if (Index == 1) {
    GPIOG_ODR |= (1u << LED1_BIT);          // Switch off LED1
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOG_ODR ^= (1u << LED0_BIT);          // Toggle LED0
  } else if (Index == 1) {
    GPIOG_ODR ^= (1u << LED1_BIT);          // Toggle LED1
  }
}

/****** End Of File *************************************************/

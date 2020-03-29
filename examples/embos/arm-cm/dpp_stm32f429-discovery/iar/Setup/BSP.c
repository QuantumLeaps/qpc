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

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for the IAR STM32F429 SK eval board
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

#define GPIOA_BASE_ADDR           ((unsigned int)0x40020000)
#define GPIOA_MODER               (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x00))
#define GPIOA_ODR                 (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x14))

#define GPIOG_BASE_ADDR           ((unsigned int)0x40021800)
#define GPIOG_MODER               (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x00))
#define GPIOG_ODR                 (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x14))

#define LED0_BIT                  (4u)  // LED1 - PA4
#define LED1_BIT                  (3u)  // LED2 - PG3

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

  GPIOA_MODER &= ~(3u << (LED0_BIT * 2));   // Reset mode; sets port to input
  GPIOA_MODER |=  (1u << (LED0_BIT * 2));   // Set to output mode
  GPIOA_ODR   &=  ~(1u << LED0_BIT);        // Initially clear LEDs, low active

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
    GPIOA_ODR &= ~(1u << LED0_BIT);         // Switch on LED0
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
    GPIOA_ODR |= (1u << LED0_BIT);          // Switch off LED0
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
    GPIOA_ODR ^= (1u << LED0_BIT);          // Toggle LED0
  } else if (Index == 1) {
    GPIOG_ODR ^= (1u << LED1_BIT);          // Toggle LED1
  }
}

/****** End Of File *************************************************/

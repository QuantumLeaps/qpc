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

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for STM32H7x3I-Eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define LED1_PIN         (10)
#define LED3_PIN         ( 4)

#define GPIOA_BASE_ADDR  (0x58020000u)
#define GPIOA_MODER      (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x00u))
#define GPIOA_OTYPER     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x04u))
#define GPIOA_OSPEEDR    (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x08u))
#define GPIOA_PUPDR      (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x0Cu))
#define GPIOA_ODR        (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x14u))
#define GPIOA_BSRR       (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x18u))

#define GPIOF_BASE_ADDR  (0x58021400u)
#define GPIOF_MODER      (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x00u))
#define GPIOF_OTYPER     (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x04u))
#define GPIOF_OSPEEDR    (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x08u))
#define GPIOF_PUPDR      (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x0Cu))
#define GPIOF_ODR        (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x14u))
#define GPIOF_BSRR       (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR    (0x58024400u)
#define RCC_AHB4ENR      (*(volatile unsigned int*)(RCC_BASE_ADDR + 0xE0u))

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
  // Enable GPIO clocks
  //
  RCC_AHB4ENR   |= (0x1u << 5)                 // Enable the GPIOF clock
                | (0x1u << 0);                 // Enable the GPIOA clock
  //
  // Initialize LED1
  //
  GPIOF_MODER   &= ~(0x3u << (LED1_PIN * 2));  // Clear mode register
  GPIOF_MODER   |=  (0x1u << (LED1_PIN * 2));  // Set IO direction to output mode
  GPIOF_OSPEEDR |=  (0x3u << (LED1_PIN * 2));  // Set speed to high frequency
  GPIOF_OTYPER  &= ~(0x1u << (LED1_PIN * 1));  // Set output to push-pull
  GPIOF_PUPDR   &= ~(0x3u << (LED1_PIN * 2));  // Clear the pull-up/pull-down register
  GPIOF_PUPDR   |=  (0x1u << (LED1_PIN * 2));  // Set push-pull to pull-up
  GPIOF_BSRR     =  (0x1u << LED1_PIN);        // Turn LED off
  //
  // Initialize LED3
  //
  GPIOA_MODER   &= ~(0x3u << (LED3_PIN * 2));  // Clear mode register
  GPIOA_MODER   |=  (0x1u << (LED3_PIN * 2));  // Set IO direction to output mode
  GPIOA_OSPEEDR |=  (0x3u << (LED3_PIN * 2));  // Set speed to high frequency
  GPIOA_OTYPER  &= ~(0x1u << (LED3_PIN * 1));  // Set output to push-pull
  GPIOA_PUPDR   &= ~(0x3u << (LED3_PIN * 2));  // Clear the pull-up/pull-down register
  GPIOA_PUPDR   |=  (0x1u << (LED3_PIN * 2));  // Set push-pull to pull-up
  GPIOA_BSRR     =  (0x1u << LED3_PIN);        // Turn LED off
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOF_BSRR |= ((0x1u << 16) << LED1_PIN);  // Turn LED on
  } else if (Index == 1) {
    GPIOA_BSRR |= ((0x1u << 16) << LED3_PIN);  // Turn LED on
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOF_BSRR |= (0x1u << LED1_PIN);          // Turn LED off
  } else if (Index == 1) {
    GPIOA_BSRR |= (0x1u << LED3_PIN);          // Turn LED off
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOF_ODR ^= (0x1u << LED1_PIN);           // Toggle LED
  } else if (Index == 1) {
    GPIOA_ODR ^= (0x1u << LED3_PIN);           // Toggle LED
  }
}

/****** End Of File *************************************************/

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
Purpose : BSP for STM32H743I Nucleo board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define LD1_PIN          ( 0)   // LD1, Green Led
#define LD2_PIN          ( 7)   // LD2, Blue Led
#define LD3_PIN          (14)   // LD3, Red Led

#define GPIOB_BASE_ADDR  (0x58020400u)
#define GPIOB_MODER      (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x00u))
#define GPIOB_OTYPER     (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x04u))
#define GPIOB_OSPEEDR    (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x08u))
#define GPIOB_PUPDR      (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x0Cu))
#define GPIOB_ODR        (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x14u))
#define GPIOB_BSRR       (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x18u))

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
  RCC_AHB4ENR   |= (0x01u << 1);                // Enable the GPIOB clock
  //
  // Initialize LD1
  //
  GPIOB_MODER   &= ~(0x3u << (LD1_PIN * 2));    // Clear mode register
  GPIOB_MODER   |=  (0x1u << (LD1_PIN * 2));    // Set IO direction to output mode
  GPIOB_OSPEEDR |=  (0x3u << (LD1_PIN * 2));    // Set speed to high frequency
  GPIOB_OTYPER  &= ~(0x1u << (LD1_PIN * 1));    // Set output to push-pull
  GPIOB_PUPDR   &= ~(0x3u << (LD1_PIN * 2));    // Clear the pull-up/pull-down register
  GPIOB_PUPDR   |=  (0x1u << (LD1_PIN * 2));    // Set push-pull to pull-up
  GPIOB_BSRR     =  ((0x1u << 16) << LD1_PIN);  // Turn LED off
  //
  // Initialize LD2
  //
  GPIOB_MODER   &= ~(0x3u << (LD2_PIN * 2));    // Clear mode register
  GPIOB_MODER   |=  (0x1u << (LD2_PIN * 2));    // Set IO direction to output mode
  GPIOB_OSPEEDR |=  (0x3u << (LD2_PIN * 2));    // Set speed to high frequency
  GPIOB_OTYPER  &= ~(0x1u << (LD2_PIN * 1));    // Set output to push-pull
  GPIOB_PUPDR   &= ~(0x3u << (LD2_PIN * 2));    // Clear the pull-up/pull-down register
  GPIOB_PUPDR   |=  (0x1u << (LD2_PIN * 2));    // Set push-pull to pull-up
  GPIOB_BSRR     =  ((0x1u << 16) << LD2_PIN);  // Turn LED off
  //
  // Initialize LD3
  //
  GPIOB_MODER   &= ~(0x3u << (LD3_PIN * 2));    // Clear mode register
  GPIOB_MODER   |=  (0x1u << (LD3_PIN * 2));    // Set IO direction to output mode
  GPIOB_OSPEEDR |=  (0x3u << (LD3_PIN * 2));    // Set speed to high frequency
  GPIOB_OTYPER  &= ~(0x1u << (LD3_PIN * 1));    // Set output to push-pull
  GPIOB_PUPDR   &= ~(0x3u << (LD3_PIN * 2));    // Clear the pull-up/pull-down register
  GPIOB_PUPDR   |=  (0x1u << (LD3_PIN * 2));    // Set push-pull to pull-up
  GPIOB_BSRR     =  ((0x1u << 16) << LD3_PIN);  // Turn LED off
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOB_BSRR |= (0x1u << LD1_PIN);            // Turn LED on
  } else if (Index == 1) {
    GPIOB_BSRR |= (0x1u << LD2_PIN);            // Turn LED on
  } else if (Index == 2) {
    GPIOB_BSRR |= (0x1u << LD3_PIN);            // Turn LED on
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOB_BSRR |= ((0x1u << 16) << LD1_PIN);    // Turn LED off
  } else if (Index == 1) {
    GPIOB_BSRR |= ((0x1u << 16) << LD2_PIN);    // Turn LED off
  } else if (Index == 2) {
    GPIOB_BSRR |= ((0x1u << 16) << LD3_PIN);    // Turn LED off
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOB_ODR ^= (0x1u << LD1_PIN);             // Toggle LED
  } else if (Index == 1) {
    GPIOB_ODR ^= (0x1u << LD2_PIN);             // Toggle LED
  } else if (Index == 2) {
    GPIOB_ODR ^= (0x1u << LD3_PIN);             // Toggle LED
  }
}

/****** End Of File *************************************************/

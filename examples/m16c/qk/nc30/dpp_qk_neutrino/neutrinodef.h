/***********************************************************************************
FILE NAME    : M16C26ADEF.h

DESCRIPTION  : NEUTRINO M16C26A Specific Definitions

Copyright   : 2006 Renesas Technology Europe Ltd.
Copyright   : 2006 Renesas Technology Corporation.
          All Rights Reserved
***********************************************************************************/

#ifndef NEUTRINODEF_H
#define NEUTRINODEF_H

/* Peripheral Clock Speed */
#define f1_CLK_SPEED        20000000
#define fc_CLK_SPEED        32768

/* Define switches to be polled if not available as interrupts */
/* Switches */
#define SW1                 p9_0
#define SW1_DDR             pd9_0

/* LEDs */
#define LED0                p7_0
#define LED1                p7_2
#define LED2                p7_4
#define LED3                p7_6

#define LED0_DDR            pd7_0
#define LED1_DDR            pd7_2
#define LED2_DDR            pd7_4
#define LED3_DDR            pd7_6

/* Analog Inputs */
#define RC_IN               p10_0
#define CdS_IN              p10_1

/* Filtered Discrete Output */
#define RC_OUT              p8_0

/* Audio Outputs */
#define AUDIO_LEFT          p7_4
#define AUDIO_RIGHT         p7_6

/* Serial Flash interface */
#define FLASH_SCS           p6_0
#define FLASH_SCK           p6_1
#define FLASH_SSO           p6_2
#define FLASH_SSI           p6_3

/* Interrupt Control Macros */
#define ENABLE_IRQ          _asm(" FSET I")
#define DISABLE_IRQ         _asm(" FCLR I")

/* General Values */
#define LEDS_ON             0x00
#define LEDS_OFF            0xFF
#define LED_ON              0
#define LED_OFF             1
#define SET_BIT_HIGH        1
#define SET_BIT_LOW         0
#define SET_BYTE_HIGH       0xFF
#define SET_BYTE_LOW        0x00
#define TRUE                1
#define FALSE               0
#define ON                  1
#define OFF                 0

#endif                                            /* NEUTRINODEF_H included */
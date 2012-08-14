#ifndef TAUDEF_H_INCLUDED
#define TAUDEF_H_INCLUDED

/***********************************************************************************
FILE NAME       TAUdef.h

DESCRIPTION     Defines for R8C25 TAU

Copyright   : 2006 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
10.05.2006 RSO-PIS First Release
***********************************************************************************/


/***********************************************************************************
Defines
***********************************************************************************/

/* Peripheral Clock Speed set up in ConfigureOperatingFrequency() */
#define f1_CLK_SPEED        20000000
#define fc_CLK_SPEED        32768
#define XIN_FREQ            f1_CLK_SPEED

/* Switches */
#define SW1                 p4_5
#define SW1_DDR             pd4_5


/* LEDs */
#define LED0                p2_0
#define LED1                p2_1
#define LED2                p2_2
#define LED3                p2_3

#define LED0_DDR            pd2_0
#define LED1_DDR            pd2_1
#define LED2_DDR            pd2_2
#define LED3_DDR            pd2_3

#define LED_ON              0
#define LED_OFF             1

/* Analog Inputs */
#define RC_IN               p0_7
#define CdS_IN              p0_6

/* Filtered Discrete Output */
#define RC_OUT              p2_4

/* Audio Outputs */
#define AUDIO_LEFT          p2_2
#define AUDIO_RIGHT         p2_3

/* Serial Flash interface */
#define FLASH_SCS           p3_4
#define FLASH_SCK           p3_5
#define FLASH_SSO           p3_7
#define FLASH_SSI           P3_3

/* Common Defines */
#ifndef TRUE
#define TRUE                1
#endif
#ifndef FALSE
#define FALSE               0
#endif

#define SET_BIT_HIGH        1
#define SET_BIT_LOW         0
#define SET_BYTE_HIGH       0xFF
#define SET_BYTE_LOW        0x00

#define ENABLE_IRQ          _asm(" FSET I")
#define DISABLE_IRQ         _asm(" FCLR I")

#endif                                                 /* TAUDEF_H_INCLUDED */

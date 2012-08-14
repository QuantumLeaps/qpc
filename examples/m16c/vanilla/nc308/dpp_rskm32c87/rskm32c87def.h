#ifndef RSKM32C87DEF_H_INCLUDED
#define RSKM32C87DEF_H_INCLUDED

/*****************************************************************************
FILE NAME          rskM32C87def.h

DESCRIPTION        Defines for M32C87 RSK

Copyright   : 2006 Renesas Technology Europe Ltd.
Copyright   : 2006 Renesas Technology Corporation.
All Rights Reserved
*****************************************************************************/

/*****************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
07.07.2005 RTE-MAB First Release
*****************************************************************************/


/*****************************************************************************
Defines
*****************************************************************************/

/* Peripheral Clock Speed set up in ConfigureOperatingFrequency() */
#define f1_CLK_SPEED    30000000
#define fc_CLK_SPEED    32768
#define XIN_FREQ        f1_CLK_SPEED

/* Switches */
#define SW1             p8_2
#define SW2             p8_3
#define SW3             p8_4
#define SW1_DDR         pd8_2
#define SW2_DDR         pd8_3
#define SW3_DDR         pd8_4

/* LEDs */
#define LED0            p4_0
#define LED1            p4_1
#define LED2            p4_2
#define LED3            p4_3
#define LED0_DDR        pd4_0
#define LED1_DDR        pd4_1
#define LED2_DDR        pd4_2
#define LED3_DDR        pd4_3

/* General Values */
#define SET_BIT_HIGH    1
#define SET_BIT_LOW     0

#define LED_ON          0
#define LED_OFF         1

/* Common Defines */
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif

/* LED port settings  */
#define LED_PORT_DR     p4
#define LED_BIT         0x0F
#define ENABLE_IRQ      _asm(" FSET I")
#define DISABLE_IRQ     _asm(" FCLR I")


#endif /* RSKM32C87DEF_H_INCLUDED */

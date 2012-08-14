/*****************************************************************************
FILE NAME      : QUARKDEF.h

DESCRIPTION    : QUARK H8/36077 Specific Definitions

Copyright   : 2006 Renesas Technology Europe Ltd.
Copyright   : 2006 Renesas Technology Corporation.
              All Rights Reserved
*****************************************************************************/
#ifndef QUARKDEF_H
#define QUARKDEF_H
                                                 /* peripheral clock speeds */
#define f1_CLK_SPEED        10000000
#define fc_CLK_SPEED        32768

                                                                /* switches */
#define SW1                 IO.PDR1.BIT.B5
#define SW1_DDR             IO.PCR1
#define SW1_DDR_MASK        0xDF

                                                                    /* LEDs */
#define LED0                IO.PDR6.BIT.B0
#define LED1                IO.PDR6.BIT.B1
#define LED2                IO.PDR6.BIT.B2
#define LED3                IO.PDR6.BIT.B3
#define LED0_DDR_1()        (IO.PCR6 |= 0x01)
#define LED1_DDR_1()        (IO.PCR6 |= 0x02)
#define LED2_DDR_1()        (IO.PCR6 |= 0x04)
#define LED3_DDR_1()        (IO.PCR6 |= 0x08)


#define LED_ON              0
#define LED_OFF             1
#define SET_BIT_HIGH        1
#define SET_BIT_LOW         0

#endif                                                        /* QUARKDEF_H */
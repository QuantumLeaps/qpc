/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Definitions & Register
 *
 */

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Variable types                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */

#define Uint32  unsigned long
#define Uint16  unsigned short
#define Uint8   unsigned char
#define Int32   int
#define Int16   short
#define Int8    char

#define SW_BREAKPOINT      while(1);
/* ------------------------------------------------------------------------ *
 *  System Module                                                           *
 * ------------------------------------------------------------------------ */
#define SYS_EXBUSSEL       *(volatile ioport Uint16*)(0x1c00)
#define SYS_PCGCR1         *(volatile ioport Uint16*)(0x1c02)
#define SYS_PCGCR2         *(volatile ioport Uint16*)(0x1c03)
#define SYS_PRCNTR         *(volatile ioport Uint16*)(0x1c04)
#define SYS_PRCNTRLR       *(volatile ioport Uint16*)(0x1c05)
#define SYS_GPIO_DIR0      *(volatile ioport Uint16*)(0x1c06)
#define SYS_GPIO_DIR1      *(volatile ioport Uint16*)(0x1c07)
#define SYS_GPIO_DATAIN0   *(volatile ioport Uint16*)(0x1c08)
#define SYS_GPIO_DATAIN1   *(volatile ioport Uint16*)(0x1c09)
#define SYS_GPIO_DATAOUT0  *(volatile ioport Uint16*)(0x1c0a)
#define SYS_GPIO_DATAOUT1  *(volatile ioport Uint16*)(0x1c0b)
#define SYS_OUTDRSTR       *(volatile ioport Uint16*)(0x1c16)
#define SYS_SPPDIR         *(volatile ioport Uint16*)(0x1c17)

/* ------------------------------------------------------------------------ *
 *  I2C Module                                                              *
 * ------------------------------------------------------------------------ */

#define I2C_IER               *(volatile ioport Uint16*)(0x1A04)
#define I2C_STR               *(volatile ioport Uint16*)(0x1A08)
#define I2C_CLKL           *(volatile ioport Uint16*)(0x1A0C)
#define I2C_CLKH           *(volatile ioport Uint16*)(0x1A10)
#define I2C_CNT               *(volatile ioport Uint16*)(0x1A14)
#define I2C_DRR               *(volatile ioport Uint16*)(0x1A18)
#define I2C_SAR               *(volatile ioport Uint16*)(0x1A1C)
#define I2C_DXR               *(volatile ioport Uint16*)(0x1A20)
#define I2C_MDR            *(volatile ioport Uint16*)(0x1A24)
#define I2C_EDR               *(volatile ioport Uint16*)(0x1A2C)
#define I2C_PSC               *(volatile ioport Uint16*)(0x1A30)
/* ------------------------------------------------------------------------ *
 *  I2S Module                                                              *
 * ------------------------------------------------------------------------ */
#define I2S0_CR            *(volatile ioport Uint16*)(0x2800)
#define I2S0_SRGR          *(volatile ioport Uint16*)(0x2804)
#define I2S0_W0_LSW_W      *(volatile ioport Uint16*)(0x2808)
#define I2S0_W0_MSW_W      *(volatile ioport Uint16*)(0x2809)
#define I2S0_W1_LSW_W      *(volatile ioport Uint16*)(0x280C)
#define I2S0_W1_MSW_W      *(volatile ioport Uint16*)(0x280D)
#define I2S0_IR            *(volatile ioport Uint16*)(0x2810)
#define I2S0_ICMR          *(volatile ioport Uint16*)(0x2814)
#define I2S0_W0_LSW_R      *(volatile ioport Uint16*)(0x2828)
#define I2S0_W0_MSW_R      *(volatile ioport Uint16*)(0x2829)
#define I2S0_W1_LSW_R      *(volatile ioport Uint16*)(0x282C)
#define I2S0_W1_MSW_R      *(volatile ioport Uint16*)(0x282D)
/* I2S2 */
#define I2S2_CR            *(volatile ioport Uint16*)(0x2A00)
#define I2S2_SRGR          *(volatile ioport Uint16*)(0x2A04)
#define I2S2_W0_LSW_W      *(volatile ioport Uint16*)(0x2A08)
#define I2S2_W0_MSW_W      *(volatile ioport Uint16*)(0x2A09)
#define I2S2_W1_LSW_W      *(volatile ioport Uint16*)(0x2A0C)
#define I2S2_W1_MSW_W      *(volatile ioport Uint16*)(0x2A0D)
#define I2S2_IR            *(volatile ioport Uint16*)(0x2A10)
#define I2S2_ICMR          *(volatile ioport Uint16*)(0x2A14)
#define I2S2_W0_LSW_R      *(volatile ioport Uint16*)(0x2A28)
#define I2S2_W0_MSW_R      *(volatile ioport Uint16*)(0x2A29)
#define I2S2_W1_LSW_R      *(volatile ioport Uint16*)(0x2A2C)
#define I2S2_W1_MSW_R      *(volatile ioport Uint16*)(0x2A2D)

/* ------------------------------------------------------------------------ *
 *  UART Module                                                             *
 * ------------------------------------------------------------------------ */
#define UART_RBR           *(volatile ioport Uint16*)(0x1B00)
#define UART_THR           *(volatile ioport Uint16*)(0x1B00)
#define UART_IER           *(volatile ioport Uint16*)(0x1B02)
#define UART_IIR           *(volatile ioport Uint16*)(0x1B04)
#define UART_FCR           *(volatile ioport Uint16*)(0x1B04)
#define UART_LCR           *(volatile ioport Uint16*)(0x1B06)
#define UART_MCR           *(volatile ioport Uint16*)(0x1B08)
#define UART_LSR           *(volatile ioport Uint16*)(0x1B0A)
#define UART_SCR           *(volatile ioport Uint16*)(0x1B0E)
#define UART_DLL           *(volatile ioport Uint16*)(0x1B10)
#define UART_DLH           *(volatile ioport Uint16*)(0x1B12)
#define UART_PWREMU_MGMT   *(volatile ioport Uint16*)(0x1B18)

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
/* Board Initialization */
Int16 USBSTK5515_init( );

/* Wait Functions */
void USBSTK5515_wait( Uint32 delay );
void USBSTK5515_waitusec( Uint32 usec );

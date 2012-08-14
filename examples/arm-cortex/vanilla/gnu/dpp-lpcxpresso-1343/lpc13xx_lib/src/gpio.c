/*****************************************************************************
 *   gpio.c:  GPIO C file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.12.09  ver 1.01    GPIOSetValue() updated
 *   2008.07.20  ver 1.00    Preliminary version, first Release
 *
*****************************************************************************/
#include "LPC13xx.h"            /* LPC13xx Peripheral Registers */
#include "gpio.h"

/* ===================
 * CodeRed - Modified file to extract out interrupt handler related code,
 * which is really application project specific.
 * Set GPIO_GENERIC_INTS 1 to 1 to reenable original code.
 * =================== */
//#define GPIO_GENERIC_INTS 1

#ifdef GPIO_GENERIC_INTS
volatile uint32_t gpio0_counter = 0;
volatile uint32_t gpio1_counter = 0;
volatile uint32_t gpio2_counter = 0;
volatile uint32_t gpio3_counter = 0;
volatile uint32_t p0_1_counter  = 0;
volatile uint32_t p1_1_counter  = 0;
volatile uint32_t p2_1_counter  = 0;
volatile uint32_t p3_1_counter  = 0;

/*****************************************************************************
** Function name:        PIOINT0_IRQHandler
**
** Descriptions:        Use one GPIO pin(port0 pin1) as interrupt source
**
** parameters:            None
** Returned value:        None
**
*****************************************************************************/
void PIOINT0_IRQHandler(void)
{
  uint32_t regVal;

  gpio0_counter++;
  regVal = GPIOIntStatus( PORT0, 1 );
  if ( regVal )
  {
    p0_1_counter++;
    GPIOIntClear( PORT0, 1 );
  }
  return;
}

/*****************************************************************************
** Function name:        PIOINT1_IRQHandler
**
** Descriptions:        Use one GPIO pin(port1 pin1) as interrupt source
**
** parameters:            None
** Returned value:        None
**
*****************************************************************************/
void PIOINT1_IRQHandler(void)
{
  uint32_t regVal;

  gpio1_counter++;
  regVal = GPIOIntStatus( PORT1, 1 );
  if ( regVal )
  {
    p1_1_counter++;
    GPIOIntClear( PORT1, 1 );
  }
  return;
}

/*****************************************************************************
** Function name:        PIOINT2_IRQHandler
**
** Descriptions:        Use one GPIO pin(port2 pin1) as interrupt source
**
** parameters:            None
** Returned value:        None
**
*****************************************************************************/
void PIOINT2_IRQHandler(void)
{
  uint32_t regVal;

  gpio2_counter++;
  regVal = GPIOIntStatus( PORT2, 1 );
  if ( regVal )
  {
    p2_1_counter++;
    GPIOIntClear( PORT2, 1 );
  }
  return;
}

/*****************************************************************************
** Function name:        PIOINT3_IRQHandler
**
** Descriptions:        Use one GPIO pin(port3 pin1) as interrupt source
**
** parameters:            None
** Returned value:        None
**
*****************************************************************************/
void PIOINT3_IRQHandler(void)
{
  uint32_t regVal;

  gpio3_counter++;
  regVal = GPIOIntStatus( PORT3, 1 );
  if ( regVal )
  {
    p3_1_counter++;
    GPIOIntClear( PORT3, 1 );
  }
  return;
}
#endif //GPIO_GENERIC_INTS

/*****************************************************************************
** Function name:        GPIOInit
**
** Descriptions:        Initialize GPIO, install the
**                        GPIO interrupt handler
**
** parameters:            None
** Returned value:        true or false, return false if the VIC table
**                        is full and GPIO interrupt handler can be
**                        installed.
**
*****************************************************************************/
void GPIOInit( void )
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

#ifdef __JTAG_DISABLED
  LPC_IOCON->JTAG_TDO_PIO1_1  &= ~0x07;
  LPC_IOCON->JTAG_TDO_PIO1_1  |= 0x01;
#endif

  /* Set up NVIC when I/O pins are configured as external interrupts. */
  NVIC_EnableIRQ(EINT0_IRQn);
  NVIC_EnableIRQ(EINT1_IRQn);
  NVIC_EnableIRQ(EINT2_IRQn);
  NVIC_EnableIRQ(EINT3_IRQn);
  return;
}

/*****************************************************************************
** Function name:        GPIOSetDir
**
** Descriptions:        Set the direction in GPIO port
**
** parameters:            port num, bit position, direction (1 out, 0 input)
** Returned value:        None
**
*****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  /* if DIR is OUT(1), but GPIOx_DIR is not set, set DIR
  to OUT(1); if DIR is IN(0), but GPIOx_DIR is set, clr
  DIR to IN(0). All the other cases are ignored.
  On port3(bit 0 through 3 only), no error protection if
  bit value is out of range. */
  switch ( portNum )
  {
    case PORT0:
      if ( !(LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 1) )
        LPC_GPIO0->DIR |= (0x1<<bitPosi);
      else if ( (LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 0) )
        LPC_GPIO0->DIR &= ~(0x1<<bitPosi);
    break;
     case PORT1:
      if ( !(LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 1) )
        LPC_GPIO1->DIR |= (0x1<<bitPosi);
      else if ( (LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 0) )
        LPC_GPIO1->DIR &= ~(0x1<<bitPosi);
    break;
    case PORT2:
      if ( !(LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 1) )
        LPC_GPIO2->DIR |= (0x1<<bitPosi);
      else if ( (LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 0) )
        LPC_GPIO2->DIR &= ~(0x1<<bitPosi);
    break;
    case PORT3:
      if ( !(LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 1) )
        LPC_GPIO3->DIR |= (0x1<<bitPosi);
      else if ( (LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 0) )
        LPC_GPIO3->DIR &= ~(0x1<<bitPosi);
    break;
    default:
      break;
  }
  return;
}

/*****************************************************************************
** Function name:        GPIOSetValue
**
** Descriptions:        Set/clear a bitvalue in a specific bit position
**                        in GPIO portX(X is the port number.)
**
** parameters:            port num, bit position, bit value
** Returned value:        None
**
*****************************************************************************/
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
    // Check bitVal is a binary value - 0 or 1
    if (bitVal <2 )
    {
    /* The MASKED_ACCESS registers give the ability to write to a specific bit
     * (or bits) within the GPIO data register. See the LPC11/13 user manual
     * for more details.
     *
     * (1<<bitPosi) gives us the MASKED_ACCESS register specific to the bit
     * that is being requested to be set or cleared.
     *
     * (bitVal<<bitPosi) will be either be 0 or will contain a 1 in the
     * appropriate bit position that matches the MASKED_ACCESS register
     * being written to.
     */
      switch ( portNum )
      {
        case PORT0:
                 LPC_GPIO0->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
        break;
        case PORT1:
                 LPC_GPIO1->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
        break;
        case PORT2:
                 LPC_GPIO2->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
        break;
        case PORT3:
                 LPC_GPIO3->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
        break;
        default:
          break;
      }
    }
  return;
}

/*****************************************************************************
** Function name:        GPIOSetInterrupt
**
** Descriptions:        Set interrupt sense, event, etc.
**                        edge or level, 0 is edge, 1 is level
**                        single or double edge, 0 is single, 1 is double
**                        active high or low, etc.
**
** parameters:            port num, bit position, sense, single/doube, polarity
** Returned value:        None
**
*****************************************************************************/
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t sense,
            uint32_t single, uint32_t event )
{
  switch ( portNum )
  {
    case PORT0:
      if ( sense == 0 )
      {
        LPC_GPIO0->IS &= ~(0x1<<bitPosi);
        /* single or double only applies when sense is 0(edge trigger). */
        if ( single == 0 )
          LPC_GPIO0->IBE &= ~(0x1<<bitPosi);
        else
          LPC_GPIO0->IBE |= (0x1<<bitPosi);
      }
      else
          LPC_GPIO0->IS |= (0x1<<bitPosi);
      if ( event == 0 )
        LPC_GPIO0->IEV &= ~(0x1<<bitPosi);
      else
        LPC_GPIO0->IEV |= (0x1<<bitPosi);
    break;
     case PORT1:
      if ( sense == 0 )
      {
        LPC_GPIO1->IS &= ~(0x1<<bitPosi);
        /* single or double only applies when sense is 0(edge trigger). */
        if ( single == 0 )
          LPC_GPIO1->IBE &= ~(0x1<<bitPosi);
        else
          LPC_GPIO1->IBE |= (0x1<<bitPosi);
      }
      else
          LPC_GPIO1->IS |= (0x1<<bitPosi);
      if ( event == 0 )
        LPC_GPIO1->IEV &= ~(0x1<<bitPosi);
      else
        LPC_GPIO1->IEV |= (0x1<<bitPosi);
    break;
    case PORT2:
      if ( sense == 0 )
      {
        LPC_GPIO2->IS &= ~(0x1<<bitPosi);
        /* single or double only applies when sense is 0(edge trigger). */
        if ( single == 0 )
          LPC_GPIO2->IBE &= ~(0x1<<bitPosi);
        else
          LPC_GPIO2->IBE |= (0x1<<bitPosi);
      }
      else
          LPC_GPIO2->IS |= (0x1<<bitPosi);
      if ( event == 0 )
        LPC_GPIO2->IEV &= ~(0x1<<bitPosi);
      else
        LPC_GPIO2->IEV |= (0x1<<bitPosi);
    break;
    case PORT3:
      if ( sense == 0 )
      {
        LPC_GPIO3->IS &= ~(0x1<<bitPosi);
        /* single or double only applies when sense is 0(edge trigger). */
        if ( single == 0 )
          LPC_GPIO3->IBE &= ~(0x1<<bitPosi);
        else
          LPC_GPIO3->IBE |= (0x1<<bitPosi);
      }
      else
          LPC_GPIO3->IS |= (0x1<<bitPosi);
      if ( event == 0 )
        LPC_GPIO3->IEV &= ~(0x1<<bitPosi);
      else
        LPC_GPIO3->IEV |= (0x1<<bitPosi);
    break;
    default:
      break;
  }
  return;
}

/*****************************************************************************
** Function name:        GPIOIntEnable
**
** Descriptions:        Enable Interrupt Mask for a port pin.
**
** parameters:            port num, bit position
** Returned value:        None
**
*****************************************************************************/
void GPIOIntEnable( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
    case PORT0:
      LPC_GPIO0->IE |= (0x1<<bitPosi);
    break;
     case PORT1:
      LPC_GPIO1->IE |= (0x1<<bitPosi);
    break;
    case PORT2:
      LPC_GPIO2->IE |= (0x1<<bitPosi);
    break;
    case PORT3:
      LPC_GPIO3->IE |= (0x1<<bitPosi);
    break;
    default:
      break;
  }
  return;
}

/*****************************************************************************
** Function name:        GPIOIntDisable
**
** Descriptions:        Disable Interrupt Mask for a port pin.
**
** parameters:            port num, bit position
** Returned value:        None
**
*****************************************************************************/
void GPIOIntDisable( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
    case PORT0:
      LPC_GPIO0->IE &= ~(0x1<<bitPosi);
    break;
     case PORT1:
      LPC_GPIO1->IE &= ~(0x1<<bitPosi);
    break;
    case PORT2:
      LPC_GPIO2->IE &= ~(0x1<<bitPosi);
    break;
    case PORT3:
      LPC_GPIO3->IE &= ~(0x1<<bitPosi);
    break;
    default:
      break;
  }
  return;
}

/*****************************************************************************
** Function name:        GPIOIntStatus
**
** Descriptions:        Get Interrupt status for a port pin.
**
** parameters:            port num, bit position
** Returned value:        None
**
*****************************************************************************/
uint32_t GPIOIntStatus( uint32_t portNum, uint32_t bitPosi )
{
  uint32_t regVal = 0;

  switch ( portNum )
  {
    case PORT0:
      if ( LPC_GPIO0->MIS & (0x1<<bitPosi) )
        regVal = 1;
    break;
     case PORT1:
      if ( LPC_GPIO1->MIS & (0x1<<bitPosi) )
        regVal = 1;
    break;
    case PORT2:
      if ( LPC_GPIO2->MIS & (0x1<<bitPosi) )
        regVal = 1;
    break;
    case PORT3:
      if ( LPC_GPIO3->MIS & (0x1<<bitPosi) )
        regVal = 1;
    break;
    default:
      break;
  }
  return ( regVal );
}

/*****************************************************************************
** Function name:        GPIOIntClear
**
** Descriptions:        Clear Interrupt for a port pin.
**
** parameters:            port num, bit position
** Returned value:        None
**
*****************************************************************************/
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
    case PORT0:
      LPC_GPIO0->IC |= (0x1<<bitPosi);
    break;
     case PORT1:
      LPC_GPIO1->IC |= (0x1<<bitPosi);
    break;
    case PORT2:
      LPC_GPIO2->IC |= (0x1<<bitPosi);
    break;
    case PORT3:
      LPC_GPIO3->IC |= (0x1<<bitPosi);
    break;
    default:
      break;
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

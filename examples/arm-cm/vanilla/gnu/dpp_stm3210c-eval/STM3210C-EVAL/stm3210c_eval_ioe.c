/**
  ******************************************************************************
  * @file    stm3210c_eval_ioe.c
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   This file includes the IO Expander driver for STMPE811 IO Expander
  *          devices.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

  /* File Info : ---------------------------------------------------------------
    SUPPORTED FEATURES:
      - IO Read/write : Set/Reset and Read (Polling/Interrupt)
      - Joystick: config and Read (Polling/Interrupt)
      - Touch Screen Features: Single point mode (Polling/Interrupt)
      - TempSensor Feature: accuracy not determined (Polling).

    UNSUPPORTED FEATURES:
      - Row ADC Feature is not supported (not implemented on STM3210C-EVAL board)
  ----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm3210c_eval_ioe.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup STM3210C_EVAL_IOE
  * @brief This file includes the IO Expander driver for STMPE811 IO Expander
  *        devices.
  * @{
  */

/** @defgroup EVAL_IOE_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup EVAL_IOE_Private_Defines
  * @{
  */
#define TIMEOUT_MAX    0xFFF; /*<! The value of the maximal timeout for I2C waiting loops */
/**
  * @}
  */


/** @defgroup EVAL_IOE_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup EVAL_IOE_Private_Variables
  * @{
  */
TS_STATE TS_State;              /*<! The global structure holding the TS state */

uint32_t TimeOut = TIMEOUT_MAX; /*<! Value of Timeout when I2C communication fails */
/**
  * @}
  */


/** @defgroup EVAL_IOE_Private_FunctionPrototypes
  * @{
  */
static uint16_t IOE_TS_Read_X(void);
static uint16_t IOE_TS_Read_Y(void);
static uint16_t IOE_TS_Read_Z(void);

static void IOE_GPIO_Config(void);
static void IOE_I2C_Config(void);
static void IOE_EXTI_Config(void);

#ifndef USE_Delay
static void delay(__IO uint32_t nCount);
#endif /* USE_Delay*/
/**
  * @}
  */


/** @defgroup EVAL_IOE_Private_Functions
  * @{
  */


/**
  * @brief  Initializes and Configures the two IO_Expanders Functionalities
  *         (IOs, Touch Screen ..) and configures all STM3210C-EVAL necessary
  *         hardware (GPIOs, APB clocks ..).
  * @param  None
  * @retval IOE_OK if all initializations done correctly. Other value if error.
  */
uint8_t IOE_Config(void)
{
  /* Configure the needed pins */
  IOE_GPIO_Config();

  /* Read IO Expander 1 ID  */
  if(IOE_IsOperational(IOE_1_ADDR))
  {
    return IOE1_NOT_OPERATIONAL;
  }
  if(IOE_IsOperational(IOE_2_ADDR))
  {
    return IOE2_NOT_OPERATIONAL;
  }

  /* Generate IOExpander Software reset */
  IOE_Reset(IOE_1_ADDR);
  IOE_Reset(IOE_2_ADDR);

  /* ---------------------- IO Expander 1 configuration --------------------- */
  /* Enable the GPIO, Touch Screen and ADC functionalities */
  IOE_FnctCmd(IOE_1_ADDR, IOE_IO_FCT | IOE_TS_FCT | IOE_ADC_FCT, ENABLE);
  /* Configure the VBAT pin in output mode pin*/
  IOE_IOPinConfig(IOE_1_ADDR, VBAT_DIV_PIN , Direction_OUT);
  /* ENABLE the alternate function for IN1 pin */
  IOE_IOAFConfig(IOE_1_ADDR, VBAT_DIV_PIN, ENABLE);

  /* Apply the default state for the out pins */
  IOE_WriteIOPin(VBAT_DIV_PIN, BitReset);
  /* Configure the MEMS interrupt pins in Input mode */
  IOE_IOPinConfig(IOE_2_ADDR, (uint32_t)(MEMS_INT1_PIN | MEMS_INT2_PIN), Direction_IN);

  /* ENABLE the alternate function for the Joystick pins */
  IOE_IOAFConfig(IOE_2_ADDR, (uint32_t)(MEMS_INT1_PIN | MEMS_INT2_PIN), ENABLE);
  /* Configure the IOs to detect Falling and Rising Edges */
  IOE_IOEdgeConfig(IOE_2_ADDR, (uint32_t)(MEMS_INT1_PIN | MEMS_INT2_PIN), (uint32_t)(EDGE_FALLING | EDGE_RISING));
  /* Touch Screen controller configuration */
  IOE_TS_Config();

  /* ------------------------------------------------------------------------ */

  /* ---------------------- IO Expander 2 configuration --------------------- */
  /* Enable the GPIO, Temperature Sensor and ADC functionalities */
  IOE_FnctCmd(IOE_2_ADDR, IOE_IO_FCT | IOE_TEMPSENS_FCT | IOE_ADC_FCT, ENABLE);
  /* Configure the Audio Codec Reset pin in output mode pin*/
  IOE_IOPinConfig(IOE_2_ADDR, (uint32_t)(AUDIO_RESET_PIN | MII_INT_PIN), Direction_OUT);
  /* ENABLE the alternate function for IN1 pin */
  IOE_IOAFConfig(IOE_2_ADDR, (uint32_t)(AUDIO_RESET_PIN | MII_INT_PIN), ENABLE);
  /* Apply the default state for the out pins */
  IOE_WriteIOPin(AUDIO_RESET_PIN, BitReset);
  IOE_WriteIOPin(MII_INT_PIN, BitReset);
  /* Configure the Joystick pins in Input mode */
  IOE_IOPinConfig(IOE_2_ADDR, JOY_IO_PINS , Direction_IN);

  /* ENABLE the alternate function for the Joystick pins */
  IOE_IOAFConfig(IOE_2_ADDR, JOY_IO_PINS, ENABLE);
  /* Configure the IOs to detect Falling and Rising Edges */
  IOE_IOEdgeConfig(IOE_2_ADDR, JOY_IO_PINS, (uint8_t)(EDGE_FALLING | EDGE_RISING));

  /* Temperature Sensor module configuration */
  IOE_TempSens_Config();
  /* ------------------------------------------------------------------------ */

  /* Configuration is OK */
  return IOE_OK;
}

/**
  * @brief  Configures The selected interrupts on the IO Expanders.
  * @param  IOE_ITSRC_Source: the source of the interrupts. Could be one or a
  *         combination of the following parameters:
  *   @arg  IOE_ITSRC_JOYSTICK: Joystick IO intputs.
  *   @arg  IOE_ITSRC_TSC: Touch Screen interrupts.
  *   @arg  IOE_ITSRC_INMEMS: MEMS interrupt lines.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_ITConfig(uint32_t IOE_ITSRC_Source)
{
  /* Configure the Interrupt output pin to generate low level (INT_CTRL) */
  IOE_ITOutConfig(Polarity_High, Type_Level);

  /* Manage the Joystick Interrupts */
  if (IOE_ITSRC_Source & IOE_ITSRC_JOYSTICK)
  {
    /* Enable the Global interrupt */
    IOE_GITCmd(IOE_2_ADDR, ENABLE);

    /* Enable the Joystick pins to generate interrupt (GPIO_INT_EN) */
    IOE_IOITConfig(IOE_2_ADDR, IOE_JOY_IT, ENABLE);

    /* Enable the Global GPIO Interrupt */
    IOE_GITConfig(IOE_2_ADDR, IOE_GIT_GPIO, ENABLE);

    /* Read the GPIO_IT_STA to clear all pending bits if any */
    I2C_ReadDeviceRegister(IOE_2_ADDR, IOE_REG_GPIO_INT_STA);

    /* Enable the Joystick pins to generate interrupt */
    IOE_IOITConfig(IOE_2_ADDR, IOE_JOY_IT, ENABLE);

    /* Read the GPIO_IT_STA to clear all pending bits if any */
    I2C_ReadDeviceRegister(IOE_2_ADDR, IOE_REG_GPIO_INT_STA);
  }

  /* Manage the MEMS Interrupts lines  */
  if (IOE_ITSRC_Source & IOE_ITSRC_INMEMS)
  {
    /* Enable the Global interrupt */
    IOE_GITCmd(IOE_1_ADDR, ENABLE);

    /* Enable the pins to generate interrupt (GPIO_INT_EN) */
    IOE_IOITConfig(IOE_1_ADDR, IOE_INMEMS_IT, ENABLE);

    /* Enable the Global GPIO Interrupt */
    IOE_GITConfig(IOE_1_ADDR, IOE_GIT_GPIO, ENABLE);

    /* Read the GPIO_IT_STA to clear all pending bits if any */
    I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_INT_STA);

    /* Enable the pins to generate interrupt */
    IOE_IOITConfig(IOE_1_ADDR, IOE_INMEMS_IT, ENABLE);

    /* Read the GPIO_IT_STA to clear all pending bits if any */
    I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_INT_STA);
  }

  /* Manage the Touch Screen Interrupts */
  if (IOE_ITSRC_Source & IOE_ITSRC_TSC)
  {
    /* Enable the Global interrupt */
    IOE_GITCmd(IOE_1_ADDR, ENABLE);

    /* Enable the Global GPIO Interrupt */
    IOE_GITConfig(IOE_1_ADDR, (uint8_t)(IOE_GIT_TOUCH | IOE_GIT_FTH | IOE_GIT_FOV), ENABLE);

    /* Read the GPIO_IT_STA to clear all pending bits if any */
    I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_INT_STA);
  }

  /* Configure the Interrupt line as EXTI source */
  IOE_EXTI_Config();

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Writes a bit value to an output IO pin.
  * @param IO_Pin: The output pin to be set or reset. This parameter can be one
  *        of the following values:
  *   @arg  AUDIO_RESET_PIN: Audio codec reset pin
  *   @arg  MII_INT_PIN: Ethernet Phy MII interrupt pin
  *   @arg  VBAT_DIV_PIN: Battery devider pin
  * @param BitVal: The value to be set. This parameter can be one of the
  *        following values: BitSet or BitReset. See IOE_BitVal_TypeDef.
  * @retval IOE_OK or PARAM_ERROR
  */
uint8_t IOE_WriteIOPin(uint8_t IO_Pin, IOE_BitValue_TypeDef BitVal)
{
  uint8_t DeviceAddr = 0;

  /* Get the IO expander Address according to which pin is to be controlled */
  if (IO_Pin & IO1_OUT_ALL_PINS)
  {
    DeviceAddr = IOE_1_ADDR;
  }
  else if (IO_Pin & IO2_OUT_ALL_PINS)
  {
    DeviceAddr = IOE_2_ADDR;
  }
  else
  {
    return PARAM_ERROR;
  }

  /* Apply the bit value to the selected pin */
  if (BitVal == BitReset)
  {
    /* Set the register */
    I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_CLR_PIN, IO_Pin);
  }
  else
  {
    /* Set the register */
    I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_SET_PIN, IO_Pin);
  }

  return IOE_OK;
}


/**
  * @brief  Returns the status of the selected input IO pin.
  * @param IO_Pin: The input pin to be read. This parameter can be one
  *  of the following values:
  *   @arg  MEMS_INT1_PIN: MEMS interrupt line 1.
  *   @arg  MEMS_INT2_PIN: MEMS interrupt line 2.
  *   @arg  JOY_IO_PINS: Joystick IO pins (use IOE_JoyStickGetState for these pins)
  * @retval None
  */
uint8_t IOE_ReadIOPin(uint32_t IO_Pin)
{
  uint8_t DeviceAddr = 0;
  uint8_t tmp = 0;
  if (IO_Pin & IO1_IN_ALL_PINS)
  {
    DeviceAddr = IOE_1_ADDR;
  }
  else if (IO_Pin & IO2_IN_ALL_PINS)
  {
    DeviceAddr = IOE_2_ADDR;
  }
  else
  {
    return PARAM_ERROR;
  }

  /* Get all the Pins status */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_MP_STA);
  if ((tmp & (uint8_t)IO_Pin) != 0)
  {
    return BitSet;
  }
  else
  {
    return BitReset;
  }
}


/**
  * @brief  Returns the current Joystick status.
  * @param  None
  * @retval The code of the Joystick key pressed:
  *   @arg  JOY_NONE
  *   @arg  JOY_IO_CENTER
  *   @arg  JOY_DOWN
  *   @arg  JOY_LEFT
  *   @arg  JOY_RIGHT
  *   @arg  JOY_UP
  */
JOY_State_TypeDef IOE_JoyStickGetState(void)
{
  uint8_t tmp = 0;
  /* Read the status of all pins */
  tmp = (uint32_t)I2C_ReadDeviceRegister(IOE_2_ADDR, IOE_REG_GPIO_MP_STA);

  /* Check the pressed keys */
  if ((tmp & JOY_IO_NONE) == JOY_IO_NONE)
  {
    return (JOY_State_TypeDef)JOY_NONE;
  }
  else if (!(tmp & JOY_IO_CENTER))
  {
    return (JOY_State_TypeDef)JOY_CENTER;
  }
  else if (!(tmp & JOY_IO_DOWN))
  {
    return (JOY_State_TypeDef)JOY_DOWN;
  }
  else if (!(tmp & JOY_IO_LEFT))
  {
    return (JOY_State_TypeDef)JOY_LEFT;
  }
  else if (!(tmp & JOY_IO_RIGHT))
  {
    return (JOY_State_TypeDef)JOY_RIGHT;
  }
  else if (!(tmp & JOY_IO_UP))
  {
    return (JOY_State_TypeDef)JOY_UP;
  }
  else
  {
    return (JOY_State_TypeDef)JOY_NONE;
  }
}

/**
  * @brief  Returns Status and positions of the Touch screen.
  * @param  None
  * @retval Pointer to TS_STATE structure holding Touch Screen information.
  */
TS_STATE* IOE_TS_GetState(void)
{
  uint32_t xDiff, yDiff , x , y;
  static uint32_t _x = 0, _y = 0;

  /* Check if the Touch detect event happenned */
  TS_State.TouchDetected = (I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_CTRL) & 0x80);
  if(TS_State.TouchDetected)
  {
    x = IOE_TS_Read_X();
    y = IOE_TS_Read_Y();
    xDiff = x > _x? (x - _x): (_x - x);
    yDiff = y > _y? (y - _y): (_y - y);
    if (xDiff + yDiff > 5)
    {
      _x = x;
      _y = y;
    }
  }
  /* Update the X position */
  TS_State.X = _x;

  /* Update the Y position */
  TS_State.Y = _y;
  /* Update the Z Pression index */
  TS_State.Z = IOE_TS_Read_Z();

  /* Clear the interrupt pending bit and enable the FIFO again */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x01);
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x00);

  /* Return pointer to the updated structure */
  return &TS_State;
}

/**
  * @brief  Returns the temperature row value (in 16 bit format).
  * @param  None
  * @retval The temperature row value.
  */
uint32_t IOE_TempSens_GetData(void)
{
  static __IO uint32_t tmp = 0;

  /* Aquire data enable */
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_TEMP_CTRL, 0x03);

  /* Enable the TEMPSENS module */
  tmp = (uint32_t)((I2C_ReadDeviceRegister(IOE_2_ADDR, IOE_REG_TEMP_DATA) & 0x03) << 8);
  tmp |= (uint32_t)I2C_ReadDeviceRegister(IOE_2_ADDR, IOE_REG_TEMP_DATA + 1);

  tmp = (uint32_t)((33 * tmp * 100) / 751);
  tmp = (uint32_t)((tmp + 5) / 10);

  /* return the temprature row value */
  return tmp;
}

/**
  * @brief  Checks the selected Global interrupt source pending bit
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  Global_IT: the Global interrupt source to be checked, could be:
  *   @arg  Global_IT_GPIO : All IOs interrupt
  *   @arg  Global_IT_ADC : ADC interrupt
  *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
  *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
  *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
  *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
  *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
  *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
  * @retval Status of the checked flag. Could be SET or RESET.
  */
FlagStatus IOE_GetGITStatus(uint8_t DeviceAddr, uint8_t Global_IT)
{
  __IO uint8_t tmp = 0;

  /* get the Interrupt status */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_INT_STA);

  if ((tmp & (uint8_t)Global_IT) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  Clears the selected Global interrupt pending bit(s)
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  Global_IT: the Global interrupt to be cleared, could be any combination
  *         of the following values:
  *   @arg  Global_IT_GPIO : All IOs interrupt
  *   @arg  Global_IT_ADC : ADC interrupt
  *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
  *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
  *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
  *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
  *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
  *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_ClearGITPending(uint8_t DeviceAddr, uint8_t Global_IT)
{
  /* Write 1 to the bits that have to be cleared */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_INT_STA, Global_IT);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Checks the status of the selected IO interrupt pending bit
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_IT: the IO interrupt to be checked could be IO_ITx Where x can be
  *         from 0 to 7.
  * @retval Status of the checked flag. Could be SET or RESET.
  */
FlagStatus IOE_GetIOITStatus(uint8_t DeviceAddr, uint8_t IO_IT)
{
  uint8_t tmp = 0;

  /* get the Interrupt status */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_INT_STA);

  if ((tmp & (uint8_t)IO_IT) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  Clears the selected IO interrupt pending bit(s).
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_IT: the IO interrupt to be checked could be IO_ITx Where x can be
  *         from 0 to 7.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_ClearIOITPending(uint8_t DeviceAddr, uint8_t IO_IT)
{
  /* Write 1 to the bits that have to be cleared */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_INT_STA, IO_IT);

  /* Clear the Edge detection pending bit*/
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_GPIO_ED, IO_IT);

  /* Clear the Rising edge pending bit */
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_GPIO_RE, IO_IT);

  /* Clear the Falling edge pending bit */
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_GPIO_FE, IO_IT);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Checks if the selected device is correctly configured and
  *         communicates correctly ont the I2C bus.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @retval IOE_OK if IOE is operational. Other value if failure.
  */
uint8_t IOE_IsOperational(uint8_t DeviceAddr)
{
  /* Return Error if the ID is not correct */
  if( IOE_ReadID(DeviceAddr) != (uint16_t)STMPE811_ID )
  {
    /* Check if a Timeout occured */
    if (TimeOut == 0)
    {
      return IOE_TIEMOUT;
    }
    else
    {
      return IOE_FAILURE; /* ID is not Correct */
    }
  }
  else
  {
    return IOE_OK; /* ID is correct */
  }
}

/**
  * @brief  Resets the IO Expander by Software (SYS_CTRL1, RESET bit).
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_Reset(uint8_t DeviceAddr)
{
  /* Power Down the IO_Expander */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL1, 0x02);

  /* wait for a delay to insure registers erasing */
  _delay_(2);

  /* Power On the Codec after the power off => all registers are reinitialized*/
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL1, 0x00);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Reads the selected device's ID.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @retval The Device ID (two bytes).
  */
uint16_t IOE_ReadID(uint8_t DeviceAddr)
{
  uint16_t tmp = 0;

  /* Read device ID  */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, 0);
  tmp = (uint32_t)(tmp << 8);
  tmp |= (uint32_t)I2C_ReadDeviceRegister(DeviceAddr, 1);

  /* Return the ID */
  return (uint16_t)tmp;
}

/**
  * @brief  Configures the selcted IO Expander functionalities.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IOE_TEMPSENS_FCT: the functions to be configured. could be any
  *         combination of the following values:
  *   @arg  IOE_IO_FCT : IO function
  *   @arg  IOE_TS_FCT : Touch Screen function
  *   @arg  IOE_ADC_FCT : ADC function
  *   @arg  IOE_TEMPSENS_FCT : Tempreature Sensor function
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_FnctCmd(uint8_t DeviceAddr, uint8_t Fct, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Get the register value */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL2);

  if (NewState != DISABLE)
  {
    /* Set the Functionalities to be Enabled */
    tmp &= ~(uint8_t)Fct;
  }
  else
  {
    /* Set the Functionalities to be Disabled */
    tmp |= (uint8_t)Fct;
  }

  /* Set the register value */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL2, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Configures the selected pin direction (to be an input or an output)
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_Pin: IO_Pin_x: Where x can be from 0 to 7.
  * @param  Direction: could be Direction_IN or Direction_OUT.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOPinConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Direction)
{
  uint8_t tmp = 0;

  /* Get all the Pins direction */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_DIR);

  if (Direction != Direction_IN)
  {
    tmp |= (uint8_t)IO_Pin;
  }
  else
  {
    tmp &= ~(uint8_t)IO_Pin;
  }

  /* Write the register new value */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_DIR, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Enables or disables the Global interrupt.
  * @param  DeviceAddr: The address of the IOExpander, could be :I OE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  NewState: could be ENABLE or DISABLE.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_GITCmd(uint8_t DeviceAddr, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Read the Interrupt Control register  */
  I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_INT_CTRL);

  if (NewState != DISABLE)
  {
    /* Set the global interrupts to be Enabled */
    tmp |= (uint8_t)IOE_GIT_EN;
  }
  else
  {
    /* Set the global interrupts to be Disabled */
    tmp &= ~(uint8_t)IOE_GIT_EN;
  }

  /* Write Back the Interrupt Control register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_INT_CTRL, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Configures the selected source to generate or not a global interrupt
  * @param DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *        or IOE_2_ADDR.
  * @param Global_IT: the interrupt source to be configured, could be:
  *   @arg  Global_IT_GPIO : All IOs interrupt
  *   @arg  Global_IT_ADC : ADC interrupt
  *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
  *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
  *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
  *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
  *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
  *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_GITConfig(uint8_t DeviceAddr, uint8_t Global_IT, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Get the current value of the INT_EN register */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_INT_EN);

  if (NewState != DISABLE)
  {
    /* Set the interrupts to be Enabled */
    tmp |= (uint8_t)Global_IT;
  }
  else
  {
    /* Set the interrupts to be Disabled */
    tmp &= ~(uint8_t)Global_IT;
  }
  /* Set the register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_INT_EN, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Configures the selected pins to generate an interrupt or not.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_IT: The IO interrupt to be configured. This parameter could be any
  *         combination of the following values:
  *   @arg  IO_IT_x: where x can be from 0 to 7.
  * @param  NewState: could be ENABLE or DISABLE.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOITConfig(uint8_t DeviceAddr, uint8_t IO_IT, FunctionalState NewState)
{
  uint8_t tmp = 0;

  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_INT_EN);

  if (NewState != DISABLE)
  {
    /* Set the interrupts to be Enabled */
    tmp |= (uint8_t)IO_IT;
  }
  else
  {
    /* Set the interrupts to be Disabled */
    tmp &= ~(uint8_t)IO_IT;
  }

  /* Set the register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_INT_EN, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Configures the touch Screen Controller (Single point detection)
  * @param  None
  * @retval IOE_OK if all initializations are OK. Other value if error.
  */
uint8_t IOE_TS_Config(void)
{
  uint8_t tmp = 0;

  /* Enable TSC Fct: already done in IOE_Config */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_SYS_CTRL2);
  tmp &= ~(uint32_t)(IOE_TS_FCT | IOE_ADC_FCT);
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_SYS_CTRL2, tmp);

  /* Enable the TSC gloabl interrupts */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_INT_EN);
  tmp |= (uint32_t)(IOE_GIT_TOUCH | IOE_GIT_FTH | IOE_GIT_FOV);
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_INT_EN, tmp);

  /* Select Sample Time, bit number and ADC Reference */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_ADC_CTRL1, 0x49);

  /* Wait for ~20 ms */
  _delay_(2);

  /* Select the ADC clock speed: 3.25 MHz */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_ADC_CTRL2, 0x01);

  /* Select TSC pins in non default mode */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_AF);
  tmp &= ~(uint8_t)TOUCH_IO_ALL;
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_AF, tmp);

  /* Select 2 nF filter capacitor */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_CFG, 0x9A);

  /* Select single point reading  */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_TH, 0x01);

  /* Write 0x01 to clear the FIFO memory content. */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x01);

  /* Write 0x00 to put the FIFO back into operation mode  */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x00);

  /* set the data format for Z value: 7 fractional part and 1 whole part */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_FRACT_XYZ, 0x01);

  /* set the driving capability of the device for TSC pins: 50mA */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_I_DRIVE, 0x01);

  /* Use no tracking index, touchscreen controller operation mode (XYZ) and
     enable the TSC */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_CTRL, 0x01);

  /*  Clear all the status pending bits */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_INT_STA, 0xFF);

  /* Initialize the TS structure to their default values */
  TS_State.TouchDetected = TS_State.X = TS_State.Y = TS_State.Z = 0;

  /* All configuration done */
  return IOE_OK;
}

/**
  * @brief  Configures and enables the Temperature sensor module.
  * @param  None
  * @retval IOE_OK if all initializations are OK. Other value if error.
  */
uint8_t IOE_TempSens_Config(void)
{
  __IO uint8_t tmp = 0;

  /* Enable Temperature Sensor Fct: already done in IOE_Config */
  tmp = I2C_ReadDeviceRegister(IOE_2_ADDR, IOE_REG_SYS_CTRL2);
  tmp &= ~(uint32_t)(IOE_TEMPSENS_FCT | IOE_ADC_FCT);
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_SYS_CTRL2, tmp);

  /* Enable the TEMPSENS module */
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_TEMP_CTRL, 0x01);

  /* Aquire data enable */
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_TEMP_CTRL, 0x3);

  /* All configuration done */
  return IOE_OK;
}

/**
  * @brief  Configures the selected pin to be in Alternate function or not
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_Pin: IO_Pin_x, Where x can be from 0 to 7.
  * @param  NewState: State of the AF for the selected pin, could be
  *         ENABLE or DISABLE.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOAFConfig(uint8_t DeviceAddr, uint8_t IO_Pin, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Get the current state of the GPIO_AF register */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_AF);

  if (NewState != DISABLE)
  {
    /* Enable the selected pins alternate function */
    tmp |= (uint8_t)IO_Pin;
  }
  else
  {
    /* Disable the selected pins alternate function */
    tmp &= ~(uint8_t)IO_Pin;
  }

  /* Write back the new valu in GPIO_AF register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_AF, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Configures the Edge for which a transition is detectable for the
  *         the selected pin.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_Pin: IO_Pin_x, Where x can be from 0 to 7.
  * @param  Edge: The edge which will be detected. This parameter can be one or a
  *         a combination of follwing values: EDGE_FALLING and EDGE_RISING .
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOEdgeConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Edge)
{
  uint8_t tmp1 = 0, tmp2 = 0;

  /* Get the registers values */
  tmp1 = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_FE);
  tmp2 = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_RE);

  /* Disable the Falling Edge */
  tmp1 &= ~(uint8_t)IO_Pin;
  /* Disable the Falling Edge */
  tmp2 &= ~(uint8_t)IO_Pin;

  /* Enable the Falling edge if selected */
  if (Edge & EDGE_FALLING)
  {
    tmp1 |= (uint8_t)IO_Pin;
  }

  /* Enable the Rising edge if selected */
  if (Edge & EDGE_RISING)
  {
    tmp2 |= (uint8_t)IO_Pin;
  }

  /* Write back the registers values */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_FE, tmp1);
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_RE, tmp2);

  /* if OK return 0 */
  return IOE_OK;
}

/**
  * @brief  Configures the Interrupt line active state and format (level/edge)
  * @param  Polarity: could be
  *   @arg  Polarity_Low: Interrupt line is active Low/Falling edge
  *   @arg  Polarity_High: Interrupt line is active High/Rising edge
  * @param Type: Interrupt line activity type, could be one of the following values
  *   @arg  Type_Level: Interrupt line is active in level model
  *   @arg  Type_Edge: Interrupt line is active in edge model
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_ITOutConfig(uint8_t Polarity, uint8_t Type)
{
  uint8_t tmp = 0;

  /*  Get the register IOE_REG_INT_CTRL value */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_INT_CTRL);

  /* Mask the polarity and type bits */
  tmp &= ~(uint8_t)0x06;

  /* Modify the Interrupt Output line configuration */
  tmp |= (uint8_t)(Polarity | Type);

  /* Set the register */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_INT_CTRL, tmp);


  /*  Get the register IOE_REG_INT_CTRL value */
  tmp = I2C_ReadDeviceRegister(IOE_2_ADDR, IOE_REG_INT_CTRL);
  /* Mask the polarity and type bits */
  tmp &= ~(uint8_t)0x06;

  /* Modify the Interrupt Output line configuration */
  tmp |= (uint8_t)(Polarity | Type);

  /* Set the register */
  I2C_WriteDeviceRegister(IOE_2_ADDR, IOE_REG_INT_CTRL, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Writes a value in a register of the IOE through I2C.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  RegisterAddr: The target register adress
  * @param  RegisterValue: The target register value to be written
  * @retval IOE_OK: if all operations are OK. Other value if error.
  */
uint8_t I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue)
{
  uint32_t read_verif = 0;

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(IOE_I2C, ENABLE);
  I2C_SoftwareResetCmd(IOE_I2C, DISABLE);

  TimeOut = TIMEOUT_MAX;

  /* Enable the IOE_I2C peripheral  */
  I2C_Cmd(IOE_I2C, ENABLE);

  /* Configure the I2C peripheral */
  IOE_I2C_Config();

  /* Begin the config sequence */
  I2C_GenerateSTART(IOE_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Transmit the first address for r/w operations */
  I2C_SendData(IOE_I2C, RegisterAddr);

  TimeOut = TIMEOUT_MAX;

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Prepare the register value to be sent */
  I2C_SendData(IOE_I2C, RegisterValue);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* End the configuration sequence */
  I2C_GenerateSTOP(IOE_I2C, ENABLE);

#ifdef VERIFY_WRITTENDATA
  /* Verify (if needed) that the loaded data is correct  */

  /* Read the just written register*/
  read_verif = I2C_ReadDeviceRegister(DeviceAddr, RegisterAddr);
  /* Load the register and verify its value  */
  if (read_verif != RegisterValue)
  {
    /* Control data wrongly tranfered */
    read_verif = IOE_FAILURE;
  }
  else
  {
    /* Control data correctly transfered */
    read_verif = 0;
  }
#endif

  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return read_verif;
}

/**
  * @brief  Reads a register of the audio Codec through I2C.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  RegisterAddr: The target register adress (between 00x and 0x24)
  * @retval The value of the read register (0xAA if Timout occured)
  */
uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr)
{
  uint32_t tmp = 0;

  /* Disable the IOE_I2C peripheral  */
  I2C_Cmd(IOE_I2C, DISABLE);

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(IOE_I2C, ENABLE);
  I2C_SoftwareResetCmd(IOE_I2C, DISABLE);

  /* Configure the I2C peripheral */
  IOE_I2C_Config();

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(IOE_I2C, ENABLE);

  TimeOut = TIMEOUT_MAX;
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }
  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(IOE_I2C, DISABLE);

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Transmitter);

  TimeOut = TIMEOUT_MAX;
  /* Test on EV6 and clear it */

  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Transmit the first address for r/w operations */
  I2C_SendData(IOE_I2C, RegisterAddr);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }
  /* Regenerate a start condition */
  I2C_GenerateSTART(IOE_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Test on EV7 and clear it */
  while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* End the configuration sequence */
  I2C_GenerateSTOP(IOE_I2C, ENABLE);

  /* Load the register value */
  tmp = I2C_ReceiveData(IOE_I2C);

  /* Enable Acknowledgement */
  I2C_AcknowledgeConfig(IOE_I2C, ENABLE);

  /* Return the read value */
  return tmp;
}


/**
  * @brief  Reads a buffer of 4 bytes from IO_Expander registers.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  RegisterAddr: The target register adress (between 00x and 0x24)
  * @retval : The value of the read register (0xAA if Timout occured)
  */
uint32_t I2C_ReadDataBuffer(u8 DeviceAddr, uint32_t RegisterAddr)
{
  u8 Buffer[4] , idx = 2;

  /* Initialize the buffer */
  Buffer[0] = 0;
  Buffer[1] = 0;
  Buffer[2] = 0;
  Buffer[3] = 0;

  /* Disable the I2C1 peripheral  */
  I2C_Cmd(I2C1, DISABLE);

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, DISABLE);

  /* Configure the I2C peripheral */
  IOE_I2C_Config();

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(I2C1, ENABLE);

  TimeOut = TIMEOUT_MAX;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Send device address for write */
  I2C_Send7bitAddress(I2C1, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the device's internal address to write to */
  I2C_SendData(I2C1, RegisterAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Send STRAT condition a second time */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, DeviceAddr, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    if (TimeOut-- == 0) return IOE_TIEMOUT;
  }

  /* While there is data to be read */
  while(idx)
  {
    if(idx == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);

      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      /* Read a byte from the EEPROM */
      Buffer[idx-1] = I2C_ReceiveData(I2C1);

      /* Decrement the read bytes counter */
      idx--;
    }
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /* return a pointer to the buffer */
  return *(uint32_t *)Buffer;
}















/**
  * @brief  Return Touch Screen X position value
  * @param  None
  * @retval X position.
  */
static uint16_t IOE_TS_Read_X(void)
{
  s32 x, xr;

  x = I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_Y);

  /* first correction */
  xr =  (x * 320) >> 12;
  /* second correction */
  xr = ((xr * 32)/29) - 17;

  if(xr <= 0)
    xr = 0;

  return (uint16_t)(xr);
}

/**
  * @brief  Return Touch Screen Y position value
  * @param  None
  * @retval Y position.
  */
static uint16_t IOE_TS_Read_Y(void)
{
  s32 y, yr;
  y= I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_X);

  yr= (y * 240) >> 12;
  yr = ((yr * 240) / 217) - 12;

  if(yr <= 0)
    yr = 0;

  return (uint16_t)(yr);
}

/**
  * @brief  Return Touch Screen Z position value
  * @param  None
  * @retval Z position.
  */
static uint16_t IOE_TS_Read_Z(void)
{
  uint32_t z;
  z = I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_Z);


  if(z <= 0)
    z = 0;

  return (uint16_t)(z);
}

/**
  * @brief  Initializes the GPIO pins used by the IO expander.
  * @param  None
  * @retval None
  */
static void IOE_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable IOE_I2C and IOE_I2C_PORT & Alternate Function clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_IOE_I2C, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB_IOE_I2C_PORT | RCC_APB_GPIO_IOE_ITPORT | RCC_APB2Periph_AFIO, ENABLE);

  /* Reset IOE_I2C IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_IOE_I2C, ENABLE);

  /* Release reset signal of IOE_I2C IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_IOE_I2C, DISABLE);

  /* IOE_I2C SCL and SDA pins configuration */
  GPIO_InitStructure.GPIO_Pin = IOE_SCL_PIN | IOE_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(IOE_I2C_PORT, &GPIO_InitStructure);

  /* Set EXTI pin as Input PullUp - IO_Expander_INT */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IOE_ITPIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIO_IOE_ITPORT, &GPIO_InitStructure);

  /* Connect IO Expander IT line to EXTI line */
  GPIO_EXTILineConfig(GPIO_PortSource_IOE_ITPORT, GPIO_PinSource_IOE_ITPIN);
}


/**
  * @brief  Configure the I2C Peripheral used to communicate with IO_Expanders.
  * @param  None
  * @retval None
  */
static void IOE_I2C_Config(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  /* IOE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = IOE_I2C_SPEED;

  I2C_Init(IOE_I2C, &I2C_InitStructure);
}


/**
  * @brief  Configures the IO expander Interrupt line and GPIO in EXTI mode.
  * @param  None
  * @retval None
  */
static void IOE_EXTI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Enable Button GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB_GPIO_IOE_ITPORT | RCC_APB2Periph_AFIO, ENABLE);

  /* Configure Button pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_IOE_ITPIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_IOE_ITPORT, &GPIO_InitStructure);

  /* Connect Button EXTI Line to Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PortSource_IOE_ITPORT, GPIO_PinSource_IOE_ITPIN);

  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_IOE_ITLINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = IOE_IT_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#ifndef USE_Delay
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0;
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
#endif /* USE_Delay*/
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

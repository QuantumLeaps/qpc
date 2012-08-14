/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_hd44780.c
 *    Description : HD44780 driver
 *
 *    History :
 *    1. Date        : Feb 12, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : Nov 3, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *                      Correct some functions -
 *                     HD44780_StrShow, HD44780_CheckVisual,
 *                     HD44780_DisplayShift, HD44780_GetDDRamAdd
 *    3. Date        : Jan 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *                      Fix power-up initialization problem whit 4 bit
 *                     interface
 *
 *    $Revision: 16170 $
 **************************************************************************/
#include <ctype.h>
#include <stdint.h>

#include "drv_hd44780_cnfg.h"
#include "drv_hd44780_l.h"
#include "drv_hd44780.h"

#if (HD44780_BUS_WIDTH != 4) && (HD44780_BUS_WIDTH != 8)
#error The HD44780_BUS_WIDTH must be 8 or 4!
#endif

/* delay definition */
#define HD44780_POWER_UP_DLY          400     /* 40 ms (tick 100us) */
#define HD44780_FIRST_COMM_DLY        41      /* 4.1 ms (tick 100us) */
#define HD44780_SECOND_COMM_DLY       2       /* 100 us (tick 100us) */
#define HD44780_MAX_COMM_DLY          30      /* 3ms (tick 100us) */

/* HD44780 command set */
#define HD44780_CLEAR                 0x01    /* Clear display */
#define HD44780_RETURN                0x02    /* Return display to home */

#define HD44780_ENTRY_MODE            0x04    /* Set Cursor, Display move and direction */
#define HD44780_ENTRY_MODE_CURSOR_S   0x00
#define HD44780_ENTRY_MODE_BOTH_S     0x01
#define HD44780_ENTRY_MODE_DEC        0x00
#define HD44780_ENTRY_MODE_INC        0x02

#define HD44780_DISPLAY_CTRL          0x08    /* Set display and cursor mode */
#define HD44780_DISPLAY_CTRL_C_NORM   0x00
#define HD44780_DISPLAY_CTRL_C_BLINK  0x01
#define HD44780_DISPLAY_CTRL_C_OFF    0x00
#define HD44780_DISPLAY_CTRL_C_ON     0x02
#define HD44780_DISPLAY_CTRL_D_OFF    0x00
#define HD44780_DISPLAY_CTRL_D_ON     0x04

#define HD44780_DISPLAY_MOVE          0x10    /* Display and cursor move */
#define HD44780_DISPLAY_MOVE_LEFT     0x00
#define HD44780_DISPLAY_MOVE_RIGHT    0x04
#define HD44780_DISPLAY_MOVE_C        0x00
#define HD44780_DISPLAY_MOVE_D        0x08

#define HD44780_FUNCTION_SET          0x20    /* Set display function */
#define HD44780_FUNCTION_SET_DOT_5_8  0x00
#define HD44780_FUNCTION_SET_DOT_5_10 0x04
#define HD44780_FUNCTION_SET_1_LINE   0x00
#define HD44780_FUNCTION_SET_2_LINE   0x08
#define HD44780_FUNCTION_SET_4_BIT    0x00
#define HD44780_FUNCTION_SET_8_BIT    0x10

#define HD44780_SET_CGRAM_ADD         0x40    /* Set AC point to CGRAM */
#define HD44780_SET_DDRAM_ADD         0x80    /* Set AC point to DDRAM */

// HD44780 Read Status
#define HD44780_STATUS_AC_MASK        0x7F    /* AC Mask */
#define HD44780_STATUS_BUSY_MASK      0x80    /* Busy Mask */

#define HD44780_BUSY_CHECK            TRUE
#define HD44780_NOT_BUSY_CHECK        FALSE

HD44780_CTRL_DEF HD4478_Ctrl =
{
  0,                    /* Display Position */
  HD44780_AC_DIR,       /* Address counter direction */
  HD44780_MOVE,         /* Cursor or display move */
  HD44780_LINE,         /* Number of line */
  HD44780_DOT_MATRIX,   /* Character Dot matrix */
  HD44780_DISPLAY_STATE,/* Display state On or Off */
  HD44780_CURSOR_STATE, /* Cursor state On or Off */
  HD44780_CURSOR_MODE,  /* Cursor mode Blink or not */
};

#if HD4780_WR == 0
int8_t DataRamAddHold = 0;
#endif

#if HD44780_BUS_WIDTH == 8
  #define HD44780WrComm_High(Data)  HD44780WrIO(Data)
#else
  #define HD44780WrComm_High(Data)  HD44780WrIO((Data>>4))
#endif

/*************************************************************************
 * Function Name: HD44780WrData
 * Parameters: uint8_t Data
 * Return: none
 * Description: Write data to HD44780
 *
 *************************************************************************/
void HD44780WrData (uint8_t Data)
{
  HD44780SetRS(1);
#if HD44780_BUS_WIDTH == 8
  HD44780WrIO(Data);
#else
  HD44780WrIO((Data>>4) & 0xF);
  HD44780WrIO(Data & 0xF);
#endif
}

#if HD4780_WR > 0
/*************************************************************************
 * Function Name: HD44780RdData
 * Parameters: none
 * Return: uint8_t
 * Description: Read data from HD44780
 *
 *************************************************************************/
uint8_t HD44780RdData (void)
{
uint8_t Data;
  HD44780SetRS(1);
#if HD44780_BUS_WIDTH == 8
  Data = HD44780RdIO();
#else
  Data = (HD44780RdIO() << 4) & 0xF0;
  Data |= HD44780RdIO()       & 0x0F;
#endif
  return Data;
}
#endif

/*************************************************************************
 * Function Name: HD44780WrComm
 * Parameters: uint8_t Command
 * Return: none
 * Description: Send command to HD44780
 *
 *************************************************************************/
void HD44780WrComm (uint8_t Command)
{
  HD44780SetRS(0);
#if HD44780_BUS_WIDTH == 8
  HD44780WrIO(Command);
#else
  HD44780WrIO((Command >> 4) & 0xF);
  HD44780WrIO(Command & 0xF);
#endif
}

#if HD4780_WR > 0
/*************************************************************************
 * Function Name: HD44780RdStatus
 * Parameters: none
 * Return: uint8_t
 * Description: Read status of HD44780
 *
 *************************************************************************/
uint8_t HD44780RdStatus (void)
{
uint8_t Data;
  HD44780SetRS(0);
#if HD44780_BUS_WIDTH == 8
  Data = HD44780RdIO();
#else
  Data = (HD44780RdIO() << 4) & 0xF0;
  Data |= HD44780RdIO()       & 0x0F;
#endif
  return Data;
}
#endif

/*************************************************************************
 * Function Name: HD44780_GetDDRamAdd
 * Parameters: HD44780_XY_DEF X,
 *             HD44780_XY_DEF Y,
 *             uint8_t * DDAdd
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *       HD44780_ERROR 2: Out of boundary
 * Description: Calculate DDRAM address by X,Y coordinate
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_GetDDRamAdd (HD44780_XY_DEF X, HD44780_XY_DEF Y,uint8_t * DDAdd)
{
  /* Find Address by coordinate */
  if (Y > 2)
  {
    return HD44780_ERROR;
  }
  if (X > HD44780_MAX_LINE1_ADD)
  {
    return HD44780_ERROR;
  }
  *DDAdd = X-1;
  if (Y == 2)
  {
    * DDAdd += HD44780_MIN_LINE2_ADD;
  }
  return HD44780_OK;
}


/*************************************************************************
 * Function Name: HD44780_CheckVisual
 * Parameters: uint8_t DDRamAdd
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK           : Pass
 *         HD44780_ERROR        : Out of address boundary
 *         HD44780_OUT_OF_VISUAL: Out of visual boundary
 * Description: Whether current DDRAM address is into visual area
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_CheckVisual(uint8_t DDRamAdd)
{
uint8_t LastPos = 0;
  if (DDRamAdd > HD44780_MAX_LINE2_ADD)
  {
    return HD44780_ERROR;
  }
  if((DDRamAdd > HD44780_MAX_LINE1_ADD) && (DDRamAdd < HD44780_MIN_LINE2_ADD))
  {
    return HD44780_ERROR;
  }
  if (DDRamAdd >= HD44780_MIN_LINE2_ADD)
  {
    /* Convert address to line 1 address */
    DDRamAdd -= 0x40;
  }
  LastPos = HD4478_Ctrl.DisplayPos+HD44780_HORIZONTAL_SIZE;
  if(LastPos > HD44780_MAX_LINE1_ADD)
  {
    if((DDRamAdd > HD44780_MAX_LINE1_ADD - HD4478_Ctrl.DisplayPos) && (DDRamAdd < HD4478_Ctrl.DisplayPos))
    {
      return HD44780_OUT_OF_VISUAL;
    }
    LastPos = HD44780_MAX_LINE1_ADD;
  }
  if ((DDRamAdd < HD4478_Ctrl.DisplayPos) || (DDRamAdd > LastPos))
  {
    return HD44780_OUT_OF_VISUAL;
  }
  return HD44780_OK;
}

/*************************************************************************
 * Function Name: HD44780_BusyCheck
 * Parameters: uint8_t * AddCount, uint32_t MaxDly
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *     HD44780_ERROR 1: Busy check Time Out
 * Description: Wait MaxDly *100u or until the busy flag is clear
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_BusyCheck (uint8_t * AddCount, uint32_t MaxDly)
{
#if HD4780_WR > 0
uint8_t AddHold;
#endif
  for (;MaxDly;--MaxDly)
  {
#if HD4780_WR > 0
    AddHold = HD44780RdStatus();
    if ((AddHold & HD44780_STATUS_BUSY_MASK) == 0)
    {
      /* Wait 1.5 * Tlcd */
      HD44780_BUS_DLY();
      HD44780_BUS_DLY();
      /* Get current AC */
      AddHold = HD44780RdStatus();
      AddHold &= HD44780_STATUS_AC_MASK;
      if(AddCount != NULL)
      {
        *AddCount = AddHold;
      }
      return HD44780_OK;
    }
#endif
    Dly100us((void *)1);
  }
#if HD4780_WR > 0
  return HD44780_BUSY_TO_ERROR;
#else
  if(AddCount != NULL)
  {
    *AddCount = DataRamAddHold;
  }
  return HD44780_OK;
#endif
}

/*************************************************************************
 * Function Name: HD44780_Init
 * Parameters: none
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *     HD44780_ERROR 1: Busy check Time Out
 * Description: Init HD44780 after power-up
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_PowerUpInit (void)
{
uint8_t Command;
  HD4478_Ctrl.DisplayPos = 0;
  /* Init MCU IO */
  HD44780_IO_Init();
  /* Power up init sequence */
  Dly100us((void *)HD44780_POWER_UP_DLY);
  HD44780SetRS(0);
  HD44780WrComm_High(HD44780_FUNCTION_SET+HD44780_FUNCTION_SET_8_BIT);
  Dly100us((void *)HD44780_FIRST_COMM_DLY);
  HD44780WrComm_High(HD44780_FUNCTION_SET+HD44780_FUNCTION_SET_8_BIT);
  Dly100us((void *)HD44780_SECOND_COMM_DLY);
  HD44780WrComm_High(HD44780_FUNCTION_SET+HD44780_FUNCTION_SET_8_BIT);
  Dly100us((void *)HD44780_SECOND_COMM_DLY);
  /* Display Function set */
#if HD44780_BUS_WIDTH == 8
  Command = HD44780_FUNCTION_SET + HD44780_FUNCTION_SET_8_BIT;
#else
  HD44780WrComm_High(HD44780_FUNCTION_SET + HD44780_FUNCTION_SET_4_BIT);
  Dly100us((void *)HD44780_SECOND_COMM_DLY);
  Command = HD44780_FUNCTION_SET + HD44780_FUNCTION_SET_4_BIT;
#endif
  if (HD4478_Ctrl.Line)
  {
    Command |= HD44780_FUNCTION_SET_2_LINE;
  }
  if (HD4478_Ctrl.DotMode)
  {
    Command |= HD44780_FUNCTION_SET_DOT_5_10;
  }
  HD44780WrComm(Command);
  if (HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Display off */
  HD44780WrComm(HD44780_DISPLAY_CTRL);
  if (HD44780_BusyCheck(NULL,HD44780_MAX_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Display clear */
  if (HD44780_ClearDisplay() != HD44780_OK)
  {
    return HD44780_ERROR;
  }
#if HD4780_WR == 0
  DataRamAddHold = 0;
#endif
  /* Set entry mode */
  Command = HD44780_ENTRY_MODE;
  if (HD4478_Ctrl.AC_Direction)
  {
    Command |= HD44780_ENTRY_MODE_INC;
  }
  if (HD4478_Ctrl.DisplayShift)
  {
    Command |= HD44780_ENTRY_MODE_BOTH_S;
  }
  /* Set Display and cursor mode */
  if (HD44780_SetMode() != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  return HD44780_OK;
}

/*************************************************************************
 * Function Name: HD44780_GetSetting
 * Parameters: none
 * Return: HD44780_CTRL_DEF *
 * Description: Return pointer to Driver settings structure
 *
 *************************************************************************/
HD44780_CTRL_DEF * HD44780_GetSetting(void)
{
  return &HD4478_Ctrl;
}

/*************************************************************************
 * Function Name: HD44780_SetMode
 * Parameters: none
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *     HD44780_ERROR 1: Busy check Time Out
 * Description: Set display mode: Display On/Off; Cursor On/Off
 *  Cursor blink On/Off
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_SetMode (void)
{
uint8_t Command = HD44780_DISPLAY_CTRL;
  if (HD4478_Ctrl.DisplayOn)
  {
    Command |= HD44780_DISPLAY_CTRL_D_ON;
  }
  if (HD4478_Ctrl.CursorOn)
  {
    Command |= HD44780_DISPLAY_CTRL_C_ON;
  }
  if (HD4478_Ctrl.CursorBlink)
  {
    Command |= HD44780_DISPLAY_CTRL_C_BLINK;
  }
  HD44780WrComm(Command);
  return HD44780_BusyCheck(NULL,HD44780_FIRST_COMM_DLY);
}

/*************************************************************************
 * Function Name: HD44780_ClearDisplay
 * Parameters: none
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *     HD44780_ERROR 1: Busy check Time Out
 * Description: Clear display and set position to home
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_ClearDisplay (void)
{
#if HD4780_WR == 0
  DataRamAddHold = 0;
#endif
  HD4478_Ctrl.DisplayPos = 0;
  HD44780WrComm(HD44780_CLEAR);
  return HD44780_BusyCheck(NULL,HD44780_MAX_COMM_DLY);
}

/*************************************************************************
 * Function Name: HD44780_ReturnToHome
 * Parameters: none
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *     HD44780_ERROR 1: Busy check Time Out
 * Description: Set display position to home
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_ReturnToHome (void)
{
#if HD4780_WR == 0
  DataRamAddHold = 0;
#endif
  HD4478_Ctrl.DisplayPos = 0;
  HD44780WrComm(HD44780_RETURN);
  return HD44780_BusyCheck(NULL,HD44780_MAX_COMM_DLY);
}

/*************************************************************************
 * Function Name: HD44780_GetDisplayPosition
 * Parameters: none
 * Return: uint8_t
 *
 * Description: Return X coordinate of visual part
 *
 *************************************************************************/
uint8_t HD44780_GetDisplayPosition (void)
{
  return HD4478_Ctrl.DisplayPos;
}

/*************************************************************************
 * Function Name: HD44780_DisplayShift
 * Parameters: uint8_t DisplayOn, int8_t DisplayShift
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Shift display
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_DisplayShift (uint8_t DisplayOn, int8_t DisplayShift)
{
uint8_t ShiftDir;
  if (DisplayShift >= 0)
  {
    if(DisplayShift > HD44780_MAX_LINE1_ADD)
    {
      return HD44780_ERROR;
    }
    ShiftDir = HD44780_DISPLAY_MOVE_RIGHT;
  }
  else
  {
    if(DisplayShift < -HD44780_MAX_LINE2_ADD)
    {
      return HD44780_ERROR;
    }
    ShiftDir = HD44780_DISPLAY_MOVE_LEFT;
    DisplayShift = 0 - DisplayShift;  /* DisplayShift = -DisplayShift */
  }
  for (int i = 0; i < DisplayShift; ++i)
  {
    HD44780WrComm(HD44780_DISPLAY_MOVE + HD44780_DISPLAY_MOVE_D + ShiftDir);
    if (HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) != HD44780_OK)
    {
      return HD44780_ERROR;
    }
    if (ShiftDir == HD44780_DISPLAY_MOVE_LEFT)
    {
      if(++HD4478_Ctrl.DisplayPos > HD44780_MAX_LINE1_ADD)
      {
        HD4478_Ctrl.DisplayPos = 0;
      }
    }
    else
    {
      if(--HD4478_Ctrl.DisplayPos > HD44780_MAX_LINE1_ADD) /* --0 = 0xFF > 39 */
      {
        HD4478_Ctrl.DisplayPos = HD44780_MAX_LINE1_ADD;
      }
    }
  }
  return HD44780_OK;
}

/*************************************************************************
 * Function Name: HD44780_CursorPosSet
 * Parameters: uint8_t CursorOn,uint8_t CursorBlink,
 *             HD44780_XY_DEF X, HD44780_XY_DEF Y
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK  0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Set position of cursor
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_CursorPosSet (uint8_t CursorOn,uint8_t CursorBlink, HD44780_XY_DEF X, HD44780_XY_DEF Y)
{
uint8_t CursorPos;
  /* Find Address by coordinate */
  if (HD44780_GetDDRamAdd(X,Y,&CursorPos) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Cursor off */
  HD4478_Ctrl.CursorOn = HD44780_CURSOR_OFF;
  if (HD44780_SetMode() != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Set Address to DDRAM */
  HD44780WrComm(HD44780_SET_DDRAM_ADD + CursorPos);
#if HD4780_WR == 0
  DataRamAddHold = CursorPos;
#endif
  if (HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Set Cursor dependence of CursorOn */
  HD4478_Ctrl.CursorOn = CursorOn;
  HD4478_Ctrl.CursorBlink = CursorBlink;
  return HD44780_SetMode();
}

#if HD4780_WR > 0
/*************************************************************************
 * Function Name: HD44780_RdCGRAM
 * Parameters: HD44780_STRING_DEF * CG_Data,
 *             uint8_t CGRAM_Add
 * Return: HD44780_ERROR_CODE_DEF
 *        HD44780_OK   0: Pass
 *      HD44780_ERROR  1: Busy check Time Out
 * Description: Read Character pattern from CGRAM
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_RdCGRAM (HD44780_STRING_DEF * CG_Data, uint8_t CGRAM_Add)
{
uint8_t Counter = 8;
uint8_t DDRAM_AddHold;
  if (CGRAM_Add > HD44780_MAX_CGRAM)
  {
    return HD44780_ERROR;
  }
  /* Get current DDRAM address */
  if (HD44780_BusyCheck(&DDRAM_AddHold,HD44780_SECOND_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  if (HD4478_Ctrl.DotMode)
  {
    Counter = 10;
    /* Address align to characters space */
    CGRAM_Add &= 0xF;
  }
  else
  {
    Counter = 8;
    /* Address align to characters space */
    CGRAM_Add &= 0x7;
  }
  /* Set CGRAM Address */
  HD44780WrComm(HD44780_SET_CGRAM_ADD + CGRAM_Add);
  if (HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  for ( ;Counter; --Counter)
  {
    *CG_Data = HD44780RdData();
    if (HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) != HD44780_OK)
    {
      break;
    }
    ++CG_Data;
  }
  HD44780WrComm(HD44780_SET_DDRAM_ADD + DDRAM_AddHold);
  return((HD44780_ERROR_CODE_DEF)(HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) || (Counter?HD44780_ERROR:HD44780_OK)));
}
#endif

/*************************************************************************
 * Function Name: HD44780_WrCGRAM
 * Parameters: const HD44780_STRING_DEF * CG_Data,
 *             uint8_t CGRAM_Add
 * Return: HD44780_ERROR_CODE_DEF
 *        HD44780_OK   0: Pass
 *      HD44780_ERROR  1: Busy check Time Out
 * Description: Write Character pattern into CGRAM
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_WrCGRAM (const HD44780_STRING_DEF * CG_Data, uint8_t CGRAM_Add)
{
uint8_t Counter = 8;
uint8_t DDRAM_AddHold;
  /* Get current DDRAM address */
  if (HD44780_BusyCheck(&DDRAM_AddHold,HD44780_SECOND_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  if (HD4478_Ctrl.DotMode)
  {
    Counter = 10;
    /* Address align to characters space */
    CGRAM_Add &= 0xF;
  }
  else
  {
    Counter = 8;
    /* Address align to characters space */
    CGRAM_Add &= 0x7;
  }
  CGRAM_Add *= Counter;
  if (CGRAM_Add > HD44780_MAX_CGRAM)
  {
    return HD44780_ERROR;
  }
  /* Set CGRAM Address */
  HD44780WrComm(HD44780_SET_CGRAM_ADD + CGRAM_Add);
  if (HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  for ( ;Counter; --Counter)
  {
    HD44780WrData(*CG_Data);
    if (HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) != HD44780_OK)
    {
      break;
    }
    ++CG_Data;
  }
  HD44780WrComm(HD44780_SET_DDRAM_ADD + DDRAM_AddHold);
  return((HD44780_ERROR_CODE_DEF)(HD44780_BusyCheck(NULL,HD44780_SECOND_COMM_DLY) || (Counter?HD44780_ERROR:HD44780_OK)));
}

/*************************************************************************
 * Function Name: HD44780_StrShow
 * Parameters: none
 * Return: MENU_ERROR_CODE_DEF
 *        HD44780_OK   0: Pass
 *      HD44780_ERROR  1: Busy check TimeOut
 * Description: Show zero terminate string into LCD
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_StrShow(HD44780_XY_DEF X, HD44780_XY_DEF Y, const HD44780_STRING_DEF * DataStr)
{
uint8_t DDRamAdd;
HD44780_ERROR_CODE_DEF ErrorRes = HD44780_OK;
  if(HD44780_GetDDRamAdd(X,Y,&DDRamAdd) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Set Address to DDRAM */
  HD44780WrComm(HD44780_SET_DDRAM_ADD + DDRamAdd);
#if HD4780_WR == 0
  DataRamAddHold = DDRamAdd;
#endif
  if (HD44780_BusyCheck(&DDRamAdd,HD44780_SECOND_COMM_DLY) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Write zero terminate string int LCD */
  while (*DataStr)
  {
    ErrorRes |= HD44780_CheckVisual(DDRamAdd);
    HD44780WrData(*DataStr);
#if HD4780_WR == 0
    if(HD4478_Ctrl.AC_Direction)
    {
      if((++DataRamAddHold > HD44780_MAX_LINE1_ADD) && (Y == 1))
      {
        DataRamAddHold = HD44780_MIN_LINE2_ADD;
      }
      else if ((Y == 2) && (DataRamAddHold > HD44780_MAX_LINE2_ADD))
      {
        DataRamAddHold = HD44780_MIN_LINE1_ADD;
      }
    }
    else
    {
      if((--DataRamAddHold < 0) && (Y == 1))
      {
        DataRamAddHold = HD44780_MAX_LINE2_ADD;
      }
      else if ((Y == 2) && (DataRamAddHold < HD44780_MIN_LINE2_ADD))
      {
        DataRamAddHold = HD44780_MAX_LINE1_ADD;
      }
    }
#endif
    if (HD44780_BusyCheck(&DDRamAdd,HD44780_SECOND_COMM_DLY) != HD44780_OK)
    {
      return HD44780_ERROR;
    }
    ++DataStr;
    if((Y == 1) && DDRamAdd > HD44780_MAX_LINE1_ADD)
    {
      HD44780WrComm(HD44780_SET_DDRAM_ADD+HD44780_MIN_LINE1_ADD);
    #if HD4780_WR == 0
      DataRamAddHold = HD44780_MIN_LINE1_ADD;
    #endif
      if (HD44780_BusyCheck(&DDRamAdd,HD44780_SECOND_COMM_DLY) != HD44780_OK)
      {
        return HD44780_ERROR;
      }
    }
    else if ((Y == 2) && DDRamAdd < HD44780_MIN_LINE2_ADD)
    {
      HD44780WrComm(HD44780_SET_DDRAM_ADD+HD44780_MIN_LINE2_ADD);
    #if HD4780_WR == 0
      DataRamAddHold = HD44780_MIN_LINE2_ADD;
    #endif
      if (HD44780_BusyCheck(&DDRamAdd,HD44780_SECOND_COMM_DLY) != HD44780_OK)
      {
        return HD44780_ERROR;
      }
    }
  }
  return ErrorRes;
}

/*************************************************************************
 * Function Name: HD44780_DispHorBarInit
 * Parameters: none
 * Return: MENU_ERROR_CODE_DEF
 *        HD44780_OK   0: Pass
 *      HD44780_ERROR  1: Busy check TimeOut
 * Description: Initialize horizontal bar graph
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_DispHorBarInit(void) {
    static int8_t const bar1[] = {
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
    };
    static int8_t const bar2[] = {
        0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18
    };
    static int8_t const bar3[] = {
        0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C
    };
    static int8_t const bar4[] = {
        0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E
    };
    static int8_t const bar5[] = {
        0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F
    };

    HD44780_ERROR_CODE_DEF ErrorRes = HD44780_OK;

    if ((ErrorRes = HD44780_WrCGRAM(bar1, 1)) != HD44780_OK) return ErrorRes;
    if ((ErrorRes = HD44780_WrCGRAM(bar2, 2)) != HD44780_OK) return ErrorRes;
    if ((ErrorRes = HD44780_WrCGRAM(bar3, 3)) != HD44780_OK) return ErrorRes;
    if ((ErrorRes = HD44780_WrCGRAM(bar4, 4)) != HD44780_OK) return ErrorRes;
    if ((ErrorRes = HD44780_WrCGRAM(bar5, 5)) != HD44780_OK) return ErrorRes;

    return ErrorRes;
}

/*************************************************************************
 * Function Name: HD44780_DispHorBar
 * Parameters: none
 * Return: MENU_ERROR_CODE_DEF
 *        HD44780_OK   0: Pass
 *      HD44780_ERROR  1: Busy check TimeOut
 * Description: Display horizontal bar graph
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_DispHorBar(HD44780_XY_DEF X, HD44780_XY_DEF Y,
                                          uint8_t val, uint8_t max)
{
    int8_t str[HD44780_HORIZONTAL_SIZE + 1];
    uint8_t i;

    if (max > HD44780_HORIZONTAL_SIZE*5) { /* clamp val at the display size */
        max = HD44780_HORIZONTAL_SIZE*5;
    }
    if (val > HD44780_HORIZONTAL_SIZE*5) { /* clamp val at the display size */
        val = HD44780_HORIZONTAL_SIZE*5;
    }
    if (val > max) {                                    /* clamp at maximum */
        val = max;
    }
    for (i = 0; max >= 5; max -= 5) {      /* prepare the string to display */
        if (val >= 5) {
            str[i++] = 5;                           /* full block character */
            val -= 5;
        }
        else if (val != 0) {
            str[i++] = val;               /* partial block character (0..4) */
            val = 0;
        }
        else {
            str[i++] = ' ';                        /* empty block character */
        }
    }
    str[i] = 0;                                           /* zero-terminate */

    return HD44780_StrShow(X, Y, str);             /* display the bar graph */
}

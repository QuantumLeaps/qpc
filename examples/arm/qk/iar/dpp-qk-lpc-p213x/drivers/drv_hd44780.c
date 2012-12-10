/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Data        : Feb 12, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 1.3 $
**************************************************************************/
#include <iolpc2138.h>         // IAR-compiler header file for the LPC2138 I/O
#include <stddef.h>
#include "lpc_data_type.h"
#include "drv_hd44780.h"
#include "drv_hd44780_cnfg.h"

/* delay definition */
#define HD44780_POWER_UP_DLY          400     /* 40 ms (tick 100us) */
#define HD44780_FIRST_COMM_DLY        41      /* 4.1 ms (tick 100us) */
#define HD44780_SECOND_COMM_DLY       1       /* 100 us (tick 100us) */
#define HD44780_MAX_COMM_DLY          30      /* 3ms (tick 100us) */

/* HD44780 command set */
#define HD44780_CLEAR                 0x01    /* Clear dislplay */
#define HD44780_RETURN                0x02    /* Return dispaly to home */

#define HD44780_ENTRY_MODE            0x04    /* Display move and direction */
#define HD44780_ENTRY_MODE_CURSOR_S   0x00
#define HD44780_ENTRY_MODE_BOTH_S     0x01
#define HD44780_ENTRY_MODE_DEC        0x00
#define HD44780_ENTRY_MODE_INC        0x02

#define HD44780_DISPLAY_CTRL          0x08    /* Dispaly and cursor mode */
#define HD44780_DISPLAY_CTRL_C_NORM   0x00
#define HD44780_DISPLAY_CTRL_C_BLINK  0x01
#define HD44780_DISPLAY_CTRL_C_OFF    0x00
#define HD44780_DISPLAY_CTRL_C_ON     0x02
#define HD44780_DISPLAY_CTRL_D_OFF    0x00
#define HD44780_DISPLAY_CTRL_D_ON     0x04

#define HD44780_DISPLAY_MOVE          0x10    /* Dispaly and cursor move */
#define HD44780_DISPLAY_MOVE_LEFT     0x00
#define HD44780_DISPLAY_MOVE_RIGHT    0x04
#define HD44780_DISPLAY_MOVE_C        0x00
#define HD44780_DISPLAY_MOVE_D        0x08

#define HD44780_FUNCTION_SET          0x20    /* Set diplay function */
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

/* Display address range definition */
#define HD44780_MIN_LINE1_ADD         0x00
#define HD44780_MAX_LINE1_ADD         0x27
#define HD44780_MIN_LINE2_ADD         0x40
#define HD44780_MAX_LINE2_ADD         0x67

#define HD44780_INPUT_SET_MASK        ~0x00003C00
#define HD44780_OUTPUT_SET_MASK       0x84803C00

typedef struct
{
  LPC_INT32U          :10;
  LPC_INT32U  DataIO  : 4;
  LPC_INT32U          : 9;
  LPC_INT32U  RS      : 1;
  LPC_INT32U          : 2;
  LPC_INT32U  E       : 1;
  LPC_INT32U          : 4;
  LPC_INT32U  R_W     : 1;
} HD44780_IO_DEF;

static volatile HD44780_IO_DEF * HD44780_IO
       = (HD44780_IO_DEF *)0xE0028000;   /* IOPIN0 */
static volatile HD44780_IO_DEF * HD44780_IO_Set
       = (HD44780_IO_DEF *)0xE0028004;   /* IOSET0 */
static volatile HD44780_IO_DEF * HD44780_IO_Clr
       = (HD44780_IO_DEF *)0xE002800C;   /* IOCLR0 */

HD44780_CTRL_DEF HD4478_Ctrl =
{
  0,                    /* Dispaly Position */
  HD44780_AC_DIR,       /* Address counter direction */
  HD44780_MOVE,         /* Cursor or dispaly move */
  HD44780_INTERFACE,    /* Interface to MCU */
  HD44780_LINE,         /* Number of line */
  HD44780_DOT_MATRIX,   /* Character Dot matrix */
  HD44780_DISPLAY_STATE,/* Dispaly state On or Off */
  HD44780_CURSOR_STATE, /* Cursor state On or Off */
  HD44780_CURSOR_MODE,  /* Cursor mode Blink or not */
};

static LPC_BOOL LightState;

static void HD44780WrData (LPC_INT8U Data);
static LPC_INT8U HD44780RdData (void);
static void HD44780WrComm (LPC_INT8U Command);
static LPC_INT8U HD44780RdStatus (void);
static HD44780_ERROR_CODE_DEF HD44780_CheckVisual(LPC_INT8U DDRamAdd);
static HD44780_ERROR_CODE_DEF HD44780_BusyCheck (LPC_INT8U * AddCount);
static HD44780_ERROR_CODE_DEF HD44780_GetDDRamAdd (HD44780_XY_DEF X,
                                   HD44780_XY_DEF Y,LPC_INT8U * DDAdd);


/*************************************************************************
 * Function Name: HD44780WrData
 * Parameters: LPC_INT8U Data
 * Return: none
 * Description: Write data to HD44780
 *
 *************************************************************************/
void HD44780WrData (LPC_INT8U Data)
{
  HD44780_IO_Clr->R_W = HD44780_IO_Set->RS = 1;
  HD44780_IO_Set->E = 1;
  if(HD4478_Ctrl.Interface == HD44780_8_BIT)
  {
    HD44780_IO_Set->DataIO = Data;
    HD44780_IO_Clr->DataIO = ~Data;
  }
  else
  {
    HD44780_IO_Set->DataIO = ((Data>>4) & 0xF);
    HD44780_IO_Clr->DataIO = ~((Data>>4) & 0xF);
    HD44780_BUS_DLY();
    HD44780_IO_Clr->E = 1;
    HD44780_BUS_DLY();
    HD44780_IO_Set->E = 1;
    HD44780_IO_Set->DataIO = Data & 0xF;
    HD44780_IO_Clr->DataIO = ~(Data & 0xF);
  }
  HD44780_BUS_DLY();
  HD44780_IO_Clr->E = 1;
}

/*************************************************************************
 * Function Name: HD44780RdData
 * Parameters: none
 * Return: LPC_INT8U
 * Description: Read data from HD44780
 *
 *************************************************************************/
LPC_INT8U HD44780RdData (void)
{
LPC_INT8U Data;
  HD44780_IO_Set->R_W = HD44780_IO_Set->RS = 1;
  IO0DIR &= HD44780_INPUT_SET_MASK;
  HD44780_IO_Set->E = 1;
  if(HD4478_Ctrl.Interface == HD44780_8_BIT)
  {
    Data = HD44780_IO->DataIO;
  }
  else
  {
    Data = (HD44780_IO->DataIO << 4) & 0xF0;
    HD44780_IO_Clr->E = 1;
    HD44780_BUS_DLY();
    HD44780_IO_Set->E = 1;
    HD44780_BUS_DLY();
    Data |= HD44780_IO->DataIO & 0xF;
  }
  HD44780_IO_Clr->R_W = HD44780_IO_Clr->E = 1;
  IO0DIR |= HD44780_OUTPUT_SET_MASK;
  return Data;
}

/*************************************************************************
 * Function Name: HD44780WrComm
 * Parameters: LPC_INT8U LPC_INT8U Command
 * Return: none
 * Description: Send comand to HD44780
 *
 *************************************************************************/
void HD44780WrComm (LPC_INT8U Command)
{
  HD44780_IO_Clr->R_W = HD44780_IO_Clr->RS = 1;
  HD44780_IO_Set->E = 1;
  if(HD4478_Ctrl.Interface == HD44780_8_BIT)
  {
    HD44780_IO_Set->DataIO = Command;
    HD44780_IO_Clr->DataIO = ~Command;
  }
  else
  {
    HD44780_IO_Set->DataIO = ((Command>>4) & 0xF);
    HD44780_IO_Clr->DataIO = ~((Command>>4) & 0xF);
    HD44780_BUS_DLY();
    HD44780_IO_Clr->E = 1;
    HD44780_BUS_DLY();
    HD44780_IO_Set->E = 1;
    HD44780_IO_Set->DataIO = (Command & 0xF);
    HD44780_IO_Clr->DataIO = ~(Command & 0xF);
  }
  HD44780_BUS_DLY();
  HD44780_IO_Clr->E = 1;
}

/*************************************************************************
 * Function Name: HD44780RdStatus
 * Parameters: none
 * Return: LPC_INT8U
 * Description: Read status of HD44780
 *
 *************************************************************************/
LPC_INT8U HD44780RdStatus (void)
{
LPC_INT8U Data;
  HD44780_IO_Set->R_W = HD44780_IO_Clr->RS = 1;
  IO0DIR &= HD44780_INPUT_SET_MASK;
  HD44780_IO_Set->E = 1;
  if(HD4478_Ctrl.Interface == HD44780_8_BIT)
  {
    Data = HD44780_IO->DataIO;
  }
  else
  {
    Data = (HD44780_IO->DataIO << 4) & 0xF0;
    HD44780_BUS_DLY();
    HD44780_IO_Clr->E = 1;
    HD44780_BUS_DLY();
    HD44780_IO_Set->E = 1;
    Data |= HD44780_IO->DataIO & 0xF;
  }
  HD44780_BUS_DLY();
  HD44780_IO_Clr->R_W = HD44780_IO_Clr->E = 1;
  IO0DIR |= HD44780_OUTPUT_SET_MASK;
  return Data;
}

/*************************************************************************
 * Function Name: HD44780_GetDDRamAdd
 * Parameters: HD44780_XY_DEF X,
 *             HD44780_XY_DEF Y,
 *             LPC_INT8U * DDAdd
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 2: Out of boundary
 * Description: Calculate DDRAM address by X,Y cooardinate
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_GetDDRamAdd (HD44780_XY_DEF X,
                                           HD44780_XY_DEF Y,LPC_INT8U * DDAdd)
{
  /* Find Addres by coordinate */
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
 * Parameters: LPC_INT8U DDRamAdd
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK              : Pass
 *         HD44780_ERROR          : Out of address boundary
 *       HD44780_OUT_OF_VISUAL  : Out of visual boundary
 * Description: Whether current DDRAM address is into visual area
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_CheckVisual(LPC_INT8U DDRamAdd)
{
LPC_INT8U LastPos = 0;
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
  LastPos = HD4478_Ctrl.DiplayPos+HD44780_HORIZONTAL_SIZE;
  if(LastPos > HD44780_MAX_LINE1_ADD)
  {
    if((DDRamAdd > HD44780_MAX_LINE1_ADD - HD4478_Ctrl.DiplayPos)
       && (DDRamAdd < HD4478_Ctrl.DiplayPos))
    {
      return HD44780_OUT_OF_VISUAL;
    }
    LastPos = HD44780_MAX_LINE1_ADD;
  }
  if ((DDRamAdd < HD4478_Ctrl.DiplayPos) || (DDRamAdd > LastPos))
  {
    return HD44780_OUT_OF_VISUAL;
  }
  return HD44780_OK;
}

/*************************************************************************
 * Function Name: HD44780_BusyCheck
 * Parameters: LPC_INT8U * AddCount
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Wait until busy flag is clear (Max waiting - 3ms)
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_BusyCheck (LPC_INT8U * AddCount)
{
LPC_INT8U AddHold;
  for (int Count = 30;Count;--Count)
  {
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
    Dly100us((void *)1);
  }
  return HD44780_ERROR;
}

/*************************************************************************
 * Function Name: HD44780_Init
 * Parameters: none
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Init HD44780 after power-up
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_PowerUpInit (void)
{
LPC_INT8U Command;
  /* Init MCU IO */
  PINSEL0 &= ~0x0FF00000;
  PINSEL1 &= ~0xC030C000;
  HD44780_IO_Set->R_W = HD44780_IO_Clr->RS = HD44780_IO_Clr->E = 1;
  IO0DIR |= HD44780_OUTPUT_SET_MASK;
  /* Power up init sequence */
  Dly100us((void *)HD44780_POWER_UP_DLY);
  HD44780WrComm(HD44780_FUNCTION_SET+HD44780_FUNCTION_SET_8_BIT);
  Dly100us((void *)HD44780_FIRST_COMM_DLY);
  HD44780WrComm(HD44780_FUNCTION_SET+HD44780_FUNCTION_SET_8_BIT);
  Dly100us((void *)HD44780_SECOND_COMM_DLY);
  HD44780WrComm(HD44780_FUNCTION_SET+HD44780_FUNCTION_SET_8_BIT);
  if (HD44780_BusyCheck(NULL) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Dispalay Function set */
  Command = HD44780_FUNCTION_SET;
  if (HD4478_Ctrl.Interface)
  {
    Command |= HD44780_FUNCTION_SET_8_BIT;
  }
  if (HD4478_Ctrl.Line)
  {
    Command |= HD44780_FUNCTION_SET_2_LINE;
  }
  if (HD4478_Ctrl.DotMode)
  {
    Command |= HD44780_FUNCTION_SET_DOT_5_10;
  }
  HD44780WrComm(Command);
  if (HD44780_BusyCheck(NULL) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Display off */
  HD44780WrComm(HD44780_DISPLAY_CTRL);
  if (HD44780_BusyCheck(NULL) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Display clear */
  if (HD44780_ClearDisplay() != HD44780_OK)
  {
    return HD44780_ERROR;
  }
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
  HD44780WrComm(HD44780_DISPLAY_CTRL);
  if (HD44780_BusyCheck(NULL) != HD44780_OK)
  {
    return HD44780_ERROR;
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
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Set diplay mode: Display On/Off; Cursor On/Off
 *  Cursor blink On/Off
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_SetMode (void)
{
LPC_INT8U Command = HD44780_DISPLAY_CTRL;
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
  return HD44780_BusyCheck(NULL);
}

/*************************************************************************
 * Function Name: HD44780_ClearDisplay
 * Parameters: none
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Clear dispaly and set position to home
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_ClearDisplay (void)
{
  HD44780WrComm(HD44780_CLEAR);
  return HD44780_BusyCheck(NULL);
}

/*************************************************************************
 * Function Name: HD44780_ReturnToHome
 * Parameters: none
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Set display position to home
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_ReturnToHome (void)
{
  HD44780WrComm(HD44780_RETURN);
  return HD44780_BusyCheck(NULL);
}

/*************************************************************************
 * Function Name: HD44780_DisplayShift
 * Parameters: LPC_BOOL DisplayOn, LPC_INT8S DiplayShift
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Shift display
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_DisplayShift (LPC_BOOL DisplayOn,
                                             LPC_INT8S DiplayShift)
{
LPC_INT8U ShiftDir;
  if (DiplayShift >= 0)
  {
    if(DiplayShift > HD44780_MAX_LINE1_ADD)
    {
      return HD44780_ERROR;
    }
    ShiftDir = HD44780_DISPLAY_MOVE_RIGHT;
  }
  else
  {
    if(DiplayShift < -HD44780_MAX_LINE2_ADD)
    {
      return HD44780_ERROR;
    }
    ShiftDir = HD44780_DISPLAY_MOVE_LEFT;
    DiplayShift = 0 - DiplayShift;  /* DiplayShift = -DiplayShift */
  }
  for (int i = 0; i < DiplayShift; ++i)
  {
    HD44780WrComm(HD44780_DISPLAY_MOVE + HD44780_DISPLAY_MOVE_D + ShiftDir);
    if (HD44780_BusyCheck(NULL) != HD44780_OK)
    {
      return HD44780_ERROR;
    }
    if (ShiftDir == HD44780_DISPLAY_MOVE_LEFT)
    {
      if(++HD4478_Ctrl.DiplayPos > 39)
      {
        HD4478_Ctrl.DiplayPos = 0;
      }
    }
    else
    {
      if(--HD4478_Ctrl.DiplayPos > 39) /* --0 = 0xFF > 39 */
      {
        HD4478_Ctrl.DiplayPos = 39;
      }
    }
  }
  return HD44780_OK;
}

/*************************************************************************
 * Function Name: HD44780_CursorPosSet
 * Parameters: LPC_BOOL CursorOn,
 *             HD44780_XY_DEF X,
 *             HD44780_XY_DEF Y
 * Return: HD44780_ERROR_CODE_DEF
 *         HD44780_OK     0: Pass
 *       HD44780_ERROR 1: Busy check Time Out
 * Description: Set position of cursor
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_CursorPosSet (LPC_BOOL CursorOn,
                   LPC_BOOL CursorBlink, HD44780_XY_DEF X, HD44780_XY_DEF Y)
{
LPC_INT8U CursorPos;
  /* Find Addres by coordinate */
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
  if (HD44780_BusyCheck(NULL) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Set Cursor dependence of CursorOn */
  HD4478_Ctrl.CursorOn = CursorOn;
  HD4478_Ctrl.CursorBlink = CursorBlink;
  return HD44780_SetMode();
}

/*************************************************************************
 * Function Name: HD44780_RdCGRAM
 * Parameters: HD44780_STRING_DEF * CG_Data,
 *             LPC_INT8U CGRAM_Add
 * Return: HD44780_ERROR_CODE_DEF
 *        HD44780_OK     0: Pass
 *      HD44780_ERROR  1: Busy check Time Out
 * Description: Read Character patern from CGRAM
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_RdCGRAM (HD44780_STRING_DEF * CG_Data,
                                        LPC_INT8U CGRAM_Add)
{
LPC_INT8U Counter = 8;
LPC_INT8U DDRAM_AddHold;
  if (CGRAM_Add > HD44780_MAX_CGRAM)
  {
    return HD44780_ERROR;
  }
  /* Get current DDRAM address */
  if (HD44780_BusyCheck(&DDRAM_AddHold) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  if (HD4478_Ctrl.DotMode)
  {
    Counter = 10;
    /* Address aling to chracter space */
    CGRAM_Add &= 0xF;
  }
  else
  {
    Counter = 8;
    /* Address aling to chracter space */
    CGRAM_Add &= 0x7;
  }
  /* Set CGRAM Address */
  HD44780WrComm(HD44780_SET_CGRAM_ADD + Counter);
  if (HD44780_BusyCheck(NULL) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  for ( ;Counter; --Counter)
  {
    *CG_Data = HD44780RdData();
    if (HD44780_BusyCheck(NULL) != HD44780_OK)
    {
      break;
    }
    ++CG_Data;
  }
  HD44780WrComm(HD44780_SET_DDRAM_ADD + Counter);
  return((HD44780_ERROR_CODE_DEF)(HD44780_BusyCheck(NULL)
         || (Counter?HD44780_ERROR:HD44780_OK)));
}

/*************************************************************************
 * Function Name: HD44780_WrCGRAM
 * Parameters: const HD44780_STRING_DEF * CG_Data,
 *             LPC_INT8U CGRAM_Add
 * Return: HD44780_ERROR_CODE_DEF
 *        HD44780_OK     0: Pass
 *      HD44780_ERROR  1: Busy check Time Out
 * Description: Write Charcter patern into CGRAM
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_WrCGRAM (const HD44780_STRING_DEF * CG_Data,
                                        LPC_INT8U CGRAM_Add)
{
LPC_INT8U Counter = 8;
LPC_INT8U DDRAM_AddHold;
  if (CGRAM_Add > HD44780_MAX_CGRAM)
  {
    return HD44780_ERROR;
  }
  /* Get current DDRAM address */
  if (HD44780_BusyCheck(&DDRAM_AddHold) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  if (HD4478_Ctrl.DotMode)
  {
    Counter = 10;
    /* Address aling to chracter space */
    CGRAM_Add &= 0xF;
  }
  else
  {
    Counter = 8;
    /* Address aling to chracter space */
    CGRAM_Add &= 0x7;
  }
  /* Set CGRAM Address */
  HD44780WrComm(HD44780_SET_CGRAM_ADD + Counter);
  if (HD44780_BusyCheck(NULL) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  for ( ;Counter; --Counter)
  {
    HD44780WrData(*CG_Data);
    if (HD44780_BusyCheck(NULL) != HD44780_OK)
    {
      break;
    }
    ++CG_Data;
  }
  HD44780WrComm(HD44780_SET_DDRAM_ADD + Counter);
  return((HD44780_ERROR_CODE_DEF)(HD44780_BusyCheck(NULL)
         || (Counter?HD44780_ERROR:HD44780_OK)));
}

/*************************************************************************
 * Function Name: Menu
 * Parameters: none
 * Return: MENU_ERROR_CODE_DEF
 *        HD44780_OK     0: Pass
 *      HD44780_ERROR  1: Busy check TimeOut
 * Description: Show zero terminate string into LCD
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_StrShow(HD44780_XY_DEF X,
                        HD44780_XY_DEF Y, const HD44780_STRING_DEF * DataStr)
{
LPC_INT8U DDRamAdd;
HD44780_ERROR_CODE_DEF ErrorRes = HD44780_OK;
  if(HD44780_GetDDRamAdd(X,Y,&DDRamAdd) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Set Address to DDRAM */
  HD44780WrComm(HD44780_SET_DDRAM_ADD + DDRamAdd);
  if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Write zero terminate string int LCD */
  while (*DataStr)
  {
    ErrorRes = HD44780_CheckVisual(DDRamAdd);
    HD44780WrData(*DataStr);
    if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
    {
      return HD44780_ERROR;
    }
    ++DataStr;
    if((Y == 1) && DDRamAdd > HD44780_MAX_LINE1_ADD)
    {
      HD44780WrComm(HD44780_SET_DDRAM_ADD+HD44780_MIN_LINE1_ADD);
      if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
      {
        return HD44780_ERROR;
      }
    }
    else if ((Y == 2) && DDRamAdd < HD44780_MIN_LINE2_ADD)
    {
      HD44780WrComm(HD44780_SET_DDRAM_ADD+HD44780_MIN_LINE2_ADD);
      if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
      {
        return HD44780_ERROR;
      }
    }
  }
  return ErrorRes;
}

/*************************************************************************
 * Function Name: Menu
 * Parameters: none
 * Return: MENU_ERROR_CODE_DEF
 *        HD44780_OK     0: Pass
 *      HD44780_ERROR  1: Busy check TimeOut
 * Description: Show zero terminate string into LCD
 *
 *************************************************************************/
HD44780_ERROR_CODE_DEF HD44780_CharShow(HD44780_XY_DEF X,
                                        HD44780_XY_DEF Y, char DataChar)
{
  LPC_INT8U DDRamAdd;
  HD44780_ERROR_CODE_DEF ErrorRes = HD44780_OK;
  if(HD44780_GetDDRamAdd(X,Y,&DDRamAdd) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  /* Set Address to DDRAM */
  HD44780WrComm(HD44780_SET_DDRAM_ADD + DDRamAdd);
  if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  ErrorRes = HD44780_CheckVisual(DDRamAdd);
  HD44780WrData(DataChar);
  if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
  {
    return HD44780_ERROR;
  }
  if((Y == 1) && DDRamAdd > HD44780_MAX_LINE1_ADD)
  {
    HD44780WrComm(HD44780_SET_DDRAM_ADD+HD44780_MIN_LINE1_ADD);
    if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
    {
      return HD44780_ERROR;
    }
  }
  else if ((Y == 2) && DDRamAdd < HD44780_MIN_LINE2_ADD)
  {
    HD44780WrComm(HD44780_SET_DDRAM_ADD+HD44780_MIN_LINE2_ADD);
    if (HD44780_BusyCheck(&DDRamAdd) != HD44780_OK)
    {
      return HD44780_ERROR;
    }
  }
  return ErrorRes;
}

/*************************************************************************
 * Function Name: LightInit
 * Parameters: none
 * Return: none
 * Description: Init light control
 *
 *************************************************************************/
void LightInit(void)
{
  /* Connect DAC output to pin P0_25 */
  PINSEL1_bit.P0_25 = 2;
  /* DAC Init */
  DACR_bit.BIAS = 0x1;
  /* Light Off */
  DACR_bit.VALUE = LIGHT_OFF_VALUE;
  /* Init Current state of light */
  LightState = LIGHT_OFF;

}

/*************************************************************************
 * Function Name: LightInit
 * Parameters: LPC_BOOL Slow,
               LPC_BOOL On
 * Return: none
 * Description: Light control
 *
 *************************************************************************/
void LightCntr (LPC_BOOL Slow,LPC_BOOL On)
{
  if(LightState != On)
  {
    if(Slow == LIGHT_SLOW)
    {
      if(On == LIGHT_ON)
      {
        if(DACR_bit.VALUE < LIGHT_ON_VALUE-0xF)
        {
          DACR_bit.VALUE += 0xF;
        }
        else
        {
          LightState = On;
        }
      }
      else
      {
        if(DACR_bit.VALUE > 0xF)
        {
          DACR_bit.VALUE -= 0xF;
        }
        else
        {
          LightState = On;
        }
      }
    }
    else
    {
      LightState = On;
      DACR_bit.VALUE = (On == LIGHT_ON)? LIGHT_ON_VALUE: LIGHT_OFF_VALUE;
    }
  }
}


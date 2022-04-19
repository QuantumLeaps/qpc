/**
  ******************************************************************************
  * @file    hx8347i.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for HX8347I LCD.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hx8347i.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup HX8347I
  * @{
  */

/** @defgroup HX8347I_Private_Types HX8347I Private Types
  * @{
  */
/**
  * @}
  */

/** @defgroup HX8347I_Private_Defines HX8347I Private Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup HX8347I_Private_Variables HX8347I Private Variables
  * @{
  */
/* LCD driver structure initialization */
HX8347I_Drv_t HX8347I_Driver =
{
  HX8347I_Init,
  HX8347I_DeInit,
  HX8347I_ReadID,
  HX8347I_DisplayOn,
  HX8347I_DisplayOff,
  HX8347I_SetBrightness,
  HX8347I_GetBrightness,
  HX8347I_SetOrientation,
  HX8347I_GetOrientation,
  HX8347I_SetCursor,
  HX8347I_DrawBitmap,
  HX8347I_FillRGBRect,
  HX8347I_DrawHLine,
  HX8347I_DrawVLine,
  HX8347I_FillRect,
  HX8347I_GetPixel,
  HX8347I_SetPixel,
  HX8347I_GetXSize,
  HX8347I_GetYSize
};
/**
  * @}
  */

/** @defgroup HX8347I_Private_FunctionPrototypes HX8347I Private Function Prototypes
  * @{
  */
static int32_t HX8347I_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint32_t Length);
static int32_t HX8347I_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint32_t Length);
static void    HX8347I_Delay(HX8347I_Object_t *pObj, uint32_t Delay);
/**
  * @}
  */

/** @addtogroup HX8347I_Exported_Functions
  * @{
  */
/**
  * @brief  Function to register IO bus.
  * @param  pObj Component object pointer.
  * @param  pIO  Component IO pointer.
  * @retval Error status.
  */
int32_t HX8347I_RegisterBusIO(HX8347I_Object_t *pObj, HX8347I_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = HX8347I_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.ReadReg  = HX8347I_ReadRegWrap;
    pObj->Ctx.WriteReg = HX8347I_WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = HX8347I_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the LCD driver component and the control interface.
  * @param  pObj pointer to component object.
  * @param  ColorCoding Color coding.
  * @param  Orientation Orientation.
  * @retval Component status.
  */
int32_t HX8347I_Init(HX8347I_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation)
{
  int32_t ret = HX8347I_OK;
  uint8_t pData[2];

  pData[1] = 0x00U;

  if (pObj->IsInitialized == 0U)
  {
    /* Set color mode */
    pData[0] = (uint8_t) ColorCoding;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLMOD_CTRL, pData, 1); /* default 0x06 262k color,  0x05 65k color */

    /* Set display frame rate */
    pData[0] = 0x36U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_OSC_CTRL1, pData, 1); /* Display frame rate = 60Hz */

    /* Set panel */
    pData[0] = 0x09U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_PANEL_CTRL, pData,
                             1); /* SS_PANEL = 1, GS_PANEL = 0, REV_PANEL = 0, BGR_PANEL = 1 */

    /* Set GRAM Area - Partial Display Control */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_DISPLAY_MODE_CTRL, pData, 1); /* DP_STB=0, DP_STB_S=0, SCROLL=0, */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START2, pData, 1); /* SP */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START1, pData, 1); /* SP */
    if ((Orientation == HX8347I_ORIENTATION_PORTRAIT) || (Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
    {
      /* 320 lines */
      pData[0] = 0x01U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
      pData[0] = 0x3FU;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    }
    else
    {
      /* 240 lines */
      pData[0] = 0x00U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
      pData[0] = 0xEFU;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    }
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START2, pData, 1); /* SC */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START1, pData, 1); /* SC */
    if ((Orientation == HX8347I_ORIENTATION_PORTRAIT) || (Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
    {
      /* 240 columns */
      pData[0] = 0x00U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
      pData[0] = 0xEFU;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
    }
    else
    {
      /* 320 columns */
      pData[0] = 0x01U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
      pData[0] = 0x3FU;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
    }
    if (Orientation == HX8347I_ORIENTATION_PORTRAIT)
    {
      pData[0] = 0x00U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                               1); /* Memory access control: MY = 0, MX = 0, MV = 0, ML = 0 */
    }
    else if (Orientation == HX8347I_ORIENTATION_LANDSCAPE)
    {
      pData[0] = 0xA0U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                               1); /* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
    }
    else if (Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180)
    {
      pData[0] = 0xC0U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                               1); /* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
    }
    else
    {
      pData[0] = 0x60U;
      ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                               1); /* Memory access control: MY = 0, MX = 1, MV =1, ML = 0 */
    }

    /* Power On sequence */
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_OSC_CTRL2, pData, 1); /* OSC_EN = 1 */
    HX8347I_Delay(pObj, 10); /* Wait at least 5ms */
    pData[0] = 0x03U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_POWER_CTRL3, pData, 1); /* AP[2:0] = 011 */
    ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_POWER_CTRL6, pData, 1);
    pData[1] = 0x00U;
    pData[0] &= 0xFEU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_POWER_CTRL6, pData, 1); /* STB = 0 */
    HX8347I_Delay(pObj, 10); /* Wait at least 5ms */
    pData[0] &= 0xF7U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_POWER_CTRL6, pData, 1); /* DK = 0 */
    pData[0] |= 0x10U;
    HX8347I_Delay(pObj, 10); /* Wait at least 5ms */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_POWER_CTRL6, pData, 1); /* PON = 1 */

    HX8347I_Delay(pObj, 10); /* Wait at least 5ms */
    pObj->IsInitialized = 1U;
    pObj->Orientation   = Orientation;
  }

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  De-Initialize the LCD driver component.
  * @param  pObj pointer to component object.
  * @retval Component status.
  */
int32_t HX8347I_DeInit(HX8347I_Object_t *pObj)
{
  int32_t ret = HX8347I_OK;
  uint8_t pData[2];

  if (pObj->IsInitialized != 0U)
  {
    ret += HX8347I_DisplayOff(pObj);

    /* Power Off sequence ---------------------------------------------------------*/
    ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_POWER_CTRL6, pData, 1);
    pData[1] = 0x00U;
    pData[0] &= 0xAFU;
    pData[0] |= 0x08U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_POWER_CTRL6, pData, 1); /* PON = 0, VCOMG = 0 and DK = 1 */
    pData[0] |= 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_POWER_CTRL6, pData, 1); /* STB = 1 */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_OSC_CTRL2, pData, 1); /* OSC_EN = 0 */

    pObj->IsInitialized = 0;
  }

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }
  else
  {
    if (pObj->IO.DeInit != NULL)
    {
      ret = pObj->IO.DeInit();
    }
    else
    {
      ret = HX8347I_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Read the LCD component ID.
  * @param  pObj pointer to component object.
  * @param  Id   pointer to component id.
  * @retval Component status.
  */
int32_t HX8347I_ReadID(HX8347I_Object_t *pObj, uint32_t *Id)
{
  int32_t ret;
  uint8_t hx8347i_id[2];

  /* Get ID from component */
  ret = hx8347i_read_reg(&pObj->Ctx, HX8347I_HIMAX_ID, hx8347i_id, 1);

  *Id = (uint32_t)hx8347i_id[0] | ((uint32_t)hx8347i_id[1] << 8U);

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the display on.
  * @param  pObj pointer to component object.
  * @retval Component status.
  */
int32_t HX8347I_DisplayOn(HX8347I_Object_t *pObj)
{
  int32_t ret = HX8347I_OK;
  uint8_t pData[2];

  pData[1] = 0x00U;

  /* Display On */
  pData[0] = 0x38U;
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_DISPLAY_CTRL3, pData, 1);
  HX8347I_Delay(pObj, 40); /* Wait at least 2 frames */
  pData[0] = 0x3CU;
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_DISPLAY_CTRL3, pData, 1);

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the display off.
  * @param  pObj pointer to component object.
  * @retval Component status.
  */
int32_t HX8347I_DisplayOff(HX8347I_Object_t *pObj)
{
  int32_t ret = HX8347I_OK;
  uint8_t pData[2];

  pData[1] = 0x00U;

  /* Display Off */
  pData[0] = 0x38U;
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_DISPLAY_CTRL3, pData, 1);
  HX8347I_Delay(pObj, 40); /* Wait at least 2 frames */
  pData[0] = 0x04U;
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_DISPLAY_CTRL3, pData, 1);

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the display brightness.
  * @param  pObj Pointer to component object.
  * @param  Brightness Display brightness to be set.
  * @retval Component status.
  */
int32_t HX8347I_SetBrightness(HX8347I_Object_t *pObj, uint32_t Brightness)
{
  /* Feature not supported */
  (void)pObj;
  (void)Brightness;
  return HX8347I_ERROR;
}

/**
  * @brief  Get the display brightness.
  * @param  pObj Pointer to component object.
  * @param  Brightness Current display brightness.
  * @retval Component status.
  */
int32_t HX8347I_GetBrightness(HX8347I_Object_t *pObj, uint32_t *Brightness)
{
  /* Feature not supported */
  (void)pObj;
  (void)Brightness;
  return HX8347I_ERROR;
}

/**
  * @brief  Set the display orientation.
  * @param  pObj Pointer to component object.
  * @param  Orientation Display orientation to be set.
  * @retval Component status.
  */
int32_t HX8347I_SetOrientation(HX8347I_Object_t *pObj, uint32_t Orientation)
{
  int32_t  ret = HX8347I_OK;
  uint8_t  pData[2];

  pData[1] = 0x00U;

  /* Update GRAM Area - Partial Display Control */
  if ((Orientation == HX8347I_ORIENTATION_PORTRAIT) || (Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    /* 320 lines */
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    /* 240 columns */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
  }
  else
  {
    /* 240 lines */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    /* 320 columns */
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
  }
  if (Orientation == HX8347I_ORIENTATION_PORTRAIT)
  {
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                             1); /* Memory access control: MY = 0, MX = 0, MV = 0, ML = 0 */
  }
  else if (Orientation == HX8347I_ORIENTATION_LANDSCAPE)
  {
    pData[0] = 0xA0U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                             1); /* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
  }
  else if (Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180)
  {
    pData[0] = 0xC0U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                             1); /* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
  }
  else
  {
    pData[0] = 0x60U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData,
                             1); /* Memory access control: MY = 0, MX = 1, MV =1, ML = 0 */
  }

  pObj->Orientation   = Orientation;

  return ret;
}

/**
  * @brief  Get the display orientation.
  * @param  pObj Pointer to component object.
  * @param  Orientation Current display orientation.
  * @retval Component status.
  */
int32_t HX8347I_GetOrientation(HX8347I_Object_t *pObj, uint32_t *Orientation)
{
  int32_t  ret = HX8347I_OK;

  *Orientation = pObj->Orientation;

  return ret;
}

/**
  * @brief  Set cursor.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @retval Component status.
  */
int32_t HX8347I_SetCursor(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos)
{
  int32_t ret = HX8347I_OK;
  uint8_t pData[2];

  pData[1] = 0x00U;

  pData[0] = (uint8_t)(Ypos >> 8);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START2, pData, 1);
  pData[0] = (uint8_t)(Ypos & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START1, pData, 1);
  pData[0] = (uint8_t)(Xpos >> 8);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START2, pData, 1);
  pData[0] = (uint8_t)(Xpos & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START1, pData, 1);

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Display a bitmap picture.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  pBmp Pointer to bitmap.
  * @retval Component status.
  */
int32_t HX8347I_DrawBitmap(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  int32_t  ret = HX8347I_OK;
  uint8_t pData[2];
  uint32_t index;
  uint32_t size;
  uint32_t width;
  uint32_t height;
  uint32_t Ystart;
  uint32_t Ystop;

  /* Read file size */
  size = ((uint32_t)pBmp[5] << 24) | ((uint32_t)pBmp[4] << 16) | ((uint32_t)pBmp[3] << 8) | (uint32_t)pBmp[2];
  /* Get bitmap data address offset */
  index = ((uint32_t)pBmp[13] << 24) | ((uint32_t)pBmp[12] << 16) | ((uint32_t)pBmp[11] << 8) | (uint32_t)pBmp[10];
  /* Get image width */
  width = ((uint32_t)pBmp[21] << 24) | ((uint32_t)pBmp[20] << 16) | ((uint32_t)pBmp[19] << 8) | (uint32_t)pBmp[18];
  width--;
  /* Get image height */
  height = ((uint32_t)pBmp[25] << 24) | ((uint32_t)pBmp[24] << 16) | ((uint32_t)pBmp[23] << 8) | (uint32_t)pBmp[22];
  height--;
  /* Get size of data */
  size = size - index;
  size = size / 2U;

  /* Compute new Y start and stop values */
  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    Ystart = 319U - (Ypos + height);
    Ystop  = 319U - Ypos;
  }
  else
  {
    Ystart = 239U - (Ypos + height);
    Ystop  = 239U - Ypos;
  }

  /* Set GRAM Area - Partial Display Control */
  pData[1] = 0x00U;
  pData[0] = (uint8_t)((Xpos >> 8) & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START2, pData, 1); /* SC */
  pData[0] = (uint8_t)(Xpos & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START1, pData, 1); /* SC */
  pData[0] = (uint8_t)(((Xpos + width) >> 8) & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
  pData[0] = (uint8_t)((Xpos + width) & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
  pData[0] = (uint8_t)((Ystart >> 8) & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START2, pData, 1); /* SP */
  pData[0] = (uint8_t)(Ystart & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START1, pData, 1); /* SP */
  pData[0] = (uint8_t)((Ystop >> 8) & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
  pData[0] = (uint8_t)(Ystop & 0xFFU);
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    /* Memory access control: Invert MY */
    ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
    pData[1] = 0x00U;
    if ((pData[0] & 0x80U) != 0U)
    {
      pData[0] &= 0x7FU;
    }
    else
    {
      pData[0] |= 0x80U;
    }
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
  }
  else
  {
    /* Memory access control: Invert MX */
    ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
    pData[1] = 0x00U;
    if ((pData[0] & 0x40U) != 0U)
    {
      pData[0] &= 0xBFU;
    }
    else
    {
      pData[0] |= 0x40U;
    }
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
  }

  /* Write GRAM */
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_READ_DATA, &pBmp[index], size);

  /* Restore GRAM Area - Partial Display Control */
  pData[1] = 0x00U;
  pData[0] = 0x00U;
  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START2, pData, 1); /* SC */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START1, pData, 1); /* SC */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START2, pData, 1); /* SP */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START1, pData, 1); /* SP */
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
  }
  else
  {
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START2, pData, 1); /* SC */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_START1, pData, 1); /* SC */
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START2, pData, 1); /* SP */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_START1, pData, 1); /* SP */
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
  }
  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    /* Memory access control: Re-invert MY */
    ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
    pData[1] = 0x00U;
    if ((pData[0] & 0x80U) != 0U)
    {
      pData[0] &= 0x7FU;
    }
    else
    {
      pData[0] |= 0x80U;
    }
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
  }
  else
  {
    /* Memory access control: Re-invert MX */
    ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
    pData[1] = 0x00U;
    if ((pData[0] & 0x40U) != 0U)
    {
      pData[0] &= 0xBFU;
    }
    else
    {
      pData[0] |= 0x40U;
    }
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
  }

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Fill rectangle with RGB buffer.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  pData Pointer on RGB pixels buffer.
  * @param  Width Width of the rectangle.
  * @param  Height Height of the rectangle.
  * @retval Component status.
  */
int32_t HX8347I_FillRGBRect(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width,
                            uint32_t Height)
{
  int32_t  ret = HX8347I_OK;
  uint8_t  buffer[640];
  uint8_t *pTmp;
  uint32_t i;
  uint32_t j;

  pTmp = pData;

  for (i = 0; i < Height; i++)
  {
    /* Set Cursor */
    ret += HX8347I_SetCursor(pObj, Xpos, Ypos + i);

    /* Sent a complete line */
    for (j = 0; j < Width; j++)
    {
      buffer[2U * j]      = *pTmp;
      pTmp++;
      buffer[(2U * j) + 1U] = *pTmp;
      pTmp++;
    }
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_READ_DATA, buffer, Width);
  }

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Display a horizontal line.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  Length Length of the line.
  * @param  Color  Color of the line.
  * @retval Component status.
  */
int32_t HX8347I_DrawHLine(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t  ret = HX8347I_OK;
  uint8_t  pData[640];
  uint32_t i;

  pData[1] = 0x00U;

  /* Set Cursor */
  ret += HX8347I_SetCursor(pObj, Xpos, Ypos);

  /* Sent a complete line */
  for (i = 0; i < Length; i++)
  {
    pData[2U * i]      = (uint8_t)(Color & 0xFFU);
    pData[(2U * i) + 1U] = (uint8_t)((Color >> 8) & 0xFFU);
  }
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_READ_DATA, pData, Length);

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Display a vertical line.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  Length Length of the line.
  * @param  Color  Color of the line.
  * @retval Component status.
  */
int32_t HX8347I_DrawVLine(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t  ret = HX8347I_OK;
  uint8_t  pData[640];
  uint32_t i;

  pData[1] = 0x00U;

  /* Invert row and column end addresses */
  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
  }
  else
  {
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
  }
  /* Memory access control: Invert MV */
  ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
  pData[1] = 0x00U;
  if ((pData[0] & 0x20U) != 0U)
  {
    pData[0] &= 0xDFU;
  }
  else
  {
    pData[0] |= 0x20U;
  }
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);

  /* Set Cursor */
  ret += HX8347I_SetCursor(pObj, Ypos, Xpos);

  /* Sent a complete line */
  for (i = 0; i < Length; i++)
  {
    pData[2U * i]      = (uint8_t)(Color & 0xFFU);
    pData[(2U * i) + 1U] = (uint8_t)((Color >> 8) & 0xFFU);
  }
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_READ_DATA, pData, Length);

  /* Restore row and column end addresses */
  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    pData[1] = 0x00U;
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
  }
  else
  {
    pData[1] = 0x00U;
    pData[0] = 0x00U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END2, pData, 1); /* EP */
    pData[0] = 0xEFU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_ROW_ADDRESS_END1, pData, 1); /* EP */
    pData[0] = 0x01U;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END2, pData, 1); /* EC */
    pData[0] = 0x3FU;
    ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_COLUMN_ADDRESS_END1, pData, 1); /* EC */
  }
  /* Memory access control: Re-invert MV */
  ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);
  pData[1] = 0x00U;
  if ((pData[0] & 0x20U) != 0U)
  {
    pData[0] &= 0xDFU;
  }
  else
  {
    pData[0] |= 0x20U;
  }
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_MEMORY_ACCESS_CTRL, pData, 1);

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Fill rectangle.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  Width Width of the rectangle.
  * @param  Height Height of the rectangle.
  * @param  Color  Color of the rectangle.
  * @retval Component status.
  */
int32_t HX8347I_FillRect(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height,
                         uint32_t Color)
{
  int32_t  ret = HX8347I_OK;
  uint32_t i;

  for (i = 0U; i < Height; i++)
  {
    if (HX8347I_DrawHLine(pObj, Xpos, (i + Ypos), Width, Color) != HX8347I_OK)
    {
      ret = HX8347I_ERROR;
      break;
    }
  }

  return ret;
}

/**
  * @brief  Get pixel.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  Color Color of the pixel.
  * @retval Component status.
  */
int32_t HX8347I_GetPixel(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
  int32_t  ret = HX8347I_OK;
  uint8_t  pData[6];

  /* Set Cursor */
  ret += HX8347I_SetCursor(pObj, Xpos, Ypos);

  /* read pixel */
  ret += hx8347i_read_reg(&pObj->Ctx, HX8347I_READ_DATA, pData, 3);

  /* Set color in RGB565 format */
  *Color = ((((uint32_t)pData[3] << 8) & 0xF800U) |
            (((uint32_t)pData[2] << 3) & 0x07E0U) |
            (((uint32_t)pData[5] >> 3) & 0x001FU));

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Set pixel.
  * @param  pObj Pointer to component object.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  Color Color of the pixel.
  * @retval Component status.
  */
int32_t HX8347I_SetPixel(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color)
{
  int32_t  ret = HX8347I_OK;

  /* Set Cursor */
  ret += HX8347I_SetCursor(pObj, Xpos, Ypos);

  /* read pixel */
  ret += hx8347i_write_reg(&pObj->Ctx, HX8347I_READ_DATA, (uint8_t *) &Color, 1);

  if (ret != HX8347I_OK)
  {
    ret = HX8347I_ERROR;
  }

  return ret;
}

/**
  * @brief  Get X size.
  * @param  pObj Pointer to component object.
  * @param  Xsize X size of LCD.
  * @retval Component status.
  */
int32_t HX8347I_GetXSize(HX8347I_Object_t *pObj, uint32_t *Xsize)
{
  int32_t  ret = HX8347I_OK;

  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    *Xsize = HX8347I_SIZE_240;
  }
  else
  {
    *Xsize = HX8347I_SIZE_320;
  }

  return ret;
}

/**
  * @brief  Get Y size.
  * @param  pObj Pointer to component object.
  * @param  Ysize Y size of LCD.
  * @retval Component status.
  */
int32_t HX8347I_GetYSize(HX8347I_Object_t *pObj, uint32_t *Ysize)
{
  int32_t  ret = HX8347I_OK;

  if ((pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT) || (pObj->Orientation == HX8347I_ORIENTATION_PORTRAIT_ROT180))
  {
    *Ysize = HX8347I_SIZE_320;
  }
  else
  {
    *Ysize = HX8347I_SIZE_240;
  }

  return ret;
}
/**
  * @}
  */

/** @defgroup HX8347I_Private_Functions HX8347I Private Functions
  * @{
  */
/**
  * @brief  Read register wrapped function.
  * @param  handle  Component object handle.
  * @param  Reg     The target register address to read.
  * @param  pData   The target register value to be red.
  * @param  Length  Buffer size to be red.
  * @retval error status.
  */
static int32_t HX8347I_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint32_t Length)
{
  HX8347I_Object_t *pObj = (HX8347I_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Write register wrapped function.
  * @param  handle Component object handle.
  * @param  Reg    The target register address to write.
  * @param  pData  The target register value to be written.
  * @param  Length Buffer size to be written.
  * @retval error status.
  */
static int32_t HX8347I_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint32_t Length)
{
  HX8347I_Object_t *pObj = (HX8347I_Object_t *)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  HX8347I delay
  * @param  Delay  Delay in ms
  * @retval Component error status
  */
static void HX8347I_Delay(HX8347I_Object_t *pObj, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while ((pObj->IO.GetTick() - tickstart) < Delay)
  {
  }
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

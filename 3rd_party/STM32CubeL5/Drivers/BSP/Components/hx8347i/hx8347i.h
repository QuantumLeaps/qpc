/**
  ******************************************************************************
  * @file    hx8347i.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the hx8347i.c
  *          driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HX8347I_H
#define HX8347I_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hx8347i_reg.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup HX8347I
  * @{
  */

/** @defgroup HX8347I_Exported_Types HX8347I Exported Types
  * @{
  */
typedef int32_t (*HX8347I_Init_Func)(void);
typedef int32_t (*HX8347I_DeInit_Func)(void);
typedef int32_t (*HX8347I_GetTick_Func)(void);
typedef int32_t (*HX8347I_Delay_Func)(uint32_t);
typedef int32_t (*HX8347I_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint32_t);
typedef int32_t (*HX8347I_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint32_t);

typedef struct
{
  HX8347I_Init_Func          Init;
  HX8347I_DeInit_Func        DeInit;
  uint16_t                   Address;
  HX8347I_WriteReg_Func      WriteReg;
  HX8347I_ReadReg_Func       ReadReg;
  HX8347I_GetTick_Func       GetTick;
} HX8347I_IO_t;

typedef struct
{
  HX8347I_IO_t         IO;
  HX8347I_ctx_t        Ctx;
  uint8_t              IsInitialized;
  uint32_t             Orientation;
} HX8347I_Object_t;

typedef struct
{
  /* Control functions */
  int32_t (*Init)(HX8347I_Object_t *, uint32_t, uint32_t);
  int32_t (*DeInit)(HX8347I_Object_t *);
  int32_t (*ReadID)(HX8347I_Object_t *, uint32_t *);
  int32_t (*DisplayOn)(HX8347I_Object_t *);
  int32_t (*DisplayOff)(HX8347I_Object_t *);
  int32_t (*SetBrightness)(HX8347I_Object_t *, uint32_t);
  int32_t (*GetBrightness)(HX8347I_Object_t *, uint32_t *);
  int32_t (*SetOrientation)(HX8347I_Object_t *, uint32_t);
  int32_t (*GetOrientation)(HX8347I_Object_t *, uint32_t *);

  /* Drawing functions*/
  int32_t (*SetCursor)(HX8347I_Object_t *, uint32_t, uint32_t);
  int32_t (*DrawBitmap)(HX8347I_Object_t *, uint32_t, uint32_t, uint8_t *);
  int32_t (*FillRGBRect)(HX8347I_Object_t *, uint32_t, uint32_t, uint8_t *, uint32_t, uint32_t);
  int32_t (*DrawHLine)(HX8347I_Object_t *, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*DrawVLine)(HX8347I_Object_t *, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*FillRect)(HX8347I_Object_t *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*GetPixel)(HX8347I_Object_t *, uint32_t, uint32_t, uint32_t *);
  int32_t (*SetPixel)(HX8347I_Object_t *, uint32_t, uint32_t, uint32_t);
  int32_t (*GetXSize)(HX8347I_Object_t *, uint32_t *);
  int32_t (*GetYSize)(HX8347I_Object_t *, uint32_t *);
} HX8347I_Drv_t;
/**
  * @}
  */

/** @defgroup HX8347I_Exported_Constants HX8347I Exported Constants
  * @{
  */
#define HX8347I_OK                (0)
#define HX8347I_ERROR             (-1)

#define HX8347I_ID    0x0095U

#define HX8347I_ORIENTATION_PORTRAIT           0x00U /* Portrait orientation choice of LCD screen */
#define HX8347I_ORIENTATION_LANDSCAPE          0x01U /* Landscape orientation choice of LCD screen */
#define HX8347I_ORIENTATION_PORTRAIT_ROT180    0x02U /* Portrait with 180 degrees orientation choice of LCD screen */
#define HX8347I_ORIENTATION_LANDSCAPE_ROT180   0x03U /* Landscape with 180 degrees orientation choice of LCD screen */

#define HX8347I_FORMAT_RBG444          0x03U /* Pixel format chosen is RGB444 : 12 bpp (currently not supported) */
#define HX8347I_FORMAT_RBG565          0x05U /* Pixel format chosen is RGB565 : 16 bpp */
#define HX8347I_FORMAT_RBG666          0x06U /* Pixel format chosen is RGB666 : 18 bpp (currently not supported) */

#define HX8347I_SIZE_240 240
#define HX8347I_SIZE_320 320
/**
  * @}
  */

/** @defgroup HX8347I_Exported_Variables HX8347I Exported Variables
  * @{
  */
extern HX8347I_Drv_t HX8347I_Driver;
/**
  * @}
  */

/** @defgroup HX8347I_Exported_Functions HX8347I Exported Functions
  * @{
  */
int32_t HX8347I_RegisterBusIO(HX8347I_Object_t *pObj, HX8347I_IO_t *pIO);
int32_t HX8347I_Init(HX8347I_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation);
int32_t HX8347I_DeInit(HX8347I_Object_t *pObj);
int32_t HX8347I_ReadID(HX8347I_Object_t *pObj, uint32_t *Id);
int32_t HX8347I_DisplayOn(HX8347I_Object_t *pObj);
int32_t HX8347I_DisplayOff(HX8347I_Object_t *pObj);
int32_t HX8347I_SetBrightness(HX8347I_Object_t *pObj, uint32_t Brightness);
int32_t HX8347I_GetBrightness(HX8347I_Object_t *pObj, uint32_t *Brightness);
int32_t HX8347I_SetOrientation(HX8347I_Object_t *pObj, uint32_t Orientation);
int32_t HX8347I_GetOrientation(HX8347I_Object_t *pObj, uint32_t *Orientation);
int32_t HX8347I_SetCursor(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos);
int32_t HX8347I_DrawBitmap(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp);
int32_t HX8347I_FillRGBRect(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width,
                            uint32_t Height);
int32_t HX8347I_DrawHLine(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t HX8347I_DrawVLine(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t HX8347I_FillRect(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height,
                         uint32_t Color);
int32_t HX8347I_GetPixel(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);
int32_t HX8347I_SetPixel(HX8347I_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t HX8347I_GetXSize(HX8347I_Object_t *pObj, uint32_t *Xsize);
int32_t HX8347I_GetYSize(HX8347I_Object_t *pObj, uint32_t *Ysize);
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

#ifdef __cplusplus
}
#endif

#endif /* HX8347I_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

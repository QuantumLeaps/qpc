/**
  ******************************************************************************
  * @file    hx8347i_reg.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the hx8347i_reg.c
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
#ifndef HX8347I_REG_H
#define HX8347I_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup HX8347I
  * @{
  */

/** @defgroup HX8347I_REG_Exported_Constants HX8347I_REG Exported Constants
  * @{
  */
/**
  * @brief  HX8347I Registers Page 0
  */
#define HX8347I_HIMAX_ID                        0x00U
#define HX8347I_DISPLAY_MODE_CTRL               0x01U
#define HX8347I_COLUMN_ADDRESS_START2           0x02U
#define HX8347I_COLUMN_ADDRESS_START1           0x03U
#define HX8347I_COLUMN_ADDRESS_END2             0x04U
#define HX8347I_COLUMN_ADDRESS_END1             0x05U
#define HX8347I_ROW_ADDRESS_START2              0x06U
#define HX8347I_ROW_ADDRESS_START1              0x07U
#define HX8347I_ROW_ADDRESS_END2                0x08U
#define HX8347I_ROW_ADDRESS_END1                0x09U
#define HX8347I_PARTIAL_AREA_START_ROW2         0x0AU
#define HX8347I_PARTIAL_AREA_START_ROW1         0x0BU
#define HX8347I_PARTIAL_AREA_END_ROW2           0x0CU
#define HX8347I_PARTIAL_AREA_END_ROW1           0x0DU
#define HX8347I_VERTICAL_SCROLL_TOP_AREA2       0x0EU
#define HX8347I_VERTICAL_SCROLL_TOP_AREA1       0x0FU
#define HX8347I_VERTICAL_SCROLL_HEIGHT_AREA2    0x10U
#define HX8347I_VERTICAL_SCROLL_HEIGHT_AREA1    0x11U
#define HX8347I_VERTICAL_SCROLL_BUTTON_AREA2    0x12U
#define HX8347I_VERTICAL_SCROLL_BUTTON_AREA1    0x13U
#define HX8347I_VERTICAL_SCROLL_ADDRESS_START2  0x14U
#define HX8347I_VERTICAL_SCROLL_ADDRESS_START1  0x15U
#define HX8347I_MEMORY_ACCESS_CTRL              0x16U
#define HX8347I_COLMOD_CTRL                     0x17U
#define HX8347I_OSC_CTRL1                       0x18U
#define HX8347I_OSC_CTRL2                       0x19U
#define HX8347I_POWER_CTRL1                     0x1AU
#define HX8347I_POWER_CTRL2                     0x1BU
#define HX8347I_POWER_CTRL3                     0x1CU
#define HX8347I_POWER_CTRL4                     0x1DU
#define HX8347I_POWER_CTRL5                     0x1EU
#define HX8347I_POWER_CTRL6                     0x1FU
#define HX8347I_READ_DATA                       0x22U
#define HX8347I_VCOM_CTRL1                      0x23U
#define HX8347I_VCOM_CTRL2                      0x24U
#define HX8347I_VCOM_CTRL3                      0x25U
#define HX8347I_DISPLAY_CTRL1                   0x26U
#define HX8347I_DISPLAY_CTRL2                   0x27U
#define HX8347I_DISPLAY_CTRL3                   0x28U
#define HX8347I_FRAME_CTRL1                     0x29U
#define HX8347I_FRAME_CTRL2                     0x2AU
#define HX8347I_FRAME_CTRL3                     0x2BU
#define HX8347I_FRAME_CTRL4                     0x2CU
#define HX8347I_CYCLE_CTRL1                     0x2DU
#define HX8347I_CYCLE_CTRL2                     0x2EU
#define HX8347I_DISPLAY_INVERSION               0x2FU
#define HX8347I_RGB_CTRL1                       0x31U
#define HX8347I_RGB_CTRL2                       0x32U
#define HX8347I_RGB_CTRL3                       0x33U
#define HX8347I_RGB_CTRL4                       0x34U
#define HX8347I_PANEL_CTRL                      0x36U
#define HX8347I_OTP1                            0x38U
#define HX8347I_OTP2                            0x39U
#define HX8347I_OTP3                            0x3AU
#define HX8347I_CABC_CTRL1                      0x3CU
#define HX8347I_CABC_CTRL2                      0x3DU
#define HX8347I_CABC_CTRL3                      0x3EU
#define HX8347I_CABC_CTRL4                      0x3FU
#define HX8347I_GAMMA_CTRL1                     0x40U
#define HX8347I_GAMMA_CTRL2                     0x41U
#define HX8347I_GAMMA_CTRL3                     0x42U
#define HX8347I_GAMMA_CTRL4                     0x43U
#define HX8347I_GAMMA_CTRL5                     0x44U
#define HX8347I_GAMMA_CTRL6                     0x45U
#define HX8347I_GAMMA_CTRL7                     0x46U
#define HX8347I_GAMMA_CTRL8                     0x47U
#define HX8347I_GAMMA_CTRL9                     0x48U
#define HX8347I_GAMMA_CTRL10                    0x49U
#define HX8347I_GAMMA_CTRL11                    0x4AU
#define HX8347I_GAMMA_CTRL12                    0x4BU
#define HX8347I_GAMMA_CTRL13                    0x4CU
#define HX8347I_GAMMA_CTRL14                    0x50U
#define HX8347I_GAMMA_CTRL15                    0x51U
#define HX8347I_GAMMA_CTRL16                    0x52U
#define HX8347I_GAMMA_CTRL17                    0x53U
#define HX8347I_GAMMA_CTRL18                    0x54U
#define HX8347I_GAMMA_CTRL19                    0x55U
#define HX8347I_GAMMA_CTRL20                    0x56U
#define HX8347I_GAMMA_CTRL21                    0x57U
#define HX8347I_GAMMA_CTRL22                    0x58U
#define HX8347I_GAMMA_CTRL23                    0x59U
#define HX8347I_GAMMA_CTRL24                    0x5AU
#define HX8347I_GAMMA_CTRL25                    0x5BU
#define HX8347I_GAMMA_CTRL26                    0x5CU
#define HX8347I_GAMMA_CTRL27                    0x5DU
#define HX8347I_TE_CTRL                         0x60U
#define HX8347I_ID1                             0x61U
#define HX8347I_ID2                             0x62U
#define HX8347I_ID3                             0x63U
#define HX8347I_DISPLAY_CTRL4                   0x81U
#define HX8347I_DISPLAY_CTRL5                   0x82U
#define HX8347I_TE_OUTPUT_LINE2                 0x84U
#define HX8347I_TE_OUTPUT_LINE1                 0x85U
#define HX8347I_POWER_SAVING_INTERNAL_CTRL1     0xE4U
#define HX8347I_POWER_SAVING_INTERNAL_CTRL2     0xE5U
#define HX8347I_POWER_SAVING_INTERNAL_CTRL3     0xE6U
#define HX8347I_POWER_SAVING_INTERNAL_CTRL4     0xE7U
#define HX8347I_SOURCE_OP_CTRL1                 0xE8U
#define HX8347I_SOURCE_OP_CTRL2                 0xE9U
#define HX8347I_POWER_INTERNAL_CTRL1            0xEAU
#define HX8347I_POWER_INTERNAL_CTRL2            0xEBU
#define HX8347I_SOURCE_INTERNAL_CTRL1           0xECU
#define HX8347I_SOURCE_INTERNAL_CTRL2           0xEDU
#define HX8347I_POWER_SAVING_INTERNAL_CTRL5     0xF3U
#define HX8347I_POWER_SAVING_INTERNAL_CTRL6     0xF4U
#define HX8347I_PAGE_SELECT                     0xFFU

/**
  * @brief  HX8347I Registers Page 1
  */
#define HX8347I_DGC_CTRL                        0x00U
#define HX8347I_DGC_LUT_START                   0x01U  /* 99 bytes LUT 0x01U-0x63*/
#define HX8347I_CE_CTRL1                        0x70U
#define HX8347I_CE_CTRL2                        0x71U
#define HX8347I_CE_CTRL3                        0x72U
#define HX8347I_CE_CTRL4                        0x73U
#define HX8347I_CE_CTRL5                        0x74U
#define HX8347I_CE_CTRL6                        0x75U
#define HX8347I_CE_CTRL7                        0x76U
#define HX8347I_CABC_CTRL5                      0xC3U
#define HX8347I_CABC_CTRL6                      0xC5U
/**
  * @}
  */

/** @defgroup HX8347I_REG_Exported_Types HX8347I_REG Exported Types
  * @{
  */
typedef int32_t (*HX8347I_Write_Func)(void *, uint16_t, uint8_t *, uint32_t);
typedef int32_t (*HX8347I_Read_Func)(void *, uint16_t, uint8_t *, uint32_t);

typedef struct
{
  HX8347I_Write_Func   WriteReg;
  HX8347I_Read_Func    ReadReg;
  void                 *handle;
} HX8347I_ctx_t;
/**
  * @}
  */

/** @defgroup HX8347I_REG_Exported_Functions HX8347I_REG Exported Functions
  * @{
  */
int32_t hx8347i_write_reg(HX8347I_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint32_t length);
int32_t hx8347i_read_reg(HX8347I_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint32_t length);
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

#endif /* HX8347I_REG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

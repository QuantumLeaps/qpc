/**
  ******************************************************************************
  * @file    hx8347i_reg.c
  * @author  MCD Application Team
  * @brief   This file provides unitary register function to control the HX8347I
  *          LCD driver.
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
#include "hx8347i_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup HX8347I
  * @{
  */

/** @addtogroup HX8347I_REG_Exported_Functions
  * @{
  */
/*******************************************************************************
  * Function Name : hx8347i_read_reg.
  * Description   : Generic Reading function.
  * Input         : Driver context, register Address, length of buffer.
  * Output        : Status.
  *******************************************************************************/
int32_t hx8347i_read_reg(HX8347I_ctx_t *ctx, uint16_t reg, uint8_t *data, uint32_t length)
{
  return ctx->ReadReg(ctx->handle, reg, data, length);
}

/*******************************************************************************
  * Function Name : hx8347i_write_reg.
  * Description   : Generic Writing function.
  * Input         : Driver context, Register Address, data to be written,
                  length of buffer.
  * Output        : Status.
  *******************************************************************************/
int32_t hx8347i_write_reg(HX8347I_ctx_t *ctx, uint16_t reg, uint8_t *data, uint32_t length)
{
  return ctx->WriteReg(ctx->handle, reg, data, length);
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

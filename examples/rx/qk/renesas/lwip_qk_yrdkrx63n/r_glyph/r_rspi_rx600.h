/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rspi_rx600.h
* Version      : 1.10
* Description  : Functions for using RSPI on RX600 devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 03.08.2011 1.00    First Release
*         : 30.11.2011 1.10    Added RX63x support
***********************************************************************************************************************/

#ifndef RSPI_API_HEADER_FILE
#define RSPI_API_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Used for configuring the RSPI code */
#include "r_rspi_rx600_config.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Enumeration for chips selects associated with YRDKRX62N RSPI-0 */
typedef enum {
    NO_DEVICE_SELECTED=0,
    SDMICRO_SELECTED,
    FLASH_SELECTED,
    WIFI_SELECTED,
    LCD_SELECTED
} device_selected_t;

/***********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
bool R_RSPI_Init(uint8_t channel);
bool R_RSPI_Select(uint8_t channel, uint16_t chip_select, uint32_t pid);
bool R_RSPI_Deselect(uint8_t channel, uint16_t chip_select, uint32_t pid);
bool R_RSPI_BaudRateSet(uint8_t channel, uint8_t divisor, uint32_t pid);
bool R_RSPI_SendReceive(uint8_t channel, uint8_t const *pSrc, uint8_t *pDest, uint16_t usBytes, uint32_t pid);
bool R_RSPI_Read(uint8_t channel, uint8_t *pDest, uint16_t usBytes, uint32_t pid);
bool R_RSPI_Write(uint8_t channel, const uint8_t *pSrc, uint16_t usBytes, uint32_t pid);
bool R_RSPI_Lock(uint8_t channel, uint32_t pid);
bool R_RSPI_Unlock(uint8_t channel, uint32_t pid);
bool R_RSPI_CheckLock(uint8_t channel, uint32_t pid);

#endif /* RSPI_API_HEADER_FILE */


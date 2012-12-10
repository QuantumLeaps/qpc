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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name       : mcu_info.h
* Version      : 1.0
* Device(s)    : RX
* H/W Platform : YRDKRX63N
* Description  : Information about the MCU on this board (RDKRX63N).
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2011 1.00     First Release
***********************************************************************************************************************/

#ifndef _MCU_INFO
#define _MCU_INFO

/* MCU that is used. */
#define MCU_RX63N           (1)

/* Package. */
#define PACKAGE_LQFP100     (1)

/* Memory size of your MCU. */
#define ROM_SIZE_BYTES      (1048576)
#define RAM_SIZE_BYTES      (131072)
#define DF_SIZE_BYTES       (32768)

/* System clock speed in Hz. */
#define ICLK_HZ             (96000000)
/* Peripheral clock speed in Hz. */
#define PCLK_HZ             (48000000)
/* External bus clock speed in Hz. */
#define BCLK_HZ             (24000000)
/* FlashIF clock speed in Hz. */
#define FCLK_HZ             (48000000)

#endif /* _MCU_INFO */


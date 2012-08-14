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
* File Name     : r_rspi_rx600_config.c
* Version       : 1.10
* Description   : Configures the RSPI drivers
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 03.08.2011 1.00    First Release
*         : 30.11.2011 1.10    Added RX63x support
***********************************************************************************************************************/
#ifndef RSPI_CONFIG_HEADER_FILE
#define RSPI_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* If this definition is uncommented then obtaining a lock on the RSPI peripheral using the R_RSPI_Lock() function will
   be required. After obtaining the lock the user will have to pass the tasks PID to all other RSPI functions. If this
   is commented out then locking will not be enabled and the user can pass any value they want for 'pid' because the
   functions will ignore the lock. */
#define RSPI_REQUIRE_LOCK

#endif /* RSPI_CONFIG_HEADER_FILE */

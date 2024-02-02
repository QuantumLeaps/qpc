//!
//!*****************************************************************************
//! @file    UART/UART_WakeUpFromStopUsingFIFO/Src/stm32h7xx_hal_msp.c
//! @author  MCD Application Team
//! @version V1.2.0
//! @date    29-December-2017
//! @brief   HAL MSP module.
//!*****************************************************************************
//! @attention
//!
//! <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
//!
//! Redistribution and use in source and binary forms, with or without modification,
//! are permitted provided that the following conditions are met:
//!   1. Redistributions of source code must retain the above copyright notice,
//!      this list of conditions and the following disclaimer.
//!   2. Redistributions in binary form must reproduce the above copyright notice,
//!      this list of conditions and the following disclaimer in the documentation
//!      and/or other materials provided with the distribution.
//!   3. Neither the name of STMicroelectronics nor the names of its contributors
//!      may be used to endorse or promote products derived from this software
//!      without specific prior written permission.
//!
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//! DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//! FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//! DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//! SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//! CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//! OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//! OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//!
//!*****************************************************************************
//!

// Includes ------------------------------------------------------------------
//#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"

//! @addtogroup STM32H7xx_HAL_Examples
//! @{
//!

//! @defgroup HAL_MSP
//! @brief HAL MSP module.
//! @{
//!

// User can use this section to tailor USARTx/UARTx instance used and associated
// resources
// Definition for USARTx clock resources
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

// Definition for USARTx Pins
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3

//! @addtogroup STM32F7xx_HAL_Examples
//! @{
//!

//! @defgroup UART_TwoBoards_ComPolling
//! @{
//!

// Private typedef -----------------------------------------------------------
// Private define ------------------------------------------------------------
// Private macro -------------------------------------------------------------
// Private variables ---------------------------------------------------------
// Private function prototypes -----------------------------------------------
// Private functions ---------------------------------------------------------

//! @defgroup HAL_MSP_Private_Functions
//! @{
//!

// NOTE:
// STM32Cube's initialization calls the weakly defined HAL_InitTick(), which
// by default configures and starts the Systick interrupt. This is TOO EARLY,
// because the system os NOT ready yet to handle interrupts.
// To avoid problems, a dummy definition for HAL_InitTick() is provided
// in the file stm32l5xx_hal_msp.c. The SystTick is configured and
// started later in QF_onStartup().
//
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    (void)TickPriority; // unused parameter
    return HAL_OK;
}

//!
//! @brief UART MSP Initialization
//!       This function configures the hardware resources used in this example:
//!          - Peripheral's clock enable
//!          - Peripheral's GPIO Configuration
//! @param huart: UART handle pointer
//! @retval None
//!
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {

    //##-1- Enable peripherals and GPIO Clocks #################################
    // Enable GPIO TX/RX clock
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();

    // Enable USARTx clock
    USARTx_CLK_ENABLE();

    //##-2- Configure peripheral GPIO ##########################################
    // UART TX GPIO pin configuration
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;
    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

    // UART RX GPIO pin configuration
    GPIO_InitStruct.Pin       = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;
    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

    //##-3- Configure the NVIC for UART ########################################
    // NVIC for USART
    // this is delayed till QF_onStartup()
}

//!
//! @brief UART MSP De-Initialization
//!       This function frees the hardware resources used in this example:
//!         - Disable the Peripheral's clock
//!         - Revert GPIO and NVIC configuration to their default state
//! @param huart: UART handle pointer
//! @retval None
//!
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    //##-1- Reset peripherals ##################################################
    USARTx_FORCE_RESET();
    USARTx_RELEASE_RESET();

    //##-2- Disable peripherals and GPIO Clocks ################################
    // Configure USART Tx as alternate function
    HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
    // Configure USART Rx as alternate function
    HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

    //##-3- Disable the NVIC for UART ##########################################
    // this is delayed till QF_onCleanup()
}

// dummy legacy callbacks for STM32CubeH7
void HAL_UARTEx_RxFifoFullCallback(UART_HandleTypeDef *huart) {
    (void)huart; // unused parameter
}

void HAL_UARTEx_TxFifoEmptyCallback(UART_HandleTypeDef *huart) {
    (void)huart; // unused parameter
}

void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef *huart) {
    (void)huart; // unused parameter
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    (void)huart; // unused parameter
}

//!
//! @}
//!

//!
//! @}
//!

//!
//! @}
//!

//*********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE*****

/*
 * uart.c
 *
 *  Created on: Sep 27, 2026
 *      Author: Lenovo
 */
#include "uart.h"
#include <stdio.h>

/*==========================================================
 * Global UART2 Handle
 *==========================================================*/

UART_HandleTypeDef huart2;


/*==========================================================
 * UART2 Initialization
 *
 * USART2:
 *
 * PA2 -> TX
 * PA3 -> RX
 *
 * Baud Rate  = 115200
 * Data       = 8 bits
 * Parity     = None
 * Stop Bits  = 1
 *==========================================================*/

void UART2_Init(void)
{
    huart2.Instance = USART2;

    huart2.Init.BaudRate     = 115200;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        while (1)
        {
        }
    }
}


/*==========================================================
 * UART MSP Initialization
 *
 * HAL_UART_Init()
 *        |
 *        +----> HAL_UART_MspInit()
 *                    |
 *                    +----> Enable GPIO clock
 *                    +----> Enable USART clock
 *                    +----> Configure GPIO
 *==========================================================*/

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (huart->Instance == USART2)
    {
        /* Enable GPIOA clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Enable USART2 clock */
        __HAL_RCC_USART2_CLK_ENABLE();

        /*
         * PA2 = USART2_TX
         *
         * Alternate Function 7
         */

        GPIO_InitStruct.Pin       = GPIO_PIN_2;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}


/*==========================================================
 * printf() Redirection
 *
 * printf("Hello STM32 UART!");
 *
 *        |
 *        v
 *
 * __io_putchar()
 *
 *        |
 *        v
 *
 * HAL_UART_Transmit()
 *==========================================================*/

int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2,
                      (uint8_t *)&ch,
                      1,
                      HAL_MAX_DELAY);

    return ch;
}

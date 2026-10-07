/*
 * uart.c
 *
 * USART2 TX driver
 *
 * Created on: Sep 27, 2026
 * Author: Lenovo
 */

#include "uart.h"          /* Include UART interface header */
#include <stdio.h>         /* Include standard I/O for printf */


/*
 * Global UART handle for USART2 peripheral.
 */
UART_HandleTypeDef huart2;


/*
 * Initialize USART2 peripheral.
 */
void UART2_Init(void)
{
    /*
     * Assign USART2 peripheral register base address.
     */
    huart2.Instance = USART2;


    /*
     * Configure UART communication parameters.
     *
     * Baud rate = 115200 bps
     */
    huart2.Init.BaudRate = 115200;


    /*
     * Configure 8 data bits.
     */
    huart2.Init.WordLength = UART_WORDLENGTH_8B;


    /*
     * Configure 1 stop bit.
     */
    huart2.Init.StopBits = UART_STOPBITS_1;


    /*
     * Disable parity.
     */
    huart2.Init.Parity = UART_PARITY_NONE;


    /*
     * Configure UART in transmit-only mode.
     */
    huart2.Init.Mode = UART_MODE_TX;


    /*
     * Disable hardware flow control.
     */
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;


    /*
     * Configure 16x oversampling.
     */
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;


    /*
     * Initialize USART2 peripheral.
     */
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        /*
         * Stay here if UART initialization fails.
         */
        while (1);
    }
}


/*
 * Low-level hardware initialization callback
 * called automatically by HAL_UART_Init().
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /*
     * Check whether USART2 is being initialized.
     */
    if (huart->Instance == USART2)
    {
        /*
         * Enable GPIOA peripheral clock.
         */
        __HAL_RCC_GPIOA_CLK_ENABLE();


        /*
         * Enable USART2 peripheral clock.
         */
        __HAL_RCC_USART2_CLK_ENABLE();


        /*
         * PA2 -> USART2_TX.
         */
        GPIO_InitStruct.Pin = GPIO_PIN_2;


        /*
         * Configure PA2 as Alternate Function
         * Push-Pull.
         */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;


        /*
         * Disable internal pull-up/pull-down.
         */
        GPIO_InitStruct.Pull = GPIO_NOPULL;


        /*
         * Configure GPIO output speed.
         */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;


        /*
         * Select Alternate Function 7 for USART2.
         */
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;


        /*
         * Apply GPIO configuration.
         */
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}


/*
 * Redirect printf() character output to USART2.
 */
int __io_putchar(int ch)
{
    /*
     * Transmit one character through USART2.
     */
    HAL_UART_Transmit(
        &huart2,
        (uint8_t *)&ch,
        1,
        HAL_MAX_DELAY
    );


    /*
     * Return transmitted character.
     */
    return ch;
}

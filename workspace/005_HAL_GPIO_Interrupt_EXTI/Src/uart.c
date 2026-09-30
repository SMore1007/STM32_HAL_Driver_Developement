/*
 * uart.c
 *
 *  Created on: Sep 27, 2026
 *      Author: Lenovo
 */
#include "uart.h"                               /* Include UART interface driver header file */
#include <stdio.h>                              /* Include standard I/O header for putchar definition */

UART_HandleTypeDef huart2;                      /* Global UART handle for USART2 peripheral */

void UART2_Init(void)                           /* Function to initialize USART2 peripheral configuration */
{                                               /* Start of UART2_Init function */
    huart2.Instance = USART2;                   /* Assign USART2 peripheral register base address */
    huart2.Init.BaudRate     = 115200;          /* Set UART communication baud rate to 115200 bps */
    huart2.Init.WordLength   = UART_WORDLENGTH_8B; /* Configure 8 data bits word length */
    huart2.Init.StopBits     = UART_STOPBITS_1; /* Configure 1 stop bit */
    huart2.Init.Parity       = UART_PARITY_NONE;/* Disable parity check */
    huart2.Init.Mode         = UART_MODE_TX;    /* Configure UART in Transmit-only (TX) mode */
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE; /* Disable hardware flow control (RTS/CTS) */
    huart2.Init.OverSampling = UART_OVERSAMPLING_16; /* Set 16x oversampling mode */

    if (HAL_UART_Init(&huart2) != HAL_OK)       /* Initialize USART2 peripheral with HAL driver */
    {                                           /* Start error block */
        while (1);                              /* Infinite loop on UART initialization failure */
    }                                           /* End error block */
}                                               /* End of UART2_Init function */

void HAL_UART_MspInit(UART_HandleTypeDef *huart) /* Low-level hardware (MSP) initialization callback for UART */
{                                               /* Start of HAL_UART_MspInit function */
    GPIO_InitTypeDef GPIO_InitStruct = {0};     /* Declare and zero-initialize GPIO config struct */

    if (huart->Instance == USART2)              /* Check if initialization is requested for USART2 */
    {                                           /* Start USART2 hardware setup */
        __HAL_RCC_GPIOA_CLK_ENABLE();           /* Enable clock for GPIO Port A */
        __HAL_RCC_USART2_CLK_ENABLE();          /* Enable clock for USART2 peripheral */

        GPIO_InitStruct.Pin       = GPIO_PIN_2;  /* Select GPIO Pin 2 (PA2 - USART2_TX) */
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP; /* Configure as Alternate Function Push-Pull mode */
        GPIO_InitStruct.Pull      = GPIO_NOPULL; /* Disable internal pull-up / pull-down resistor */
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH; /* Set GPIO output speed to Very High */
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2; /* Connect PA2 to Alternate Function 7 (USART2) */

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); /* Initialize PA2 pin with specified configuration */
    }                                           /* End USART2 hardware setup */
}                                               /* End of HAL_UART_MspInit function */

int __io_putchar(int ch)                        /* Low-level I/O putchar override for printf redirection */
{                                               /* Start of __io_putchar function */
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); /* Transmit single character over USART2 in blocking mode */
    return ch;                                  /* Return character sent */
}                                               /* End of __io_putchar function */

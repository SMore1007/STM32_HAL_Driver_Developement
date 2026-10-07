/*
 * uart.c
 *
 * Driver module for USART1 peripheral setup (Interrupt Mode on PA9/PA10)
 * and USART2 setup (printf() retargeting on PA2).
 *
 * Hardware Mapping (NUCLEO-F446RE):
 * - PA9  -> USART1_TX (Jumper wire connected to PA10 RX for loopback)
 * - PA10 -> USART1_RX (Jumper wire connected to PA9 TX for loopback)
 * - PA2  -> USART2_TX (Connected to ST-LINK Virtual COM Port at 115200 baud)
 */

#include <stdint.h>
#include <stdio.h>                             /* Include standard C I/O library for printf */
#include "uart.h"                              /* Include UART interface driver header */

/* Global UART handle structures */
UART_HandleTypeDef huart1;                     /* Handle for USART1 (Interrupt Mode) */
UART_HandleTypeDef huart2;                     /* Handle for USART2 (printf retargeting) */

/* Global flags and counters for UART interrupt handling */
volatile uint8_t g_tx_complete_flag = 0;
volatile uint8_t g_rx_complete_flag = 0;
volatile uint8_t g_uart_error_flag = 0;
volatile uint32_t TxCounter = 0;
volatile uint32_t RxCounter = 0;


/*
 * Configure USART1 peripheral parameters (115200 baud, 8N1, TX/RX mode).
 */
void UART1_Init(void)
{
    /* Point handle instance to USART1 base register structure */
    huart1.Instance = USART1;

    /* Configure communication parameters */
    huart1.Init.BaudRate = 115200;                             /* Set baud rate = 115200 bits per second */
    huart1.Init.WordLength = UART_WORDLENGTH_8B;                /* Set frame length = 8 data bits */
    huart1.Init.StopBits = UART_STOPBITS_1;                    /* Set 1 stop bit */
    huart1.Init.Parity = UART_PARITY_NONE;                      /* Disable parity control */
    huart1.Init.Mode = UART_MODE_TX_RX;                         /* Configure Transmit and Receive mode (TX/RX) */
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;                /* Disable hardware flow control */
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;            /* Set 16x oversampling */

    /* Apply parameters to USART1 hardware registers; calls HAL_UART_MspInit() internally */
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        while (1);                                              /* Trap CPU if UART initialization fails */
    }
}


/*
 * Configure USART2 peripheral parameters for printf serial logging (115200 baud, 8N1, TX mode).
 */
void UART2_Init(void)
{
    /* Point handle instance to USART2 base register structure */
    huart2.Instance = USART2;

    /* Configure communication parameters */
    huart2.Init.BaudRate = 115200;                             /* Set baud rate = 115200 bits per second */
    huart2.Init.WordLength = UART_WORDLENGTH_8B;                /* Set frame length = 8 data bits */
    huart2.Init.StopBits = UART_STOPBITS_1;                    /* Set 1 stop bit */
    huart2.Init.Parity = UART_PARITY_NONE;                      /* Disable parity control */
    huart2.Init.Mode = UART_MODE_TX;                            /* Configure Transmit-only mode (TX) */
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;                /* Disable CTS/RTS hardware flow control */
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;            /* Set 16x oversampling */

    /* Apply parameters to USART2 hardware registers; calls HAL_UART_MspInit() internally */
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        while (1);                                              /* Trap CPU if UART initialization fails */
    }
}


/*
 * Low-level hardware (MSP) initialization callback automatically invoked by HAL_UART_Init().
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};                     /* Zero-initialize GPIO configuration structure */

    /* MSP initialization for USART1 peripheral (PA9 TX, PA10 RX) */
    if (huart->Instance == USART1)
    {
        /* Step 1: Enable peripheral clock for GPIOA (PA9 and PA10 pins) */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Step 2: Enable peripheral clock for USART1 hardware block (APB2 bus) */
        __HAL_RCC_USART1_CLK_ENABLE();

        /* Step 3: Configure PA9 (TX) and PA10 (RX) as Alternate Function 7 (USART1) */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;         /* Select Pin 9 (PA9 TX) and Pin 10 (PA10 RX) */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                /* Alternate Function Push-Pull mode */
        GPIO_InitStruct.Pull = GPIO_NOPULL;                    /* Disable internal pull resistors */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* High output speed */
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;           /* Route pin multiplexer to AF7 (USART1) */

        /* Apply GPIO configuration to GPIOA */
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Step 4: Configure NVIC priority and enable USART1 interrupt line */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);               /* Set highest priority for serial interrupt */
        HAL_NVIC_EnableIRQ(USART1_IRQn);                       /* Enable USART1 IRQ channel in NVIC */
    }
    /* MSP initialization for USART2 peripheral (PA2 TX for printf) */
    else if (huart->Instance == USART2)
    {
        /* Step 1: Enable peripheral clock for GPIOA (PA2 pin) */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Step 2: Enable peripheral clock for USART2 hardware block (APB1 bus) */
        __HAL_RCC_USART2_CLK_ENABLE();

        /* Step 3: Configure PA2 pin as Alternate Function 7 (USART2_TX) */
        GPIO_InitStruct.Pin = GPIO_PIN_2;                      /* Select Pin 2 (PA2) */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                /* Set mode to Alternate Function Push-Pull */
        GPIO_InitStruct.Pull = GPIO_NOPULL;                    /* Disable internal pull resistors */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* High output speed */
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;           /* Route pin multiplexer to AF7 (USART2) */

        /* Apply GPIO configuration to GPIOA */
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}


/*
 * USART1 Interrupt Service Routine (ISR)
 *
 * Invoked when USART1 transmit or receive interrupt condition occurs.
 */
void USART1_IRQHandler(void)
{
    /* Clear peripheral interrupt flags and execute HAL UART state machine callback */
    HAL_UART_IRQHandler(&huart1);
}


/*
 * HAL UART Transmit Complete Callback
 *
 * Automatically called by HAL_UART_IRQHandler() when all bytes in TxBuffer are transmitted.
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        TxCounter++;
        g_tx_complete_flag = 1;
    }
}


/*
 * HAL UART Receive Complete Callback
 *
 * Automatically called by HAL_UART_IRQHandler() when requested bytes are received into RxBuffer.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        RxCounter++;
        g_rx_complete_flag = 1;
    }
}


/*
 * HAL UART Error Callback
 *
 * Automatically called by HAL_UART_IRQHandler() if hardware framing, overrun, or noise error occurs.
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        g_uart_error_flag = 1;
    }
}


/*
 * Redirect standard C library printf() character output to USART2 (ST-LINK Virtual COM Port).
 *
 * GCC C runtime library (newlib) calls __io_putchar() for each character emitted by printf().
 */
int __io_putchar(int ch)
{
    /* Transmit single character byte over USART2 peripheral with maximum delay timeout */
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);

    /* Return character integer to signal successful output byte */
    return ch;
}


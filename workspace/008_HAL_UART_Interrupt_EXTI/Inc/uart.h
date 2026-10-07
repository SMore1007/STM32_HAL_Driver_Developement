/*
 * uart.h
 *
 * Header file for modular USART1 (Interrupt TX/RX) and USART2 (printf retargeting) drivers.
 */

#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"                     /* Include standard STM32F4 HAL library header */

/* Size of transmit and receive buffers for loopback test */
#define DATA_BUFFER_SIZE  10

/* External handle declarations for USART peripherals */
extern UART_HandleTypeDef huart1;              /* USART1 handle (PA9 TX, PA10 RX - Interrupt Mode) */
extern UART_HandleTypeDef huart2;              /* USART2 handle (PA2 TX - printf retargeting) */

/* Global interrupt completion flags and event counters (volatile for ISR safety) */
extern volatile uint8_t g_tx_complete_flag;
extern volatile uint8_t g_rx_complete_flag;
extern volatile uint8_t g_uart_error_flag;
extern volatile uint32_t TxCounter;
extern volatile uint32_t RxCounter;

/* Function prototypes */
void UART1_Init(void);                         /* Configures USART1 (PA9/PA10) in TX/RX interrupt mode */
void UART2_Init(void);                         /* Configures USART2 (PA2) for printf retargeting */

#endif /* UART_H */


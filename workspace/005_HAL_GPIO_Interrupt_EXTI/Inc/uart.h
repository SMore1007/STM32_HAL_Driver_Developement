/*
 * uart.h
 *
 *  Created on: Sep 27, 2026
 *      Author: Lenovo
 */
#ifndef UART_H                                  /* Include guard: prevent multiple inclusion of uart.h */
#define UART_H                                  /* Define UART_H macro for include guard */

#include "stm32f4xx_hal.h"                     /* Include main STM32F4 HAL library header */

extern UART_HandleTypeDef huart2;               /* External declaration for USART2 global handle */

void UART2_Init(void);                         /* Prototype for UART2 initialization function */

#endif /* UART_H */                             /* End of include guard for UART_H */

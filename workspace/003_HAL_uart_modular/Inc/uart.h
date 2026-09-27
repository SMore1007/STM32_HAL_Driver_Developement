/*
 * uart.h
 *
 *  Created on: Sep 27, 2026
 *      Author: Lenovo
 */
#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"

/*----------------------------------------------------------
 * UART2 Public Interface
 *----------------------------------------------------------*/

/* UART2 handle */
extern UART_HandleTypeDef huart2;


/* Initialize UART2 */
void UART2_Init(void);


#endif /* UART_H */

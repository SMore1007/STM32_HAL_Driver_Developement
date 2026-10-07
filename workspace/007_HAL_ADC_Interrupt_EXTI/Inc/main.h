/*
 * main.h
 *
 * Header file containing main application definitions and function prototypes.
 */

#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx_hal.h"                     /* Include standard STM32F4 HAL framework header */

/* Onboard User Push Button Pin Definitions (PC13 on Nucleo-F446RE) */
#define BTN_PORT    GPIOC                      /* Port C for Blue User Button */
#define BTN_PIN     GPIO_PIN_13                /* Pin 13 mapped to EXTI Line 13 */

/* Onboard User LED Pin Definitions (PA5 on Nucleo-F446RE) */
#define LED_PORT    GPIOA                      /* Port A for Green LED */
#define LED_PIN     GPIO_PIN_5                 /* Pin 5 mapped to GPIO Output */

/* Function Prototypes */
void SystemClock_Config(void);                 /* Configures 16 MHz HSI System Clock */
void MX_GPIO_Init(void);                       /* Configures PC13 EXTI input and PA5 LED output */

#endif /* MAIN_H */


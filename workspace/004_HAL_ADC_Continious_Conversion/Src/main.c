#include <stdio.h>                              /* Include standard I/O library for printf function */
#include "main.h"                               /* Include main application header file */
#include "uart.h"                               /* Include UART interface driver header file */
#include "adc.h"                                /* Include ADC interface driver header file */

/* Global Variable */
uint32_t SensorValue = 0U;                      /* Global variable to store ADC conversion result */

/* Main entry point of application */
int main(void)                                  /* Main function definition */
{                                               /* Start of main function body */
    HAL_Init();                                 /* Initialize STM32 HAL Library and SysTick timer */
    SystemClock_Config();                       /* Configure system clock to HSI 16 MHz */
    UART2_Init();                               /* Initialize USART2 peripheral for printf logging */
    Adc3InitStart();                            /* Initialize ADC3 hardware module and GPIO PA0 */

    printf("Setup done\r\n");                   /* Print initialization complete message to console */

    while (1)                                   /* Infinite application loop */
    {                                           /* Start of infinite loop */
        SensorValue = ReadAdc3PA0();            /* Trigger conversion and read ADC value from PA0 */
        printf("ADC Value: %ld\r\n", SensorValue); /* Print converted ADC value over UART */
        HAL_Delay(100);                         /* Delay 100 milliseconds between readings */
    }                                           /* End of infinite loop */
}                                               /* End of main function */

/* Configure System Clock to 16 MHz using HSI */
void SystemClock_Config(void)                   /* System clock configuration function */
{                                               /* Start of clock configuration */
    RCC_OscInitTypeDef RCC_OscInitStruct = {0}; /* Declare and zero-initialize oscillator config struct */
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; /* Declare and zero-initialize clock config struct */

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; /* Select High-Speed Internal (HSI) oscillator */
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                  /* Turn ON the HSI oscillator */
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; /* Set default HSI calibration value */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;              /* Disable PLL as 16 MHz HSI is sufficient */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)     /* Configure RCC oscillator, check for error */
    {                                           /* Start error block */
        while (1);                              /* Trap CPU in infinite loop on clock error */
    }                                           /* End error block */

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2; /* Configure SYSCLK, HCLK, PCLK1, and PCLK2 */
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;   /* Set HSI as SYSCLK source */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        /* Set AHB prescaler to 1 (HCLK = 16 MHz) */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         /* Set APB1 prescaler to 1 (PCLK1 = 16 MHz) */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         /* Set APB2 prescaler to 1 (PCLK2 = 16 MHz) */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) /* Configure bus clocks and flash latency */
    {                                           /* Start error block */
        while (1);                              /* Trap CPU in infinite loop on clock config failure */
    }                                           /* End error block */
}                                               /* End of SystemClock_Config */

/* SysTick interrupt handler for HAL delays */
void SysTick_Handler(void)                      /* SysTick interrupt handler function */
{                                               /* Start of handler */
    HAL_IncTick();                              /* Increment HAL tick count every 1 ms */
}                                               /* End of SysTick_Handler */

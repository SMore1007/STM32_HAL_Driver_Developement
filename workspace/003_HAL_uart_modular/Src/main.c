#include "main.h"
#include "uart.h"
#include <stdio.h>


/*==========================================================
 * MAIN
 *==========================================================*/

int main(void)
{
    /*------------------------------------------------------
     * Initialize HAL
     *------------------------------------------------------*/
    HAL_Init();

    /*------------------------------------------------------
     * Configure System Clock
     *------------------------------------------------------*/
    SystemClock_Config();

    /*------------------------------------------------------
     * Initialize USART2
     *------------------------------------------------------*/
    UART2_Init();


    /*------------------------------------------------------
     * Main Application Loop
     *------------------------------------------------------*/
    while (1)
    {
        printf("Hello UART Modular Driver!\r\n");

        HAL_Delay(1000);
    }
}


/*==========================================================
 * System Clock Configuration
 *
 * HSI  = 16 MHz
 *
 * SYSCLK = 16 MHz
 * HCLK   = 16 MHz
 * APB1   = 16 MHz
 * APB2   = 16 MHz
 *==========================================================*/

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


    /*------------------------------------------------------
     * Configure HSI
     *------------------------------------------------------*/

    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_OFF;


    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1)
        {
        }
    }


    /*------------------------------------------------------
     * Configure CPU / Bus Clocks
     *
     * HSI -> SYSCLK
     *
     * SYSCLK = 16 MHz
     * HCLK   = 16 MHz
     * APB1   = 16 MHz
     * APB2   = 16 MHz
     *------------------------------------------------------*/

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK  |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_HSI;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV1;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;


    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                            FLASH_LATENCY_0) != HAL_OK)
    {
        while (1)
        {
        }
    }
}


/*==========================================================
 * SysTick Interrupt
 *==========================================================*/

void SysTick_Handler(void)
{
    HAL_IncTick();
}

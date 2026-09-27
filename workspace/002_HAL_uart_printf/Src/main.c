#include "stm32f4xx_hal.h"
#include <stdio.h>

/*==========================================================
 * Global UART handle
 *==========================================================*/
UART_HandleTypeDef huart2;


/*==========================================================
 * Function prototypes
 *==========================================================*/
void SystemClock_Config(void);
void UART2_Init(void);


/*==========================================================
 * printf() redirection
 *==========================================================*/
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2,
                      (uint8_t *)&ch,
                      1,
                      HAL_MAX_DELAY);

    return ch;
}


/*==========================================================
 * MAIN
 *==========================================================*/
int main(void)
{
    /* Initialize HAL */
    HAL_Init();

    /* Configure system clock */
    SystemClock_Config();

    /* Initialize USART2 */
    UART2_Init();

    while (1)
    {
        printf("Hello STM32 UART!\r\n");

        HAL_Delay(1000);
    }
}


/*==========================================================
 * UART2 initialization
 *
 * USART2:
 *
 * PA2 -> TX
 * PA3 -> RX
 *
 * Baud = 115200
 * 8 data bits
 * No parity
 * 1 stop bit
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
 * This is where the GPIO and USART clocks are enabled.
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
 * System Clock Configuration
 *
 * Use the internal 16 MHz HSI.
 *
 * SYSCLK = 16 MHz
 * APB1   = 16 MHz
 *
 *
 * #define SYS_FREQ  16000000
 * #define APB1_CLK  SYS_FREQ
 *==========================================================*/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* HSI = 16 MHz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1)
        {
        }
    }

    /* HSI = SYSCLK = 16 MHz */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while (1)
        {
        }
    }
}


/*==========================================================
 * SysTick interrupt
 *==========================================================*/
void SysTick_Handler(void)
{
    HAL_IncTick();
}

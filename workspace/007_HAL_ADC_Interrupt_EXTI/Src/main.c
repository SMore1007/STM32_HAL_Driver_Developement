/*
 * main.c
 *
 * STM32 HAL Driver Development - Project 007: HAL ADC Interrupt EXTI Prototype
 *
 * Overview:
 * This learning prototype demonstrates an asynchronous, event-driven hardware flow combining:
 * 1. External GPIO Interrupt (EXTI): Triggered by pressing the onboard User Push Button (PC13).
 * 2. Non-blocking ADC Conversion: Initiated in main loop upon EXTI flag detection via HAL_ADC_Start_IT().
 * 3. ADC Interrupt Complete Handling: Executed by HAL_ADC_ConvCpltCallback() upon conversion finish.
 * 4. Serial Telemetry Logging: Formatted output printed over USART2 (PA2 TX) at 115200 baud from thread context.
 *
 * Hardware Mapping (NUCLEO-F446RE):
 * - PC13 -> User Push Button (Active-Low, EXTI Line 13)
 * - PA5  -> Green User LED (Push-Pull Output)
 * - PA0  -> Analog Input Pin (ADC3 Channel 0 / Arduino header A0)
 * - PA2  -> USART2 TX (ST-LINK Virtual COM Port)
 */

#include <stdio.h>                             /* C standard library for printf output formatting */
#include "main.h"                              /* Main application header definitions */
#include "uart.h"                              /* USART2 driver interface */
#include "adc.h"                               /* ADC3 interrupt driver interface */

/* Software debounce threshold in milliseconds to ignore mechanical contact bounce */
#define DEBOUNCE_TIME_MS  200

/* Stores timestamp of last valid button press. volatile for ISR access */
static volatile uint32_t last_button_press = 0;

/* Flag set inside EXTI ISR when button press is validated. Processed in main loop */
static volatile uint8_t g_button_pressed_flag = 0;


/*
 * Main entry point of application.
 */
int main(void)
{
    /* Step 1: Initialize STM32 HAL library, Flash prefetch, and SysTick timer (1 ms tick) */
    HAL_Init();

    /* Step 2: Configure System Clock to 16 MHz using High-Speed Internal (HSI) RC oscillator */
    SystemClock_Config();

    /* Step 3: Initialize USART2 peripheral (115200 baud) for printf serial logging */
    UART2_Init();

    /* Step 4: Initialize GPIO PC13 (EXTI Button) and PA5 (User LED) */
    MX_GPIO_Init();

    /* Step 5: Initialize ADC3 Channel 0 on pin PA0 and configure ADC interrupt line in NVIC */
    Adc3Init();

    /* Step 6: Print startup welcome message to UART console */
    printf("\r\n=======================================================\r\n");
    printf("  STM32 HAL Driver Development: ADC Interrupt EXTI     \r\n");
    printf("=======================================================\r\n");
    printf("Initialization Complete. Waiting for PC13 Button Press...\r\n\r\n");

    /*
     * Step 7: Main application loop.
     * CPU remains in lightweight loop checking for EXTI button events and ADC conversions.
     */
    while (1)
    {
        /* Check if button press event was captured by EXTI ISR */
        if (g_button_pressed_flag)
        {
            /* Clear button press flag */
        	g_button_pressed_flag = 0;

            /* Safe UART serial print executed from main thread context (not ISR) */
            printf("[EXTI Interrupt] Button PC13 Pressed! LED Toggled. Initiating ADC conversion...\r\n");

            /* Start ADC3 conversion in non-blocking interrupt mode and verify status */
            HAL_StatusTypeDef status = HAL_ADC_Start_IT(&hadc3);
            if (status != HAL_OK)
            {
                printf("[ADC Error] HAL_ADC_Start_IT returned status %d. Recovery initiated...\r\n", (int)status);
                __HAL_ADC_CLEAR_FLAG(&hadc3, ADC_FLAG_EOC | ADC_FLAG_OVR);
                hadc3.State = HAL_ADC_STATE_READY;
                /* Retry starting conversion after state recovery */
                HAL_ADC_Start_IT(&hadc3);
            }
        }

        /* Check if an ADC conversion has completed (flag set in HAL_ADC_ConvCpltCallback) */
        if (adc_conversion_complete_flag)
        {
            /* Clear conversion complete flag */
            adc_conversion_complete_flag = 0;

            /*
             * Calculate equivalent analog voltage:
             * Voltage (V) = (ADC_Raw_Value * VREF) / Max_12Bit_Resolution
             * VREF = 3.3V, Max_Resolution = 4095
             */
            float voltage = ((float)adc_value * 3.3f) / 4095.0f;

            /* Print converted raw ADC value and calculated voltage to UART console */
            printf("[ADC Interrupt Complete] Raw Value: %lu | Voltage: %.2f V\r\n\r\n", adc_value, voltage);
        }

        /* Check if an ADC hardware error (OVR) occurred inside ISR */
        if (adc_error_flag)
        {
            adc_error_flag = 0;
            printf("[ADC Error Recovery] Hardware Overrun (OVR) flag was caught and cleared. ADC restored to READY state.\r\n\r\n");
        }
    }
}


/*
 * Configure System Clock to 16 MHz using internal HSI oscillator.
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Select and enable High Speed Internal (HSI) RC oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                      /* PLL disabled; 16 MHz HSI is sufficient */

    /* Apply oscillator configuration */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1);                             /* Trap CPU if oscillator setup fails */
    }

    /* Configure bus clock prescalers (SYSCLK, HCLK, PCLK1, PCLK2) */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;             /* SYSCLK = HSI (16 MHz) */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                  /* AHB Prescaler = 1 -> HCLK = 16 MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;                   /* APB1 Prescaler = 1 -> PCLK1 = 16 MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;                   /* APB2 Prescaler = 1 -> PCLK2 = 16 MHz */

    /* Apply bus clock configuration (Flash latency 0 for 16 MHz operation) */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while (1);                             /* Trap CPU if clock config fails */
    }
}


/*
 * Configure GPIO peripherals for EXTI input (PC13) and LED output (PA5).
 */
void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Step 1: Enable peripheral clock for SYSCFG (CRITICAL for GPIO EXTI mapping) */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Step 2: Enable peripheral clock for GPIOC (Push Button) */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Step 3: Enable peripheral clock for GPIOA (User LED) */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Step 4: Configure PC13 as External Interrupt pin (EXTI Line 13) */
    GPIO_InitStruct.Pin = BTN_PIN;                                    /* Select Pin 13 (PC13) */
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;                      /* Generate EXTI interrupt on Falling Edge (button press pulls to GND) */
    GPIO_InitStruct.Pull = GPIO_NOPULL;                               /* No internal pull (external pull-up resistor present on board) */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;                       /* Set low GPIO output speed */
    HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);                        /* Apply GPIO initialization */

    /* Step 5: Configure PA5 pin as Push-Pull Output for Green User LED */
    GPIO_InitStruct.Pin = LED_PIN;                                    /* Select Pin 5 (PA5) */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;                       /* Configure as Push-Pull digital output */
    GPIO_InitStruct.Pull = GPIO_NOPULL;                               /* No internal pull resistors */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;                       /* Low output speed */
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);                        /* Apply GPIO initialization */

    /* Step 6: Clear any pending EXTI line 13 interrupt flag before enabling NVIC */
    __HAL_GPIO_EXTI_CLEAR_IT(BTN_PIN);

    /* Step 7: Configure NVIC interrupt line for EXTI Lines 10 to 15 */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);                       /* Set EXTI interrupt priority (Preemption: 2, SubPriority: 0) */
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);                               /* Enable EXTI15_10_IRQn in NVIC */
}


/*
 * EXTI Line 10-15 Interrupt Service Routine (ISR)
 *
 * Triggered when PC13 button transition occurs on EXTI Line 13.
 */
void EXTI15_10_IRQHandler(void)
{
    /* Clear pending EXTI line flag and invoke HAL_GPIO_EXTI_Callback() */
    HAL_GPIO_EXTI_IRQHandler(BTN_PIN);

    /* Clear all pending flags across EXTI lines 10 to 15 to prevent unhandled interrupt loops */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
}


/*
 * HAL GPIO External Interrupt Callback
 *
 * Invoked by HAL_GPIO_EXTI_IRQHandler() when a valid EXTI trigger is recognized.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Verify that the interrupt originated from PC13 button pin */
    if (GPIO_Pin == BTN_PIN)
    {
        /* Fetch current system time in milliseconds */
        uint32_t current_time = HAL_GetTick();

        /* Software Debounce Logic */
        if ((current_time - last_button_press) >= DEBOUNCE_TIME_MS)
        {
            last_button_press = current_time;

            /* Toggle PA5 LED immediately inside ISR for instant hardware feedback */
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

            /* Signal main loop to log message and initiate ADC conversion */
            g_button_pressed_flag = 1;
        }
    }
}


/*
 * SysTick Interrupt Handler.
 *
 * Generates periodic 1 ms interrupt for HAL delay calculations and HAL_GetTick().
 */
void SysTick_Handler(void)
{
    HAL_IncTick();                             /* Increment HAL 1 ms tick counter */
}


/*
 * HardFault Exception Handler.
 *
 * Traps CPU and rapidly blinks PA5 LED if a hardware fault (unaligned access, bus error, stack fault) occurs.
 */
void HardFault_Handler(void)
{
    while (1)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        for (volatile uint32_t i = 0; i < 200000; i++);
    }
}


/*
 * Memory Management Fault Handler.
 */
void MemManage_Handler(void)
{
    while (1);
}


/*
 * Bus Fault Handler.
 */
void BusFault_Handler(void)
{
    while (1);
}


/*
 * Usage Fault Handler.
 */
void UsageFault_Handler(void)
{
    while (1);
}


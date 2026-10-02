/* Include files */
#include <stdio.h>
#include "main.h"
#include "uart.h"
#include "adc.h"

/* GPIO pin definitions */
#define BTN_PORT    GPIOC
#define BTN_PIN     GPIO_PIN_13

#define LED_PORT    GPIOA
#define LED_PIN     GPIO_PIN_5

/* Function prototype */
void init_gpio_pc13_interrupt(void);

/* Debounce handling */
#define DEBOUNCE_TIME_MS    20

/* Stores the timestamp of the last valid button press.
 * volatile because it is accessed from interrupt context.
 */
volatile uint32_t last_button_press = 0;

/* Main entry point of application */
int main(void)
{
    /* Initialize HAL library and SysTick timer */
    HAL_Init();

    /* Configure system clock to use HSI = 16 MHz */
    SystemClock_Config();

    /* Configure PC13 as EXTI input and PA5 as LED output */
    init_gpio_pc13_interrupt();

    /* Initialize UART2 for printf output */
    UART2_Init();

    /* Main application loop.
     * Button processing is handled asynchronously through EXTI interrupt.
     */
    while (1)
    {
        /* Main loop can perform other application tasks */
    }
}


/*
 * Configure System Clock to 16 MHz using HSI
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Select and enable HSI oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;

    /* Use default HSI calibration value */
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    /* PLL is not required because HSI 16 MHz is sufficient */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;

    /* Apply oscillator configuration */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Stay here if oscillator configuration fails */
        while (1);
    }

    /*
     * Configure system and peripheral bus clocks:
     * SYSCLK, HCLK, PCLK1 and PCLK2
     */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    /* Select HSI as the system clock source */
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

    /* AHB clock = SYSCLK / 1 = 16 MHz */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

    /* APB1 clock = HCLK / 1 = 16 MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    /* APB2 clock = HCLK / 1 = 16 MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    /*
     * Apply bus clock configuration.
     * FLASH_LATENCY_0 is sufficient for 16 MHz operation.
     */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        /* Stay here if clock configuration fails */
        while (1);
    }
}


/*
 * SysTick interrupt handler
 *
 * SysTick generates a periodic interrupt used by the HAL
 * to maintain the 1 ms system tick.
 */
void SysTick_Handler(void)
{
    /* Increment HAL tick counter */
    HAL_IncTick();
}


/*
 * Configure GPIO EXTI interrupt for PC13
 *
 * PC13 -> Push button input with EXTI interrupt
 * PA5  -> LED output
 */
void init_gpio_pc13_interrupt(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIOC peripheral clock before using PC13 */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*
     * Configure PC13 as external interrupt input.
     *
     * GPIO_MODE_IT_RISING:
     * Generate EXTI interrupt on rising edge.
     */
    GPIO_InitStruct.Pin   = BTN_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);

    /* Enable GPIOA peripheral clock before using PA5 */
    __HAL_RCC_GPIOA_CLK_ENABLE();


    /*
     * Configure PA0 as external interrupt input.
     *
     * GPIO_MODE_IT_RISING:
     * Generate EXTI interrupt on rising edge.
     */
    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure PA5 as push-pull LED output */
    GPIO_InitStruct.Pin   = LED_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);


    /*
     * PC13 belongs to EXTI line 13.
     * EXTI lines 10-15 share the same NVIC IRQ.
     */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0); // EXTI0_IRQn

    /* Enable EXTI15_10 interrupt in the NVIC */
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    /*
      * PA0 belongs to EXTI line 0.
      * EXTI lines 0 uses NVIC EXTI0_IRQn.
      */
     HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0); // EXTI0_IRQn

     /* Enable EXTI0 interrupt in the NVIC */
     HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


/*
 * HAL GPIO EXTI Callback
 *
 * HAL_GPIO_EXTI_IRQHandler() calls this callback after
 * identifying the triggered EXTI line.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Check whether the interrupt came from PC13 */
    if (GPIO_Pin == BTN_PIN)
    {
        /* Get current system time in milliseconds */
        uint32_t current_time = HAL_GetTick();

        /*
         * Software debounce:
         * Accept the button press only if at least
         * DEBOUNCE_TIME_MS has elapsed since the last
         * valid button event.
         */
        if ((current_time - last_button_press) >= DEBOUNCE_TIME_MS)
        {
            /* Save timestamp of the valid button press */
            last_button_press = current_time;

            /* Toggle LED state */
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

            /* Send debug message through UART */
            printf("Button Pressed\r\n");
        }
    }

    /* Check whether the interrupt came from PC13 */
    if (GPIO_Pin == GPIO_PIN_0)
    {
      printf("PA0 Pin Interrupt Happened\r\n");
    }
}


/*
 * EXTI15_10 IRQ Handler
 *
 * EXTI lines 10 to 15 share this interrupt handler.
 * Therefore, we explicitly pass GPIO_PIN_13 to the HAL
 * handler because our application uses EXTI line 13.
 */
void EXTI15_10_IRQHandler(void)
{
    /*
     * Handle EXTI interrupt for PC13.
     *
     * This function clears the EXTI pending flag and
     * eventually calls HAL_GPIO_EXTI_Callback().
     */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

/*
 * EXTI0 IRQ Handler
 *
 * EXTI lines 0 share this interrupt handler.
 */
void EXTI0_IRQHandler(void)
{
    /*
     * Handle EXTI interrupt for PA0.
     *
     * This function clears the EXTI pending flag and
     * eventually calls HAL_GPIO_EXTI_Callback().
     */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

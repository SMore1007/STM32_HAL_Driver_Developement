/* Include files */
#include <stdio.h>
#include "main.h"
#include "uart.h"
#include "adc.h"

#define BTN_PORT    GPIOC
#define BTN_PIN     GPIO_PIN_13
#define LED_PORT    GPIOA
#define LED_PIN     GPIO_PIN_5

/* function prototype */
void init_gpio_pc13_interrupt(void);

/* Denounce handling */
#define DEBOUNCE_TIME_MS    20
volatile uint32_t last_button_press = 0;


/* Main entry point of application */
int main(void)                                  /* Main function definition */
{                                               /* Start of main function body */
    HAL_Init();                                 /* Initialize STM32 HAL Library and SysTick timer */
    SystemClock_Config();                       /* Configure system clock to HSI 16 MHz */
    init_gpio_pc13_interrupt();
    UART2_Init();
    while (1)                                   /* Infinite application loop */
    {

    }
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


/*Init GPIO PC13 Interrupt */

void init_gpio_pc13_interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Enable GPIO clocks */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Make LED Pin as Output PA5 */
	GPIO_InitStruct.Pin  = BTN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);

	/* Enable GPIOA PA5  */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Make LED Pin as Output PA5 */
	GPIO_InitStruct.Pin  = LED_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

	/* Configure EXTI */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Callback function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BTN_PIN)
    {
        uint32_t current_time = HAL_GetTick();

        if ((current_time - last_button_press) >= DEBOUNCE_TIME_MS)
        {
            last_button_press = current_time;

            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

            printf("Button Pressed\r\n");
        }
    }
}
void EXTI15_10_IRQHandler(void)
{
	// As this handler is used for pin 10 to 15 need to pass particular interrupt PIN
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"


/*=============================================================================
 * GPIO Pin Configuration
 *============================================================================*/

/*
 * NUCLEO-F446RE onboard peripherals:
 *
 * User Button : PC13
 * User LED    : PA5
 */
#define BTN_PORT    GPIOC
#define BTN_PIN     GPIO_PIN_13

#define LED_PORT    GPIOA
#define LED_PIN     GPIO_PIN_5


/*=============================================================================
 * Function Prototypes
 *============================================================================*/

/**
 * @brief Initialize the user button connected to PC13.
 *
 * Configures PC13 as a digital input with no internal pull-up/pull-down.
 */
void PC13ButtonInit(void);

/**
 * @brief Initialize the onboard LED connected to PA5.
 *
 * Configures PA5 as a push-pull digital output.
 */
void PA5LedInit(void);


/*=============================================================================
 * Global Variables
 *============================================================================*/

/*
 * Stores the current state of the user button.
 *
 * HAL_GPIO_ReadPin() returns:
 *     GPIO_PIN_SET   -> logic HIGH
 *     GPIO_PIN_RESET -> logic LOW
 */
uint8_t ButtonStatus = 0;


/*=============================================================================
 * Main Application
 *============================================================================*/

int main(void)
{
    /*
     * Initialize the STM32 HAL.
     *
     * This initializes the HAL library and configures the
     * SysTick timer used by HAL for time-base functionality.
     */
    HAL_Init();

    /* Initialize onboard LED (PA5). */
    PA5LedInit();

    /* Initialize user button (PC13). */
    PC13ButtonInit();


    /*
     * Main application loop.
     *
     * The button state is continuously monitored.
     * LED behavior:
     *
     * Button released -> LED ON
     * Button pressed   -> LED OFF
     *
     * Note:
     * On the NUCLEO-F446RE, the user button is active-low.
     */
    while (1)
    {
        /* Read the current logic level of the user button. */
        ButtonStatus = HAL_GPIO_ReadPin(BTN_PORT, BTN_PIN);

        if (ButtonStatus)
        {
            /* Button released -> turn LED OFF. */
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, RESET);
        }
        else
        {
            /* Button pressed -> turn LED ON. */
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, SET);
        }
    }
}


/*=============================================================================
 * GPIO Initialization Functions
 *============================================================================*/

/**
 * @brief Initialize the PC13 user button.
 *
 * PC13 is configured as a digital input.
 *
 * Configuration:
 * - GPIOC clock enabled
 * - PC13 as input
 * - No pull-up/pull-down resistor
 */
void PC13ButtonInit(void)
{
    /* Enable clock for GPIOC peripheral. */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Configure PC13 as a digital input. */
    GPIO_InitStruct.Pin   = BTN_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    /* Apply configuration to GPIOC. */
    HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);
}


/**
 * @brief Initialize the PA5 onboard LED.
 *
 * PA5 is configured as a push-pull digital output.
 *
 * Configuration:
 * - GPIOA clock enabled
 * - PA5 as output
 * - Push-pull output
 * - No pull-up/pull-down resistor
 * - Low GPIO speed
 */
void PA5LedInit(void)
{
    /* Enable clock for GPIOA peripheral. */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Configure PA5 as a digital output. */
    GPIO_InitStruct.Pin   = LED_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    /* Apply configuration to GPIOA. */
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}


/*=============================================================================
 * SysTick Interrupt Handler
 *============================================================================*/

/**
 * @brief SysTick interrupt handler.
 *
 * Called periodically by the SysTick interrupt.
 * HAL_IncTick() increments the HAL tick counter used by
 * HAL timing functions such as HAL_Delay().
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

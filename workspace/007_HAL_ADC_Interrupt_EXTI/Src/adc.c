/*
 * adc.c
 *
 * Driver module for ADC3 peripheral operating in non-blocking interrupt mode.
 *
 * Hardware Mapping:
 * - PA0 -> ADC3 Channel 0 (mapped to Nucleo-F446RE Arduino header pin A0)
 *
 * Step-by-Step Execution Flow:
 * 1. Adc3Init() initializes GPIO pin PA0 in Analog mode and configures ADC3 registers.
 * 2. Continuous conversion mode is DISABLED so ADC conversions run single-shot on demand.
 * 3. NVIC ADC interrupt line (ADC_IRQn) is configured with priority 5 and enabled.
 * 4. When an external trigger (EXTI button press) occurs, HAL_ADC_Start_IT(&hadc3) is invoked.
 * 5. Hardware performs ADC conversion asynchronously. Upon completion, EOC interrupt fires.
 * 6. ADC_IRQHandler() executes -> calls HAL_ADC_IRQHandler() -> invokes HAL_ADC_ConvCpltCallback().
 * 7. Callback reads digital value (adc_value) and sets adc_conversion_complete_flag = 1.
 */

#include "adc.h"                                /* Include ADC driver header */
#include <stdint.h>                             /* Include standard C integer definitions */

/* Global ADC3 handle instance required by STM32 HAL drivers */
ADC_HandleTypeDef hadc3;

/* Stores the raw 12-bit ADC result (0 to 4095). volatile for ISR modification */
volatile uint32_t adc_value = 0;

/* Flag set when conversion completes inside ISR. volatile for ISR modification */
volatile uint8_t adc_conversion_complete_flag = 0;

/* Flag set when ADC error (OVR) occurs inside ISR. volatile for ISR modification */
volatile uint8_t adc_error_flag = 0;


/*
 * Initialize GPIO PA0 and ADC3 peripheral registers.
 */
void Adc3Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};     /* Zero-initialize GPIO configuration structure */
    ADC_ChannelConfTypeDef sConfig = {0};       /* Zero-initialize ADC channel configuration structure */

    /* Step 1: Enable GPIOA peripheral clock before configuring PA0 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Step 2: Configure PA0 pin as ADC Analog Input */
    GPIO_InitStruct.Pin = GPIO_PIN_0;           /* Select Pin 0 (PA0) */
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;    /* Set pin to Analog mode (disables digital input buffer) */
    GPIO_InitStruct.Pull = GPIO_NOPULL;         /* Disable internal pull-up and pull-down resistors */
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);     /* Apply GPIO settings to PA0 */

    /* Step 3: Enable ADC3 peripheral clock */
    __HAL_RCC_ADC3_CLK_ENABLE();

    /* Step 4: Configure ADC3 core parameters */
    hadc3.Instance = ADC3;                                            /* Point handle to ADC3 base register address */
    hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;             /* Set ADC clock = PCLK2 / 2 = 16 MHz / 2 = 8 MHz */
    hadc3.Init.Resolution = ADC_RESOLUTION_12B;                      /* Set 12-bit ADC resolution (output values 0 to 4095) */
    hadc3.Init.ContinuousConvMode = DISABLE;                          /* Disable continuous conversion; single conversion on trigger */
    hadc3.Init.DiscontinuousConvMode = DISABLE;                       /* Disable discontinuous mode */
    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;  /* Software trigger initiation (started inside EXTI callback) */
    hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;                 /* Software start selected as trigger source */
    hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;                       /* Right-align 12-bit data in 16-bit ADC_DR register */
    hadc3.Init.NbrOfConversion = 1;                                   /* Perform 1 conversion per trigger */
    hadc3.Init.DMAContinuousRequests = DISABLE;                        /* Disable continuous DMA requests */
    hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                    /* Raise End-Of-Conversion (EOC) flag after each single conversion */

    /* Step 5: Initialize ADC3 peripheral with handle settings */
    if (HAL_ADC_Init(&hadc3) != HAL_OK)
    {
        /* Trap CPU if ADC initialization fails */
        while (1);
    }

    /* Step 6: Configure ADC3 Channel 0 (PA0) parameters */
    sConfig.Channel = ADC_CHANNEL_0;                                  /* Select ADC Channel 0 (PA0) */
    sConfig.Rank = 1;                                                 /* First and only channel in regular sequence */
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;                  /* Set 480 ADC clock cycles sampling time for maximum accuracy */

    /* Step 7: Apply ADC channel configuration */
    if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
    {
        /* Trap CPU if channel configuration fails */
        while (1);
    }

    /* Step 8: Clear any pending hardware conversion complete or overrun flags before enabling interrupts */
    __HAL_ADC_CLEAR_FLAG(&hadc3, ADC_FLAG_EOC | ADC_FLAG_OVR);

    /* Step 9: Configure NVIC priority and enable ADC interrupt */
    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);                             /* Set ADC interrupt priority (Preemption: 5, SubPriority: 0) */
    HAL_NVIC_EnableIRQ(ADC_IRQn);                                     /* Enable ADC_IRQn in Nested Vectored Interrupt Controller */
}


/*
 * ADC Interrupt Service Routine (ISR)
 *
 * Automatically triggered by NVIC when ADC conversion completes (EOC flag set) or error occurs.
 */
void ADC_IRQHandler(void)
{
    /*
     * Pass control to standard HAL ADC IRQ handler.
     * HAL checks EOC/OVR flags, clears interrupt flags, and calls callbacks.
     */
    HAL_ADC_IRQHandler(&hadc3);
}


/*
 * ADC Conversion Complete Callback
 *
 * Called asynchronously by HAL_ADC_IRQHandler() when conversion finishes.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    /* Verify that the callback was triggered by ADC3 peripheral instance */
    if (hadc->Instance == ADC3)
    {
        /* Read 12-bit digital conversion result from ADC data register (ADC_DR) */
        adc_value = HAL_ADC_GetValue(hadc);

        /* Set flag to signal main application loop that new ADC data is ready */
        adc_conversion_complete_flag = 1;
    }
}


/*
 * ADC Error Callback
 *
 * Called asynchronously by HAL_ADC_IRQHandler() if a hardware error (such as Overrun OVR) occurs.
 */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC3)
    {
        /* Clear overrun and end-of-conversion flags to unblock peripheral */
        __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR | ADC_FLAG_EOC);

        /* Reset handle state to READY to permit subsequent conversion requests */
        hadc->State = HAL_ADC_STATE_READY;

        /* Signal error event to main thread context */
        adc_error_flag = 1;
    }
}


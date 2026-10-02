/*
 * adc.c
 *
 *  Created on: Sep 27, 2026
 *      Author: Lenovo
 */

#include "adc.h"                                /* Include ADC module header file */
#include <stdint.h>                             /* Include standard integer types header */

ADC_HandleTypeDef hadc3;                        /* Define ADC3 handle structure instance */

static void init_adc3_continous_conversion(void); /* Prototype for static helper function to initialize ADC3 */

uint32_t ReadAdc3PA0(void)                      /* Function to trigger and read ADC3 conversion on PA0 */
{                                               /* Start of ReadAdc3PA0 function */
    HAL_ADC_Start(&hadc3);                      /* Clears OVR & EOC flags and re-triggers SWSTART */
    HAL_ADC_PollForConversion(&hadc3, 100);     /* Poll ADC until conversion completes with 100 ms timeout */
    return HAL_ADC_GetValue(&hadc3);            /* Read and return converted digital value from ADC_DR */
}                                               /* End of ReadAdc3PA0 function */

void Adc3InitStart(void)                        /* Function to initialize ADC3 and start conversion */
{                                               /* Start of Adc3InitStart function */
    init_adc3_continous_conversion();           /* Configure GPIO PA0 pin and ADC3 module peripheral */
    HAL_ADC_Start(&hadc3);                      /* Start ADC continuous conversion process */
}                                               /* End of Adc3InitStart function */

static void init_adc3_continous_conversion(void) /* Static function to set up GPIO PA0 and ADC3 registers */
{                                               /* Start of configuration helper function */
    GPIO_InitTypeDef GPIO_InitStruct = {0};     /* Declare and zero-initialize GPIO config struct */
    ADC_ChannelConfTypeDef sConfig = {0};       /* Declare and zero-initialize ADC channel config struct */

    __HAL_RCC_GPIOA_CLK_ENABLE();               /* Enable peripheral clock for GPIO Port A */

    GPIO_InitStruct.Pin       = GPIO_PIN_0;      /* Select GPIO Pin 0 (PA0) */
    GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;/* Set GPIO Pin 0 mode to Analog mode */
    GPIO_InitStruct.Pull      = GPIO_NOPULL;     /* No internal pull-up or pull-down resistor */
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);     /* Apply GPIO initialization configuration to PA0 */

    __HAL_RCC_ADC3_CLK_ENABLE();                /* Enable peripheral clock for ADC3 module */

    hadc3.Instance                     = ADC3;                     /* Assign ADC3 register block pointer */
    hadc3.Init.ClockPrescaler          = ADC_CLOCK_SYNC_PCLK_DIV2; /* Set ADC clock prescaler to PCLK2 divided by 2 */
    hadc3.Init.Resolution              = ADC_RESOLUTION_12B;       /* Set ADC resolution to 12-bit (0-4095) */
    hadc3.Init.ContinuousConvMode      = ENABLE;                   /* Enable Continuous Conversion Mode */
    hadc3.Init.DiscontinuousConvMode   = DISABLE;                  /* Disable Discontinuous Mode */
    hadc3.Init.ExternalTrigConvEdge    = ADC_EXTERNALTRIGCONVEDGE_NONE; /* No external trigger edge (software start) */
    hadc3.Init.ExternalTrigConv        = ADC_SOFTWARE_START;       /* Set trigger source to software start */
    hadc3.Init.DataAlign               = ADC_DATAALIGN_RIGHT;      /* Align converted data right in ADC_DR */
    hadc3.Init.NbrOfConversion         = 1;                        /* Specify 1 regular conversion in sequence */
    hadc3.Init.DMAContinuousRequests   = DISABLE;                  /* Disable DMA continuous requests */
    hadc3.Init.EOCSelection            = ADC_EOC_SINGLE_CONV;      /* Set EOC flag at the end of single conversion */
    HAL_ADC_Init(&hadc3);                       /* Initialize ADC3 peripheral with handle settings */

    sConfig.Channel = ADC_CHANNEL_0;            /* Select ADC Channel 0 (mapped to PA0) */
    sConfig.Rank = 1;                           /* Assign Channel 0 as rank 1 in conversion sequence */
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; /* Set channel sampling time to 480 ADC clock cycles */

    HAL_ADC_ConfigChannel(&hadc3, &sConfig);    /* Configure regular channel parameters for ADC3 */
}                                               /* End of init_adc3_continous_conversion function */

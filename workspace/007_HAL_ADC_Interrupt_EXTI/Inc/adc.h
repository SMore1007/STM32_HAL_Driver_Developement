/*
 * adc.h
 *
 * Header file for ADC3 peripheral driver in non-blocking interrupt mode.
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"                              /* Include STM32 HAL library and main definitions */

/*
 * External declaration for global ADC3 handle structure.
 * Configured in adc.c and referenced across modules if needed.
 */
extern ADC_HandleTypeDef hadc3;

/*
 * Stores the latest ADC raw digital conversion result (12-bit: 0 to 4095).
 * Declared volatile because it is updated inside HAL_ADC_ConvCpltCallback() from ISR context.
 */
extern volatile uint32_t adc_value;

/*
 * Flag set to 1 inside HAL_ADC_ConvCpltCallback() when an ADC conversion finishes.
 * Main loop reads and resets this flag to process the newly converted sample.
 */
extern volatile uint8_t adc_conversion_complete_flag;

/*
 * Flag set to 1 inside HAL_ADC_ErrorCallback() if an ADC hardware error (e.g. Overrun OVR) occurs.
 * Main loop checks and resets this flag to handle and recover from hardware errors.
 */
extern volatile uint8_t adc_error_flag;

/*
 * Function prototype to initialize GPIO PA0 (ADC3 Channel 0) and configure ADC3 peripheral registers.
 */
void Adc3Init(void);

#endif /* INC_ADC_H_ */


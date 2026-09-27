/*
 * adc.h
 *
 *  Created on: Sep 27, 2026
 *      Author: Lenovo
 */

#ifndef ADC_H_                                  /* Include guard: prevent multiple inclusion of adc.h */
#define ADC_H_                                  /* Define ADC_H_ macro for include guard */

#include "stm32f4xx_hal.h"                     /* Include main STM32F4 HAL library header */
#include "stm32f4xx_hal_adc.h"                 /* Include ADC HAL driver functions and structures */
#include "stm32f4xx_hal_adc_ex.h"              /* Include ADC extended HAL driver functions */

extern ADC_HandleTypeDef hadc3;                 /* External declaration for ADC3 global handle */

uint32_t ReadAdc3PA0(void);                    /* Prototype for reading ADC conversion from channel PA0 */
void Adc3InitStart(void);                      /* Prototype for initializing ADC3 and starting conversion */

#endif /* ADC_H_ */                             /* End of include guard for ADC_H_ */

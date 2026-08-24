#ifndef GPIO_OUTPUT_H
#define GPIO_OUTPUT_H

#include "main.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    e_GPIO_OUTPUT_OK = 0,
    e_GPIO_OUTPUT_ERROR,
    e_GPIO_OUTPUT_INVALID_PARAM

} GpioOutput_Status_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState active_level;

    bool state;

} GpioOutput_Handle_t;

GpioOutput_Status_t GpioOutput_Init(GpioOutput_Handle_t *handle, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState active_level);

GpioOutput_Status_t GpioOutput_Set(GpioOutput_Handle_t *handle, bool state);

GpioOutput_Status_t GpioOutput_On(GpioOutput_Handle_t *handle);

GpioOutput_Status_t GpioOutput_Off(GpioOutput_Handle_t *handle);

GpioOutput_Status_t GpioOutput_Toggle(GpioOutput_Handle_t *handle);

bool GpioOutput_GetState(const GpioOutput_Handle_t *handle);

#endif

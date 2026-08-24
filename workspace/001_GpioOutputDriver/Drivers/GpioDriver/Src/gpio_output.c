#include "gpio_output.h"

static GPIO_PinState GpioOutput_GetInactiveLevel(const GpioOutput_Handle_t *handle)
{
    if (handle->active_level == GPIO_PIN_SET)
    {
        return GPIO_PIN_RESET;
    }

    return GPIO_PIN_SET;
}

GpioOutput_Status_t GpioOutput_Init(GpioOutput_Handle_t *handle, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState active_level)
{
    if ((handle == NULL) || (port == NULL))
    {
        return e_GPIO_OUTPUT_INVALID_PARAM;
    }

    handle->port = port;
    handle->pin = pin;
    handle->active_level = active_level;
    handle->state = false;

    HAL_GPIO_WritePin(handle->port, handle->pin, GpioOutput_GetInactiveLevel(handle));

    return e_GPIO_OUTPUT_OK;
}

GpioOutput_Status_t GpioOutput_Set(GpioOutput_Handle_t *handle, bool state)
{
    GPIO_PinState physical_level;

    if (handle == NULL)
    {
        return e_GPIO_OUTPUT_INVALID_PARAM;
    }

    if (state)
    {
        physical_level = handle->active_level;
    }
    else
    {
        physical_level = GpioOutput_GetInactiveLevel(handle);
    }

    HAL_GPIO_WritePin(handle->port, handle->pin, physical_level);

    handle->state = state;

    return e_GPIO_OUTPUT_OK;
}

GpioOutput_Status_t GpioOutput_On(GpioOutput_Handle_t *handle)
{
    return GpioOutput_Set(handle, true);
}

GpioOutput_Status_t GpioOutput_Off(GpioOutput_Handle_t *handle)
{
    return GpioOutput_Set(handle, false);
}

GpioOutput_Status_t GpioOutput_Toggle(GpioOutput_Handle_t *handle)
{
    if (handle == NULL)
    {
        return e_GPIO_OUTPUT_INVALID_PARAM;
    }

    return GpioOutput_Set(handle, !handle->state);
}

bool GpioOutput_GetState(const GpioOutput_Handle_t *handle)
{
    if (handle == NULL)
    {
        return false;
    }

    return handle->state;
}

#include "gpio_input.h"

// Pin status
GpioInputStatusEnum_t  GpioInputInit(GpioInputHandle_t *handle, const GpioInputConfig_t *config)
{
	if((handle == NULL) || (config == NULL))
	{
		return e_GpioInputIvalidParam;
	}

	if((config->port == NULL) || (config->pin == 0U))
	{
		return e_GpioInputIvalidParam;
	}

	if((config->activeState != GPIO_PIN_SET) && (config->activeState != GPIO_PIN_RESET))
	{
		return e_GpioInputIvalidParam;
	}

	handle->config = *config;

	return e_GpioInputOk;
}


// Pin state
GpioInputStateEnum_t GpioInputRead(const GpioInputHandle_t *handle)
{
	GPIO_PinState state;

	if(handle == NULL)
	{
		return e_GpioInputInactive;
	}

	state = HAL_GPIO_ReadPin(handle->config.port, handle->config.pin);

	if(state == handle->config.activeState)
	{
		return e_GpioInputActive;
	}

	return e_GpioInputInactive;
}


// is active
bool GpioInputIsActive(const GpioInputHandle_t *handle)
{
	return (GpioInputRead(handle) == e_GpioInputActive);
}

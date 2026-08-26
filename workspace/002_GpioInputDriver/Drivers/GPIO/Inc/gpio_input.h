#ifndef GPIO_INPUT_H
#define GPIO_INPUT_H

#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
	e_GpioInputOk = 0,
	e_GpioInputIvalidParam

}GpioInputStatusEnum_t;

typedef enum
{
	e_GpioInputInactive = 0,
	e_GpioInputActive
}GpioInputStateEnum_t;

typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIO_PinState activeState;
}GpioInputConfig_t;

typedef struct
{
	GpioInputConfig_t config;
}GpioInputHandle_t;

// API Declaration
GpioInputStatusEnum_t  GpioInputInit(GpioInputHandle_t *handle, const GpioInputConfig_t *config);
GpioInputStateEnum_t   GpioInputRead(const GpioInputHandle_t *handle);
bool GpioInputIsActive(const GpioInputHandle_t *handle);


#endif /*GPIO_INPUT_H*/

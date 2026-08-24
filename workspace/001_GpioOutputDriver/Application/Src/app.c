#include "app.h"
#include "gpio_output.h"

static GpioOutput_Handle_t ledHandle;

void App_Init(void)
{
    /*
     * IMPORTANT:
     *
     * Replace GPIOA / GPIO_PIN_5
     * with the actual LED GPIO of your board.
     *
     * Example only:
     *
     * GPIOA + GPIO_PIN_5
     */

    GpioOutput_Init(&ledHandle, GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

    GpioOutput_Off(&ledHandle);
}

void App_Run(void)
{
    GpioOutput_Toggle(&ledHandle);

    HAL_Delay(500);
}

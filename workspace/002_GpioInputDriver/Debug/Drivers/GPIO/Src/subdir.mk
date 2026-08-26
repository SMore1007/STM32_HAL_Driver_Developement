################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/GPIO/Src/gpio_input.c 

OBJS += \
./Drivers/GPIO/Src/gpio_input.o 

C_DEPS += \
./Drivers/GPIO/Src/gpio_input.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/GPIO/Src/%.o Drivers/GPIO/Src/%.su Drivers/GPIO/Src/%.cyclo: ../Drivers/GPIO/Src/%.c Drivers/GPIO/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"D:/Udemy_Course/Drivers_developements/workspace/002_GpioInputDriver/Drivers/GPIO/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-GPIO-2f-Src

clean-Drivers-2f-GPIO-2f-Src:
	-$(RM) ./Drivers/GPIO/Src/gpio_input.cyclo ./Drivers/GPIO/Src/gpio_input.d ./Drivers/GPIO/Src/gpio_input.o ./Drivers/GPIO/Src/gpio_input.su

.PHONY: clean-Drivers-2f-GPIO-2f-Src


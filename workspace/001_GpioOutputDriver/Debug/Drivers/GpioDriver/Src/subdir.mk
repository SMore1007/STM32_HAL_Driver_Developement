################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/GpioDriver/Src/gpio_output.c 

OBJS += \
./Drivers/GpioDriver/Src/gpio_output.o 

C_DEPS += \
./Drivers/GpioDriver/Src/gpio_output.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/GpioDriver/Src/%.o Drivers/GpioDriver/Src/%.su Drivers/GpioDriver/Src/%.cyclo: ../Drivers/GpioDriver/Src/%.c Drivers/GpioDriver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"D:/Udemy_Course/Drivers_developements/workspace/001_GpioOutputDriver/Application/Inc" -I"D:/Udemy_Course/Drivers_developements/workspace/001_GpioOutputDriver/Drivers/GpioDriver/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-GpioDriver-2f-Src

clean-Drivers-2f-GpioDriver-2f-Src:
	-$(RM) ./Drivers/GpioDriver/Src/gpio_output.cyclo ./Drivers/GpioDriver/Src/gpio_output.d ./Drivers/GpioDriver/Src/gpio_output.o ./Drivers/GpioDriver/Src/gpio_output.su

.PHONY: clean-Drivers-2f-GpioDriver-2f-Src


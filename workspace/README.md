# STM32 HAL Driver Development

Welcome to the **STM32 HAL Driver Development** repository! This repository contains a step-by-step series of projects demonstrating peripheral driver development using the **STM32 Hardware Abstraction Layer (HAL)** on ARM Cortex-M4 microcontrollers.

---

## 🛠️ Hardware & Tools

* **Microcontroller**: STM32F446RET6 (ARM Cortex-M4 with FPU, 180 MHz max clock)
* **Development Board**: NUCLEO-F446RE
* **IDE / Toolchain**: STM32CubeIDE / GCC ARM Embedded Toolchain
* **HAL Drivers**: STM32F4xx HAL Driver Library

---

## 📁 Repository Projects Overview

| # | Project Name | Peripherals Used | Key Concepts & Drivers Covered |
|---|---|---|---|
| **001** | [`001_HAL_GpioInputOutput`](file:///d:/Udemy_Course/STM32_HAL_Driver_Development/STM32_HAL_Driver_Developement/workspace/001_HAL_GpioInputOutput) | GPIO (PA5, PC13) | Digital Output (LED), Digital Input (Push Button), Active-Low Input reading, `HAL_GPIO_ReadPin()`, `HAL_GPIO_WritePin()`. |
| **002** | [`002_HAL_uart_printf`](file:///d:/Udemy_Course/STM32_HAL_Driver_Development/STM32_HAL_Driver_Developement/workspace/002_HAL_uart_printf) | USART2 (PA2 TX) | UART TX configuration, `printf()` redirection via `__io_putchar()`, 115200 Baud, HSI 16 MHz Clock configuration. |
| **003** | [`003_HAL_uart_modular`](file:///d:/Udemy_Course/STM32_HAL_Driver_Development/STM32_HAL_Driver_Developement/workspace/003_HAL_uart_modular) | USART2 (PA2 TX) | Modular driver organization (`uart.h`, `uart.c`), clean API design, decoupled MSP initialization. |
| **004** | [`004_HAL_ADC_Continious_Conversion`](file:///d:/Udemy_Course/STM32_HAL_Driver_Development/STM32_HAL_Driver_Developement/workspace/004_HAL_ADC_Continious_Conversion) | ADC3 (PA0), USART2 | Analog-to-Digital Conversion (12-bit), Continuous Conversion Mode, Polling for conversion, Hardware Overrun (`OVR`) handling. |

---

## 📑 Detailed Project Documentation

### 001. GPIO Digital I/O (`001_HAL_GpioInputOutput`)
* **Objective**: Interface digital inputs and outputs using STM32 HAL GPIO APIs.
* **Hardware Wiring**: Onboard Green LED (`PA5`), Onboard User Button (`PC13`).
* **Implementation Details**:
  * Configures `PA5` as Push-Pull Output (`GPIO_MODE_OUTPUT_PP`).
  * Configures `PC13` as Digital Input (`GPIO_MODE_INPUT`).
  * Demonstrates polling the active-low button status using `HAL_GPIO_ReadPin()` and controlling the LED via `HAL_GPIO_WritePin()`.

---

### 002. UART Logging & `printf()` Redirection (`002_HAL_uart_printf`)
* **Objective**: Configure USART2 for serial telemetry and override standard C library `printf()`.
* **Hardware Wiring**: `PA2` (USART2_TX mapped to ST-LINK VCP UART).
* **Implementation Details**:
  * Configures USART2 (115200 Baud, 8 Data Bits, No Parity, 1 Stop Bit).
  * Implements `__io_putchar(int ch)` to route `printf()` output through `HAL_UART_Transmit()`.
  * Sets up System Clock (`SystemClock_Config`) using HSI (16 MHz).

---

### 003. Modular UART Driver (`003_HAL_uart_modular`)
* **Objective**: Restructure UART peripheral setup into clean, reusable driver modules.
* **Architecture**:
  * `uart.h` / `uart.c`: Handles USART2 handle initialization, MSP clock/pin assignment (`HAL_UART_MspInit`), and `printf()` retargeting.
  * `main.c`: Clean application layer executing system clock initialization and periodic data logging.

---

### 004. ADC Continuous Conversion Driver (`004_HAL_ADC_Continious_Conversion`)
* **Objective**: Measure analog input voltages continuously using ADC3 Channel 0 on pin `PA0`.
* **Hardware Wiring**: `PA0` connected to analog input source / potentiometer.
* **Implementation Details**:
  * Configures `PA0` in Analog Mode (`GPIO_MODE_ANALOG`).
  * Initializes ADC3 in 12-bit resolution (`ADC_RESOLUTION_12B`) with continuous conversion mode enabled (`ContinuousConvMode = ENABLE`).
  * **Critical Bug Fix / Hardware Insight**:
    When sampling in Continuous Mode while executing delays (`HAL_Delay()`), unread samples cause the hardware Overrun (`OVR`) flag to trigger, which halts the ADC conversion engine. To prevent lockup and ensure continuous real-time readings, `HAL_ADC_Start()` is invoked prior to polling (`HAL_ADC_PollForConversion()`), which clears `OVR`/`EOC` flags and re-triggers `SWSTART`.

---

## 🚀 Road Map & Upcoming Driver Modules

The development plan for upcoming HAL drivers in this workspace includes:

- [ ] **005_HAL_ADC_Interrupt**: Non-blocking ADC sampling using EOC interrupts (`HAL_ADC_Start_IT`).
- [ ] **004_HAL_ADC_DMA**: High-speed ADC data transfers directly to RAM via DMA.
- [ ] **006_HAL_Timer_Base**: Basic Timer delays and periodic interrupts (`TIM2` / `TIM6`).
- [ ] **007_HAL_Timer_PWM**: Pulse Width Modulation output for LED dimming / motor control.
- [ ] **008_HAL_SPI_Driver**: Serial Peripheral Interface (SPI) master transmitter/receiver.
- [ ] **009_HAL_I2C_Driver**: Inter-Integrated Circuit (I2C) communication with external sensors/EEPROM.
- [ ] **010_HAL_EXTI_Interrupt**: External GPIO line interrupts (`HAL_GPIO_EXTI_Callback`).

---

## 📝 Coding & Documentation Guidelines

All code in this repository adheres to line-by-line documentation standards:
* Every hardware register initialization step, callback function, and logic check contains explicit C comments (`/* comment */`).
* Full compatibility with **STM32CubeIDE** and standard GCC ARM toolchains.

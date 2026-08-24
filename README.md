# STM32F446RE Embedded Driver Development Lab

A hands-on **Embedded Systems Driver Development Lab** focused on building, testing, and progressively improving reusable firmware drivers using **STM32F446RE**.

The goal of this repository is not to collect isolated code examples. It is a structured learning journey from basic GPIO abstraction to **interrupts, timers, ADC, DMA, UART protocols, sensor drivers, FreeRTOS, and dual-MCU STM32 ↔ ESP32 architecture**.

---

## 🎯 Project Goal

Build **90 practical embedded driver projects** from the ground up.

The learning progression is:

```text
GPIO
  ↓
Timers
  ↓
PWM
  ↓
ADC
  ↓
UART
  ↓
UART Protocols
  ↓
Ultrasonic Sensor
  ↓
Events & Buffers
  ↓
Fault Handling & Diagnostics
  ↓
FreeRTOS
  ↓
STM32 ↔ ESP32
  ↓
Dual-MCU Architecture
```

Every project is designed to answer three questions:

1. **How does the hardware peripheral work?**
2. **How should a reusable embedded driver be designed?**
3. **How should the driver be integrated into a real application?**

---

# 🧠 Learning Philosophy

This repository follows a **build → test → debug → refactor → improve** approach.

Instead of starting with a complex production-style driver, projects progressively evolve from simple implementations into more robust architectures.

For example:

```text
HAL API
  ↓
Basic Driver
  ↓
Driver Abstraction
  ↓
Interrupt Driver
  ↓
DMA Driver
  ↓
Non-Blocking Driver
  ↓
RTOS Driver
  ↓
Protocol Integration
```

The objective is to understand not only **how to use STM32 HAL**, but how to build a software layer on top of HAL that is:

* Reusable
* Testable
* Maintainable
* Hardware-aware
* Modular
* Fault-tolerant
* Suitable for professional embedded firmware

---

# 🛠️ Hardware

Primary MCU:

* **STM32F446RE**

Currently available hardware:

* STM32 development board
* ESP32 development board
* Potentiometer
* Ultrasonic sensor
* Breadboard
* Jumper wires
* USB cables

ESP32 is introduced only when the project specifically focuses on **STM32 ↔ ESP32 communication or dual-MCU architecture**.

---

# 💻 Development Environment

## STM32

* STM32CubeIDE
* STM32CubeMX
* STM32 HAL
* Embedded C
* CMSIS where appropriate
* FreeRTOS for RTOS projects

## ESP32

* VS Code
* PlatformIO
* Arduino Framework
* C++

ESP32 projects do **not** use ESP-IDF.

---

# 🏗️ Driver Architecture

The general STM32 architecture used throughout the repository is:

```text
┌───────────────────────────┐
│       Application         │
├───────────────────────────┤
│       Driver API          │
├───────────────────────────┤
│       STM32 HAL           │
├───────────────────────────┤
│     MCU Peripheral        │
├───────────────────────────┤
│        Hardware           │
└───────────────────────────┘
```

The application should interact with the driver through a clean API instead of directly depending on low-level peripheral implementation.

Example:

```c
LedDriver_Init();
LedDriver_On();
LedDriver_Off();
```

rather than placing GPIO operations throughout the application:

```c
HAL_GPIO_WritePin(...);
```

---

# 📁 Repository Structure

The repository is organized according to the learning progression.

```text
stm32-embedded-drivers/
│
├── README.md
│
├── 01_GPIO/
│   ├── 01_GPIO_Output_Driver/
│   ├── 02_GPIO_Input_Driver/
│   ├── 03_LED_Driver/
│   ├── 04_Button_Driver/
│   ├── 05_Button_Debounce_Driver/
│   └── 06_GPIO_Interrupt_Driver/
│
├── 02_Timer_PWM/
│   ├── 07_Timer_Driver/
│   ├── 08_Delay_Driver/
│   ├── 09_Periodic_Timer_Driver/
│   ├── 10_PWM_Driver/
│   ├── 11_LED_Brightness_Driver/
│   ├── 12_Buzzer_Driver/
│   └── 13_Servo_PWM_Driver/
│
├── 03_ADC/
│   ├── 14_Potentiometer_ADC_Driver/
│   ├── 15_ADC_Multi_Channel_Driver/
│   ├── 16_ADC_Calibration_Driver/
│   ├── 17_ADC_Filtering_Driver/
│   ├── 18_ADC_Oversampling_Driver/
│   ├── 19_ADC_DMA_Driver/
│   └── 20_ADC_Threshold_Driver/
│
├── 04_UART/
│   ├── 21_UART_TX_Driver/
│   ├── 22_UART_RX_Driver/
│   ├── 23_UART_Echo_Driver/
│   ├── 24_UART_Interrupt_Driver/
│   ├── 25_UART_Ring_Buffer_Driver/
│   ├── 26_UART_Non_Blocking_Driver/
│   ├── 27_UART_DMA_Driver/
│   ├── 28_UART_Packet_Driver/
│   ├── 29_UART_CRC_Driver/
│   └── 30_UART_Command_Driver/
│
├── 05_STM32_ESP32_Communication/
│   ├── 31_STM32_ESP32_UART_Driver/
│   ├── 32_STM32_ESP32_ACK_Driver/
│   ├── 33_STM32_ESP32_Request_Response_Driver/
│   ├── 34_STM32_ESP32_Heartbeat_Driver/
│   ├── 35_STM32_ESP32_Timeout_Driver/
│   └── 36_STM32_ESP32_Retry_Driver/
│
├── 06_Ultrasonic/
│   ├── 37_Ultrasonic_GPIO_Trigger_Driver/
│   ├── 38_Ultrasonic_Echo_Driver/
│   ├── 39_Ultrasonic_Timer_Capture_Driver/
│   ├── 40_Ultrasonic_Distance_Driver/
│   ├── 41_Ultrasonic_Timeout_Driver/
│   ├── 42_Ultrasonic_Filtering_Driver/
│   ├── 43_Ultrasonic_Moving_Average_Driver/
│   └── 44_Ultrasonic_Threshold_Driver/
│
├── 07_Sensor_Integration/
│   ├── 45_Ultrasonic_Potentiometer_Driver/
│   └── 46_Sensor_Manager_Driver/
│
├── 08_Event_Architecture/
│   ├── 47_GPIO_Event_Driver/
│   ├── 48_Timer_Event_Driver/
│   ├── 49_Sensor_Event_Driver/
│   └── 50_Event_Queue_Driver/
│
├── 09_Buffers_Queues/
│   ├── 51_Software_FIFO_Driver/
│   ├── 52_Circular_Buffer_Driver/
│   └── 53_Generic_Queue_Driver/
│
├── 10_Fault_Diagnostics/
│   ├── 54_Watchdog_Driver/
│   ├── 55_Independent_Watchdog_Driver/
│   ├── 56_RTC_System_Time_Driver/
│   ├── 57_Non_Volatile_Configuration_Driver/
│   ├── 58_STM32_Flash_Configuration_Driver/
│   ├── 59_ESP32_NVS_Configuration_Driver/
│   ├── 60_System_Status_Driver/
│   ├── 61_Error_Code_Driver/
│   ├── 62_Diagnostic_Driver/
│   ├── 63_Debug_Logger_Driver/
│   ├── 64_UART_CLI_Driver/
│   ├── 65_CLI_Command_Parser/
│   ├── 66_Device_Information_Driver/
│   ├── 67_Firmware_Version_Driver/
│   └── 68_Boot_Status_Driver/
│
├── 11_State_Machines/
│   ├── 69_Communication_State_Machine/
│   └── 70_Sensor_State_Machine/
│
├── 12_FreeRTOS/
│   ├── 71_STM32_FreeRTOS_UART_Driver/
│   ├── 72_UART_RX_Task_Driver/
│   ├── 73_UART_Queue_Driver/
│   ├── 74_UART_Task_Notification_Driver/
│   ├── 75_ADC_FreeRTOS_Driver/
│   ├── 76_Ultrasonic_FreeRTOS_Driver/
│   ├── 77_Sensor_Manager_FreeRTOS/
│   ├── 78_UART_Sensor_Queue/
│   ├── 79_Driver_Mutex_Manager/
│   └── 80_Driver_Timeout_Manager/
│
├── 13_ESP32/
│   ├── 81_ESP32_UART_Driver/
│   ├── 82_ESP32_UART_RX_Task/
│   ├── 83_ESP32_Ring_Buffer_Driver/
│   └── 84_ESP32_Command_Handler/
│
└── 14_Dual_MCU/
    ├── 85_ESP32_STM32_Sensor_Gateway/
    ├── 86_ESP32_STM32_Remote_ADC/
    ├── 87_ESP32_STM32_Remote_Ultrasonic/
    ├── 88_ESP32_STM32_Control_Protocol/
    ├── 89_ESP32_STM32_Health_Monitor/
    └── 90_Dual_MCU_System_Driver/
```

The exact directory structure may evolve as the projects become more complex.

---

# 📚 Driver Learning Roadmap

## Phase 1 — GPIO Drivers

### 01 — GPIO Output Driver

**Concept:** GPIO abstraction

Learn:

* GPIO configuration
* Output control
* HAL GPIO APIs
* Driver abstraction

---

### 02 — GPIO Input Driver

**Concept:** Digital input

Learn:

* Input configuration
* Pull-up/pull-down
* Digital state reading
* Input abstraction

---

### 03 — LED Driver

**Concept:** GPIO driver API

Learn:

* Device-level abstraction
* LED API
* Hardware-independent application logic

---

### 04 — Button Driver

**Concept:** Input handling

Learn:

* Button state
* GPIO input
* Driver interface

---

### 05 — Button Debounce Driver

**Concept:** Software filtering

Learn:

* Mechanical bouncing
* Debouncing
* Timing-based filtering
* State management

---

### 06 — GPIO Interrupt Driver

**Concept:** EXTI / ISR

Learn:

* External interrupts
* NVIC
* EXTI
* HAL callbacks
* ISR design

---

# Phase 2 — Timer and PWM Drivers

### 07 — Timer Driver

**Concept:** Hardware timer

### 08 — Delay Driver

**Concept:** Blocking / non-blocking timing

### 09 — Periodic Timer Driver

**Concept:** Periodic callbacks

### 10 — PWM Driver

**Concept:** Timer PWM

### 11 — LED Brightness Driver

**Concept:** PWM control

### 12 — Buzzer Driver

**Concept:** PWM and timing

### 13 — Servo PWM Driver

**Concept:** PWM-based position control

---

# Phase 3 — ADC Drivers

### 14 — Potentiometer ADC Driver

**Concept:** ADC

Learn:

* Analog input
* ADC conversion
* Raw ADC values
* Voltage conversion

### 15 — ADC Multi-Channel Driver

**Concept:** Multiple ADC inputs

### 16 — ADC Calibration Driver

**Concept:** Offset and scale calibration

### 17 — ADC Filtering Driver

**Concept:** Moving average

### 18 — ADC Oversampling Driver

**Concept:** Noise reduction

### 19 — ADC DMA Driver

**Concept:** Continuous ADC + DMA

### 20 — ADC Threshold Driver

**Concept:** Analog → event conversion

---

# Phase 4 — UART Drivers

### 21 — UART TX Driver

**Concept:** UART polling

### 22 — UART RX Driver

**Concept:** UART polling

### 23 — UART Echo Driver

**Concept:** TX/RX

### 24 — UART Interrupt Driver

**Concept:** UART ISR

### 25 — UART Ring Buffer Driver

**Concept:** Circular buffering

### 26 — UART Non-Blocking Driver

**Concept:** State machine

### 27 — UART DMA Driver

**Concept:** DMA-based communication

### 28 — UART Packet Driver

**Concept:** Packet framing

### 29 — UART CRC Driver

**Concept:** Error detection

### 30 — UART Command Driver

**Concept:** Command parsing

---

# Phase 5 — STM32 ↔ ESP32 Communication

These projects introduce the ESP32 because the objective is now **multi-MCU communication**.

### 31 — STM32 ↔ ESP32 UART Driver

**Concept:** MCU-to-MCU communication

### 32 — STM32 ↔ ESP32 ACK Driver

**Concept:** ACK/NACK

### 33 — STM32 ↔ ESP32 Request/Response Driver

**Concept:** Protocol design

### 34 — STM32 ↔ ESP32 Heartbeat Driver

**Concept:** Connection monitoring

### 35 — STM32 ↔ ESP32 Timeout Driver

**Concept:** Communication fault handling

### 36 — STM32 ↔ ESP32 Retry Driver

**Concept:** Reliable communication

---

# Phase 6 — Ultrasonic Drivers

### 37 — Ultrasonic GPIO Trigger Driver

**Concept:** Trigger generation

### 38 — Ultrasonic Echo Driver

**Concept:** Echo measurement

### 39 — Ultrasonic Timer Capture Driver

**Concept:** Input capture

### 40 — Ultrasonic Distance Driver

**Concept:** Time → distance conversion

### 41 — Ultrasonic Timeout Driver

**Concept:** Missing echo handling

### 42 — Ultrasonic Filtering Driver

**Concept:** Distance smoothing

### 43 — Ultrasonic Moving Average Driver

**Concept:** Noise reduction

### 44 — Ultrasonic Threshold Driver

**Concept:** Near/far detection

---

# Phase 7 — Sensor Integration

### 45 — Ultrasonic + Potentiometer Driver

**Concept:** Sensor combination

### 46 — Sensor Manager Driver

**Concept:** Multiple sensor abstraction

Learn how multiple independent drivers can be coordinated through a common manager.

---

# Phase 8 — Event Architecture

### 47 — GPIO Event Driver

**Concept:** GPIO → event

### 48 — Timer Event Driver

**Concept:** Timer → event

### 49 — Sensor Event Driver

**Concept:** Sensor → event

### 50 — Event Queue Driver

**Concept:** Event-based architecture

---

# Phase 9 — Buffers and Queues

### 51 — Software FIFO Driver

**Concept:** Generic FIFO

### 52 — Circular Buffer Driver

**Concept:** Buffer management

### 53 — Generic Queue Driver

**Concept:** Producer/consumer architecture

---

# Phase 10 — Fault Handling, Configuration and Diagnostics

### 54 — Watchdog Driver

**Concept:** Fault recovery

### 55 — Independent Watchdog Driver

**Concept:** MCU recovery

### 56 — RTC/System Time Driver

**Concept:** Time management

### 57 — Non-Volatile Configuration Driver

**Concept:** Persistent settings

### 58 — STM32 Flash Configuration Driver

**Concept:** Internal flash

### 59 — ESP32 NVS Configuration Driver

**Concept:** Persistent ESP32 data

This is the first project in this phase specifically targeting ESP32 persistent storage.

### 60 — System Status Driver

**Concept:** Health monitoring

### 61 — Error Code Driver

**Concept:** Error management

### 62 — Diagnostic Driver

**Concept:** Runtime diagnostics

### 63 — Debug Logger Driver

**Concept:** Logging abstraction

### 64 — UART CLI Driver

**Concept:** Embedded command line

### 65 — CLI Command Parser

**Concept:** Command processing

### 66 — Device Information Driver

**Concept:** Firmware/device information

### 67 — Firmware Version Driver

**Concept:** Version management

### 68 — Boot Status Driver

**Concept:** Startup state

---

# Phase 11 — State Machines

### 69 — Communication State Machine

**Concept:** Connection states

Example:

```text
INIT
 ↓
CONNECTING
 ↓
CONNECTED
 ↓
ERROR
 ↓
RECOVERY
```

### 70 — Sensor State Machine

**Concept:** Sensor control

Learn how embedded systems can represent hardware behavior using explicit states instead of deeply nested conditional logic.

---

# Phase 12 — FreeRTOS Drivers

FreeRTOS is introduced after the fundamental bare-metal driver concepts are understood.

### 71 — STM32 FreeRTOS UART Driver

**Concept:** UART + RTOS

### 72 — UART RX Task Driver

**Concept:** ISR → task

### 73 — UART Queue Driver

**Concept:** Queue communication

### 74 — UART Task Notification Driver

**Concept:** ISR synchronization

### 75 — ADC + FreeRTOS Driver

**Concept:** ADC task

### 76 — Ultrasonic + FreeRTOS Driver

**Concept:** Sensor task

### 77 — Sensor Manager + FreeRTOS

**Concept:** Multiple tasks

### 78 — UART + Sensor Queue

**Concept:** Sensor → communication

### 79 — Driver Mutex Manager

**Concept:** Shared resources

### 80 — Driver Timeout Manager

**Concept:** RTOS timeout handling

---

# Phase 13 — ESP32 Drivers

These projects focus specifically on the ESP32 side of the dual-MCU system.

### 81 — ESP32 UART Driver

**Concept:** PlatformIO UART abstraction

### 82 — ESP32 UART RX Task

**Concept:** ESP32 asynchronous RX

### 83 — ESP32 Ring Buffer Driver

**Concept:** ESP32 buffering

### 84 — ESP32 Command Handler

**Concept:** Command processing

---

# Phase 14 — Dual-MCU Architecture

The final phase combines the concepts learned throughout the repository.

### 85 — ESP32 ↔ STM32 Sensor Gateway

**Concept:** Gateway architecture

### 86 — ESP32 ↔ STM32 Remote ADC

**Concept:** Remote sensor reading

### 87 — ESP32 ↔ STM32 Remote Ultrasonic

**Concept:** Remote sensor reading

### 88 — ESP32 ↔ STM32 Control Protocol

**Concept:** MCU command protocol

### 89 — ESP32 ↔ STM32 Health Monitor

**Concept:** Heartbeat + diagnostics

### 90 — Dual-MCU System Driver

**Concept:** Complete STM32 + ESP32 architecture

This final project combines:

```text
Drivers
+
Interrupts
+
DMA
+
Buffers
+
Protocols
+
Error Handling
+
State Machines
+
FreeRTOS
+
STM32
+
ESP32
```

---

# 🔄 Development Methodology

Every project should follow this development cycle:

```text
1. Understand
      ↓
2. Configure
      ↓
3. Wire
      ↓
4. Implement HAL Test
      ↓
5. Create Driver
      ↓
6. Integrate Application
      ↓
7. Test
      ↓
8. Debug
      ↓
9. Refactor
      ↓
10. Improve
```

Each project should document:

* Objective
* Hardware
* Wiring
* CubeIDE configuration
* Driver architecture
* API design
* Implementation
* Testing
* Debugging
* Error handling
* Key learning
* Next-level improvements

---

# 🧩 Standard Driver Pattern

Where appropriate, drivers should follow a structure similar to:

```c
typedef enum
{
    DRIVER_OK = 0,
    DRIVER_ERROR,
    DRIVER_INVALID_PARAM,
    DRIVER_TIMEOUT,
    DRIVER_BUSY
} DriverStatus_t;
```

Configuration:

```c
typedef struct
{
    /* Configuration parameters */
} DriverConfig_t;
```

Initialization:

```c
DriverStatus_t Driver_Init(const DriverConfig_t *config);
```

Operation:

```c
DriverStatus_t Driver_Start(void);

DriverStatus_t Driver_Stop(void);

DriverStatus_t Driver_Read(...);

DriverStatus_t Driver_Write(...);
```

The exact API should be adapted to each peripheral.

The goal is **clean abstraction**, not forcing every driver into an identical API.

---

# 🧪 Testing Philosophy

Every driver should be tested independently before integrating it into a larger system.

Example:

```text
Peripheral
    ↓
Driver
    ↓
Driver Test
    ↓
Application
```

Tests should include, where applicable:

* Normal operation
* Boundary values
* Invalid parameters
* Timeout
* Hardware disconnection
* Buffer overflow
* Communication failure
* Recovery
* Repeated operation
* Stress testing

---

# 🐛 Debugging Philosophy

The project should use practical debugging techniques rather than relying only on `printf`.

Primary tools:

* STM32CubeIDE debugger
* Breakpoints
* Watch expressions
* Variable inspection
* Register inspection
* UART terminal
* ESP32 Serial Monitor
* GPIO-based timing/debug signals

Optional:

* Logic analyzer
* Oscilloscope

---

# 🚀 Progressive Driver Maturity

The repository intentionally moves through multiple levels of driver maturity.

### Level 1 — HAL Usage

```text
Application → HAL → Peripheral
```

### Level 2 — Basic Driver

```text
Application → Driver → HAL → Peripheral
```

### Level 3 — Interrupt Driver

```text
Peripheral → ISR → Driver → Application
```

### Level 4 — DMA Driver

```text
Peripheral ↔ DMA
       ↓
    Driver
       ↓
 Application
```

### Level 5 — Non-Blocking Driver

```text
Application
     ↓
Driver State Machine
     ↓
Peripheral
```

### Level 6 — RTOS Driver

```text
ISR
 ↓
RTOS Synchronization
 ↓
Task
 ↓
Driver
```

### Level 7 — Protocol Driver

```text
Driver
 ↓
Packet
 ↓
CRC
 ↓
ACK/NACK
 ↓
Timeout
 ↓
Retry
```

### Level 8 — Dual-MCU System

```text
STM32F446RE
     ↕
Communication Protocol
     ↕
ESP32
```

---

# 🎓 What This Repository Is Designed to Teach

By completing the entire roadmap, I should gain practical experience in:

### Embedded C

* Structures
* Enumerations
* Pointers
* Function interfaces
* Static functions
* `const`
* `volatile`
* Bit manipulation
* Memory management

### STM32

* GPIO
* EXTI
* Timers
* PWM
* ADC
* Input Capture
* UART
* DMA
* NVIC
* HAL
* CMSIS

### Driver Development

* Driver abstraction
* API design
* Configuration structures
* Error handling
* State machines
* Non-blocking drivers
* Reusable interfaces
* Hardware/software separation

### Communication

* UART
* Packet framing
* CRC
* ACK/NACK
* Timeout
* Retry
* Heartbeat
* Request/response protocols

### Embedded Data Structures

* FIFO
* Circular buffer
* Queue
* Event queue

### Reliability

* Watchdog
* Fault detection
* Recovery
* Diagnostics
* Error codes
* Health monitoring

### RTOS

* Tasks
* Queues
* Semaphores
* Mutexes
* Task notifications
* Software timers
* ISR-to-task communication

### Multi-MCU Systems

* STM32 ↔ ESP32 communication
* Remote sensor access
* Command protocols
* Gateway architecture
* Health monitoring
* Dual-MCU system design

---

# 🏁 Final Goal

After completing all 90 projects, the target architecture is:

```text
                         ┌──────────────────────┐
                         │     Application      │
                         └──────────┬───────────┘
                                    │
                         ┌──────────▼───────────┐
                         │   Driver Framework   │
                         └──────────┬───────────┘
                                    │
              ┌─────────────────────┼─────────────────────┐
              │                     │                     │
          GPIO Driver          ADC Driver           UART Driver
              │                     │                     │
          Timer Driver          DMA Driver          Protocol Driver
              │                     │                     │
              └─────────────────────┼─────────────────────┘
                                    │
                              FreeRTOS Layer
                                    │
                         ┌──────────▼───────────┐
                         │     STM32F446RE      │
                         └──────────┬───────────┘
                                    │
                              UART Protocol
                                    │
                         ┌──────────▼───────────┐
                         │        ESP32         │
                         └──────────┬───────────┘
                                    │
                           ESP32 Driver Layer
```

The final objective is to move from:

> **"I know how to use STM32 HAL APIs."**

to:

> **"I can design, implement, test, debug, and integrate reusable embedded drivers and communication protocols on STM32F446RE."**

---

# 📌 Project Status

| Phase                       | Projects | Status |
| --------------------------- | -------: | ------ |
| GPIO                        |    01–06 | ⬜      |
| Timer & PWM                 |    07–13 | ⬜      |
| ADC                         |    14–20 | ⬜      |
| UART                        |    21–30 | ⬜      |
| STM32 ↔ ESP32 Communication |    31–36 | ⬜      |
| Ultrasonic                  |    37–44 | ⬜      |
| Sensor Integration          |    45–46 | ⬜      |
| Event Architecture          |    47–50 | ⬜      |
| Buffers & Queues            |    51–53 | ⬜      |
| Fault & Diagnostics         |    54–68 | ⬜      |
| State Machines              |    69–70 | ⬜      |
| FreeRTOS                    |    71–80 | ⬜      |
| ESP32 Drivers               |    81–84 | ⬜      |
| Dual-MCU Architecture       |    85–90 | ⬜      |

**Total: 90 driver projects**

---

# 📖 How to Use This Repository

Work through the projects sequentially.

Do not skip directly to advanced projects unless the underlying concept is already understood.

For each project:

```text
Read
 ↓
Understand hardware
 ↓
Configure STM32
 ↓
Build basic version
 ↓
Create driver
 ↓
Test
 ↓
Debug
 ↓
Refactor
 ↓
Document
 ↓
Move to next project
```

The most important goal is not simply to make the code work.

The goal is to understand **why the driver is designed that way**, how it interacts with the MCU peripheral, and how the design can scale into a larger embedded system.

---

## 🏆 End Goal

**90 projects → 1 complete embedded driver-development portfolio**

From:

```text
GPIO Output
```

to:

```text
Dual-MCU STM32 + ESP32 Embedded System
```

with practical experience in:

**STM32 HAL + Embedded C + Drivers + Interrupts + DMA + UART + Protocols + Sensors + FreeRTOS + Fault Handling + Diagnostics + Multi-MCU Architecture.**

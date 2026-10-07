/*
 * main.c
 *
 * STM32 HAL Driver Development - Project 008: HAL UART Interrupt Driver (Loopback Mode)
 *
 * Objective:
 * Demonstrate asynchronous, non-blocking serial communication using UART Interrupts.
 * Data is transmitted on USART1_TX (PA9) and received on USART1_RX (PA10) via a physical jumper wire loopback.
 * Telemetry log messages are retargeted over USART2 (PA2) to the ST-LINK Virtual COM Port.
 *
 * Hardware Mapping (NUCLEO-F446RE):
 * - PA9  -> USART1_TX (Connected via jumper wire to PA10)
 * - PA10 -> USART1_RX (Connected via jumper wire to PA9)
 * - PA2  -> USART2_TX (ST-LINK Virtual COM Port at 115200 baud)
 *
 * Execution Flow:
 * 1. Initialize HAL Library, System Clock (16 MHz HSI), USART2 (printf retargeting), and USART1 (Interrupt mode).
 * 2. Initiate non-blocking interrupt receive: HAL_UART_Receive_IT(&huart1, RxBuffer, DATA_BUFFER_SIZE).
 * 3. Initiate non-blocking interrupt transmit: HAL_UART_Transmit_IT(&huart1, TxBuffer, DATA_BUFFER_SIZE).
 * 4. Hardware generates interrupts as bytes are sent and received on PA9/PA10.
 * 5. NVIC routes interrupt to USART1_IRQHandler(), calling HAL_UART_IRQHandler(&huart1).
 * 6. HAL invokes HAL_UART_TxCpltCallback() and HAL_UART_RxCpltCallback() upon completion.
 * 7. Main loop verifies received data against transmitted payload and reports telemetry via printf().
 */

#include <stdint.h>
#include <stdio.h>                             /* C standard library for printf output formatting */
#include <string.h>                            /* C library for memcmp data verification */
#include "main.h"                              /* Main application header definitions */
#include "uart.h"                              /* Modular UART driver interface */

/* Transmit payload buffer */
uint8_t TxBuffer[DATA_BUFFER_SIZE] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

/* Receive storage buffer (zero-initialized) */
uint8_t RxBuffer[DATA_BUFFER_SIZE] = {0};


/*
 * Main entry point of application.
 */
int main(void)
{
    /* Step 1: Initialize STM32 HAL library, Flash prefetch, and SysTick timer (1 ms tick) */
    HAL_Init();

    /* Step 2: Configure System Clock to 16 MHz using High-Speed Internal (HSI) RC oscillator */
    SystemClock_Config();

    /* Step 3: Initialize USART2 peripheral (115200 baud, 8N1, TX mode) for printf serial logging */
    UART2_Init();

    /* Step 4: Initialize USART1 peripheral (115200 baud, 8N1, TX/RX mode, Interrupt enabled) */
    UART1_Init();

    /* Step 5: Print startup welcome banner to UART console */
    printf("\r\n=======================================================\r\n");
    printf("  STM32 HAL Driver Development: Project 008 - UART IT   \r\n");
    printf("=======================================================\r\n");
    printf("Hardware Setup: Jumper wire connected between PA9 (TX) and PA10 (RX)\r\n");
    printf("Initiating non-blocking Interrupt TX and RX transfer...\r\n\r\n");

    /*
     * Step 6: Start UART non-blocking receive in interrupt mode FIRST.
     * Must be called before Transmit to ensure receive interrupt hardware is primed and listening.
     */
    if (HAL_UART_Receive_IT(&huart1, RxBuffer, DATA_BUFFER_SIZE) != HAL_OK)
    {
        printf("[UART Error] HAL_UART_Receive_IT failed to initialize!\r\n");
        while (1);
    }

    /*
     * Step 7: Start UART non-blocking transmit in interrupt mode.
     * Bytes will be output on PA9 and immediately fed into PA10 via jumper wire.
     */
    if (HAL_UART_Transmit_IT(&huart1, TxBuffer, DATA_BUFFER_SIZE) != HAL_OK)
    {
        printf("[UART Error] HAL_UART_Transmit_IT failed to initialize!\r\n");
        while (1);
    }

    /*
     * Step 8: Main application loop.
     * Monitors completion flags set by UART interrupt callbacks.
     */
    while (1)
    {
        /* Check if both Transmit and Receive interrupts have completed */
        if (g_tx_complete_flag && g_rx_complete_flag)
        {
            /* Reset interrupt flags for next transfer cycle */
            g_tx_complete_flag = 0;
            g_rx_complete_flag = 0;

            printf("[UART IT Complete] TX Counter: %lu | RX Counter: %lu\r\n", TxCounter, RxCounter);

            /* Verify received data against transmitted payload */
            if (memcmp(TxBuffer, RxBuffer, DATA_BUFFER_SIZE) == 0)
            {
                printf("[SUCCESS] Buffer Match Verified! Rx Data: ");
                for (int i = 0; i < DATA_BUFFER_SIZE; i++)
                {
                    printf("%d ", RxBuffer[i]);
                }
                printf("\r\n\r\n");
            }
            else
            {
                printf("[FAIL] Data Mismatch! Rx Buffer does not match Tx Buffer.\r\n\r\n");
            }

            /* Wait 2 seconds before starting next loopback test iteration */
            HAL_Delay(2000);

            /* Clear Rx buffer for next test */
            memset(RxBuffer, 0, DATA_BUFFER_SIZE);

            /* Re-arm Receive IT followed by Transmit IT */
            HAL_UART_Receive_IT(&huart1, RxBuffer, DATA_BUFFER_SIZE);
            HAL_UART_Transmit_IT(&huart1, TxBuffer, DATA_BUFFER_SIZE);
        }

        /* Check for UART hardware errors (overrun, noise, framing) caught in ISR */
        if (g_uart_error_flag)
        {
            g_uart_error_flag = 0;
            printf("[UART Hardware Error] Error flag detected and cleared. Re-arming IT...\r\n");

            /* Clear error flags and re-arm UART IT */
            __HAL_UART_CLEAR_PEFLAG(&huart1);
            __HAL_UART_CLEAR_FEFLAG(&huart1);
            __HAL_UART_CLEAR_NEFLAG(&huart1);
            __HAL_UART_CLEAR_OREFLAG(&huart1);

            huart1.RxState = HAL_UART_STATE_READY;
            huart1.gState = HAL_UART_STATE_READY;

            HAL_UART_Receive_IT(&huart1, RxBuffer, DATA_BUFFER_SIZE);
            HAL_UART_Transmit_IT(&huart1, TxBuffer, DATA_BUFFER_SIZE);
        }
    }
}


/*
 * Configure System Clock to 16 MHz using internal HSI oscillator.
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};   /* Zero-initialize oscillator config structure */
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};   /* Zero-initialize clock bus config structure */

    /* Select High-Speed Internal (HSI) RC oscillator as clock source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                  /* Turn ON HSI oscillator */
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; /* Set default factory calibration */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;              /* Disable PLL (16 MHz HSI is sufficient) */

    /* Apply oscillator configuration to RCC */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1);                             /* Trap CPU if clock initialization fails */
    }

    /* Configure System Clock (SYSCLK) and bus clock prescalers (HCLK, PCLK1, PCLK2) */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;   /* Select HSI (16 MHz) as SYSCLK source */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        /* AHB Prescaler = 1 -> HCLK = 16 MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         /* APB1 Prescaler = 1 -> PCLK1 = 16 MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         /* APB2 Prescaler = 1 -> PCLK2 = 16 MHz */

    /* Apply bus clock prescalers with Flash Latency 0 (for <= 30 MHz operation) */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while (1);                             /* Trap CPU if bus clock setup fails */
    }
}


/*
 * SysTick Interrupt Handler.
 *
 * Called periodically every 1 ms by SysTick hardware timer to maintain HAL tick counter.
 */
void SysTick_Handler(void)
{
    HAL_IncTick();                             /* Increment HAL 1 ms system tick */
}


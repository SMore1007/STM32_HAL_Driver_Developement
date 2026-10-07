/*
 * system_stm32f4xx.c
 *
 * CMSIS Device System Source File for STM32F4xx.
 */

#include <stdint.h>
#include "stm32f4xx.h"

/* System Core Clock Frequency (16 MHz HSI default) */
uint32_t SystemCoreClock = 16000000U;

/*
 * Setup the microcontroller system.
 */
void SystemInit(void)
{
#if defined(__FPU_PRESENT) && (__FPU_PRESENT == 1) && defined(__FPU_USED) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif

  /* Reset vector table offset to FLASH base */
  SCB->VTOR = FLASH_BASE;
}



#include "main.h"
#include "stm32f1xx_it.h"

void SysTick_Handler(void) {
  tick_ms();
}
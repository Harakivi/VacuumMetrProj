#include "main.h"

uint8_t GetKeys()
{
  uint8_t pressedButtons = 0;
  pressedButtons |=  UP_BTN;
  pressedButtons |= 0x1 << ENTER_BTN; 
  pressedButtons |= 0x2 << DOWN_BTN;
  pressedButtons |= 0x3 << LEFT_BTN;
  pressedButtons |= 0x4 << RIGHT_BTN;
  return pressedButtons;
}

void initButt()
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  
  //Инициализация кнопки UP
  GPIOA->CRH &= ~(GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1);
  GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
  GPIOA->CRH |= GPIO_CRH_CNF9_1;
  GPIOA->ODR |= GPIO_ODR_ODR9;

  //Инициализация кнопки ENTER
  GPIOA->CRH &= ~(GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1);
  GPIOA->CRH &= ~GPIO_CRH_CNF10_0;
  GPIOA->CRH |= GPIO_CRH_CNF10_1;
  GPIOA->ODR |= GPIO_ODR_ODR10;
  
  //Инициализация кнопки DOWN
  GPIOA->CRH &= ~(GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1);
  GPIOA->CRH &= ~GPIO_CRH_CNF11_0;
  GPIOA->CRH |= GPIO_CRH_CNF11_1;
  GPIOA->ODR |= GPIO_ODR_ODR11;
  
  //Инициализация кнопки LEFT
  GPIOA->CRH &= ~(GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1);
  GPIOA->CRH &= ~GPIO_CRH_CNF12_0;
  GPIOA->CRH |= GPIO_CRH_CNF12_1;
  GPIOA->ODR |= GPIO_ODR_ODR12;
  
  //Инициализация кнопки RIGHT
  GPIOA->CRH &= ~(GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1);
  GPIOA->CRH &= ~GPIO_CRH_CNF15_0;
  GPIOA->CRH |= GPIO_CRH_CNF15_1;
  GPIOA->ODR |= GPIO_ODR_ODR15;
}


#include "main.h"
#include "BoardConfig.h"

//Обьявление функций
void SysTick_Init(void);

//------------------------------------------------
  
//Инициализация всей периферии
void initAll()
{
  //Впервую очередь инициализируем системное время
  SysTick_Init();
  //Инициализация дисплея 5110LCD
  LCDInit();
  
}

//Инициализация системного таймера для отсчёта времени и реализации задержки
void SysTick_Init(){
  SysTick->LOAD =  SystemCoreClock / 1000 - 1;
  SysTick->VAL = SystemCoreClock / 1000 - 1;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
#include "main.h"
#include "BoardConfig.h"

//Обьявление функций
void SysTick_Init(void);

//------------------------------------------------
  
//Инициализация всей периферии
void initAll()
{
  //Впервую очередь инициализируем системное время
  //SysTick_Init();
  //Инициализация дисплея LCD5110
  LCDInit();
  //Запускаем непрерырвную конвертацию АЦП и запись значений в буфер
  DMA_ADCInit();
  //Иницализируем кнопки
  initButt();
}

//Инициализация системного таймера для отсчёта времени и реализации задержки
void SysTick_Init(){
  SysTick->LOAD =  SystemCoreClock / 1000 - 1;
  SysTick->VAL = SystemCoreClock / 1000 - 1;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
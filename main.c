#include "main.h"

int main()
{
  //Инициализация всей периферии
  initAll();
  for( ; ; )
  {
    TIM3->CCR3 += 100;
    delay_ms(1000);
  }
}

#include "main.h"

extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];
extern xSemaphoreHandle xDISPUpdate;

int main()
{
  //Инициализация всей периферии
  initAll();
  //Инициализация задач
  DISP_Task_init();
  Buttons_Task_init();
  METER_Task_init();
  //Запуск и передача управления шедулеру
  vTaskStartScheduler();
  while(1)
  {
    //Сюда попадать не должны
  }
}

void vApplicationIdleHook(void){ }

void vApplicationTickHook(void){ }

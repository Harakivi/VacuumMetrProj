#include "main.h"

int main()
{
  //Инициализация всей периферии
  initAll();
  //Инициализация задач
  Buttons_Task_init(); //Основная задача в которой происходит обработка нажатий и работа меню
  METER_Task_init(); //Задача главного экрана, работает пока не зашли в меню
  MenuDISP_Task_init(); // Задача обновления экрана во время нахождения в меню в разделе настроек
  //Запуск и передача управления шедулеру
  vTaskStartScheduler();
  while(1)
  {
    //Сюда попадать не должны
  }
}

void vApplicationIdleHook(void){ }

void vApplicationTickHook(void){ }

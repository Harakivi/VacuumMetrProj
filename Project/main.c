#include "main.h"
#include <stdlib.h>
#include <string.h>


int main()
{
  //Инициализация всей периферии
  initAll();
  //Инициализация задач
  Buttons_Task_init(); //Задача в которой происходит обработка нажатий
  Menu_Task_init();//Основная задача в которой происходит работа меню
  METER_Task_init(); //Задача главного экрана, работает пока не зашли в меню
  LASTLEVELMENU_Task_init(); // Задача обновления экрана во время нахождения в разделе настроек
  //Запуск и передача управления шедулеру
  vTaskStartScheduler();
}

void vApplicationIdleHook(void){ }

void vApplicationTickHook(void){ }

void HardFault_Handler()
{
  while (1)
  {
    /* code */
  }
  
}

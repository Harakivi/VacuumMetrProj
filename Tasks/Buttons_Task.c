#include "main.h"

extern Config_Struct *Config;
extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];

//Стурктура со значениями измерителей
extern VDC_Struct SensorVoltageStruct;

//Счётный семафор нажатой кнопки
xSemaphoreHandle xBtnPresSem = NULL;

HeapStats_t Stats;

//Очередь нажатых кнопок
QueueHandle_t xBtnPresQueue = NULL;

//Инициализация задачи и переферии
void Buttons_Task_init(void)
{
  //Инициализируем пины GPIO к которым подключены кнопки
  initButt();

  //Создаём семафор для обработки нажатия кнопок
  xBtnPresSem = xSemaphoreCreateCounting((UBaseType_t)5, (UBaseType_t)0);

  //Создаем очередь нажатых кнопок
  xBtnPresQueue = xQueueCreate(5, sizeof(uint8_t));

  //Проверяем создался ли семафор
  if (xBtnPresSem != NULL && xBtnPresQueue != NULL)
  {
    //Создание задачи отслеживания нажатий кнопок
    xTaskCreate(vBUTTONSCheck_Task, "ButtonsCheck", BUTTONS_STACK_SIZE, NULL, BUTTONS_TASK_PRIORITY, NULL);
  }
}

//Задача отслеживания нажатий кнопок
void vBUTTONSCheck_Task(void *pvParameters)
{
  static const uint8_t UP_BTN = 1;
  static const uint8_t ENTER_BTN = 1 << 1;
  static const uint8_t DOWN_BTN = 1 << 2;
  static const uint8_t LEFT_BTN = 1 << 3;
  static const uint8_t RIGHT_BTN = 1 << 4;
  
  TickType_t lastBtnClkTime = xTaskGetTickCount();
  uint16_t lastBatteryCheckVlolt = 10000;
  uint32_t averageBatteryVoltage = 0;
  uint8_t voltageCountReads = 0;
  //Ожидается что pvParameters будет равен 1
  configASSERT(((uint32_t)pvParameters) == 1);
  for (;;)
  {
    uint16_t pressedTime = 0;
    switch (GetButtons())
    {
    case UP_BTN_CASE:
      while (GetButtons() == UP_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > TIMETOINACTIONBRIGHTOFF)
      {
        DISPLAYBRIGHT = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend(xBtnPresQueue, (void *)&UP_BTN, (TickType_t)10);
      xSemaphoreGive(xBtnPresSem);
      break;
    case ENTER_BTN_CASE:
      while (GetButtons() == ENTER_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > TIMETOINACTIONBRIGHTOFF)
      {
        DISPLAYBRIGHT = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend(xBtnPresQueue, (void *)&ENTER_BTN, (TickType_t)10);
      xSemaphoreGive(xBtnPresSem);
      break;
    case DOWN_BTN_CASE:
      while (GetButtons() == DOWN_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > TIMETOINACTIONBRIGHTOFF)
      {
        DISPLAYBRIGHT = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend(xBtnPresQueue, (void *)&DOWN_BTN, (TickType_t)10);
      xSemaphoreGive(xBtnPresSem);
      break;
    case LEFT_BTN_CASE:
      while (GetButtons() == LEFT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > TIMETOINACTIONBRIGHTOFF)
      {
        DISPLAYBRIGHT = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend(xBtnPresQueue, (void *)&LEFT_BTN, (TickType_t)10);
      xSemaphoreGive(xBtnPresSem);
      break;
    case RIGHT_BTN_CASE:
      while (GetButtons() == RIGHT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > TIMETOINACTIONBRIGHTOFF)
      {
        DISPLAYBRIGHT = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend(xBtnPresQueue, (void *)&RIGHT_BTN, (TickType_t)10);
      xSemaphoreGive(xBtnPresSem);
      break;
    }
//    if (xTaskGetTickCount() - lastBtnClkTime > TIMETOINACTIONBRIGHTOFF)
//    {
//      DISPLAYBRIGHT = 0;
//    }
    if (xTaskGetTickCount() - lastBtnClkTime > TIMETOINACTIONSTANDBY && CHECK_BATT_VOLT)
    {
      enterStandBy();
    }
    if(SensorVoltageStruct.BATT_VDC < 9000)
    {
      if(lastBatteryCheckVlolt == 10000 && SensorVoltageStruct.BATT_VDC <= 9000 && SensorVoltageStruct.BATT_VDC > 8000)
      {
        SaveWorkingTime(xTaskGetTickCount(), 9000);
        lastBatteryCheckVlolt = 9000;
      }
      else if(lastBatteryCheckVlolt == 9000 && SensorVoltageStruct.BATT_VDC <= 8000 && SensorVoltageStruct.BATT_VDC > 7000)
      {
        SaveWorkingTime(xTaskGetTickCount(), 8000);
        lastBatteryCheckVlolt = 8000;
      }
      else if(lastBatteryCheckVlolt == 8000 && SensorVoltageStruct.BATT_VDC <= 7000 && SensorVoltageStruct.BATT_VDC > 6000)
      {
        SaveWorkingTime(xTaskGetTickCount(), 7000);
        lastBatteryCheckVlolt = 7000;
      }
      else if(lastBatteryCheckVlolt == 7000 && SensorVoltageStruct.BATT_VDC <= 6000 && SensorVoltageStruct.BATT_VDC > 5000)
      {
        SaveWorkingTime(xTaskGetTickCount(), 6000);
        lastBatteryCheckVlolt = 6000;
      }
      else if(lastBatteryCheckVlolt == 6000 && SensorVoltageStruct.BATT_VDC <= 5000 && SensorVoltageStruct.BATT_VDC > 4000)
      {
        SaveWorkingTime(xTaskGetTickCount(), 5000);
        lastBatteryCheckVlolt = 5000;
      }
      else if(lastBatteryCheckVlolt == 5000 && SensorVoltageStruct.BATT_VDC <= 4000 && SensorVoltageStruct.BATT_VDC > 3500)
      {
        SaveWorkingTime(xTaskGetTickCount(), 4000);
        lastBatteryCheckVlolt = 4000;
      }
      else if(lastBatteryCheckVlolt == 4000 && SensorVoltageStruct.BATT_VDC <= 3500)
      {
        SaveWorkingTime(xTaskGetTickCount(), 3500);
        lastBatteryCheckVlolt = 3500;
      }
    }
    //Записываем текущее значение напряжения на батарее для усреднения
    averageBatteryVoltage += SensorVoltageStruct.BATT_VDC;
    //Делаем 5 измерений
    if (++voltageCountReads == 5)
    {
      averageBatteryVoltage /= 5;
      //Проверка напряжения на батарее, если меньше MIN_VOLT_ON_BATTERY, то выключаемся
      if (averageBatteryVoltage != 0 && averageBatteryVoltage < MIN_VOLT_ON_BATTERY && CHECK_BATT_VOLT)
      {
        vTaskSuspendAll();
        //Выводим сообщение
        disp_LOWBATT();
        //И засыпаем
        enterStandBy();
      }
      else //Иначе сбрасываем измерения и начинаем заного
      {
        averageBatteryVoltage = 0;
        voltageCountReads = 0;
      }
    }

    vPortGetHeapStats(&Stats);
    vTaskDelay(50);
  }
}
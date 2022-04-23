#include "main.h"

extern Config_Struct* Config;
extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];

VoltagesOnBatteryEnum VoltagesOnBattery = FULL_VOLT_ON_BATTERY;

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
  xBtnPresSem = xSemaphoreCreateCounting( (UBaseType_t) 5, (UBaseType_t) 0);
  
  //Создаем очередь нажатых кнопок
  xBtnPresQueue = xQueueCreate( 5, sizeof( uint8_t ) );

  
  //Проверяем создался ли семафор
  if(xBtnPresSem != NULL && xBtnPresQueue != NULL)
  {
    //Создание задачи отслеживания нажатий кнопок
    xTaskCreate(vBUTTONSCheck_Task, "ButtonsCheck", BUTTONS_STACK_SIZE, NULL, BUTTONS_TASK_PRIORITY, NULL);
  }
}

//Задача отслеживания нажатий кнопок
void vBUTTONSCheck_Task (void *pvParameters)
{
  static const uint8_t UP_BTN = 1;
  static const uint8_t ENTER_BTN = 1 << 1;
  static const uint8_t DOWN_BTN = 1 << 2;
  static const uint8_t LEFT_BTN = 1 << 3;
  static const uint8_t RIGHT_BTN = 1 << 4;
  TickType_t lastBtnClkTime = xTaskGetTickCount();
  uint32_t averageBatteryVoltage = 0;
  uint8_t voltageCountReads = 0;
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  for (;;)
  {
    uint16_t pressedTime = 0;
    switch(GetButtons())
    {
    case UP_BTN_CASE:
      while (GetButtons() == UP_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > Config->dimmerOff)
      {
        DISPLAYBRIGHT_REG = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend( xBtnPresQueue, ( void * )&UP_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case ENTER_BTN_CASE:
      while (GetButtons() == ENTER_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > Config->dimmerOff)
      {
        DISPLAYBRIGHT_REG = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend( xBtnPresQueue, ( void * )&ENTER_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case DOWN_BTN_CASE:
      while (GetButtons() == DOWN_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > Config->dimmerOff)
      {
        DISPLAYBRIGHT_REG = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend( xBtnPresQueue, ( void * )&DOWN_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case LEFT_BTN_CASE:
      while (GetButtons() == LEFT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);      
      }
      if(xTaskGetTickCount() - lastBtnClkTime > Config->dimmerOff)
      {
        DISPLAYBRIGHT_REG = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend( xBtnPresQueue, ( void * )&LEFT_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case RIGHT_BTN_CASE:
      while (GetButtons() == RIGHT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      if(xTaskGetTickCount() - lastBtnClkTime > Config->dimmerOff)
      {
        DISPLAYBRIGHT_REG = Config->Bright;
      }
      lastBtnClkTime = xTaskGetTickCount();
      xQueueSend( xBtnPresQueue, ( void * )&RIGHT_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    }
    if(xTaskGetTickCount() - lastBtnClkTime > Config->dimmerOff)
    {
      DISPLAYBRIGHT_REG = 0;
    }
    if(xTaskGetTickCount() - lastBtnClkTime > Config->deviceOff)
    {
      enterStandBy();
    }
    //Записываем текущее значение напряжения на батарее для усреднения
    averageBatteryVoltage += SensorVoltageStruct.BATT_VDC;
    //Делаем 5 измерений
    if(++voltageCountReads == 10)
    {
      averageBatteryVoltage /= 10;
      if(averageBatteryVoltage > FULL_VOLT_ON_BATTERY)
      {
        VoltagesOnBattery = FULL_VOLT_ON_BATTERY;
        xSemaphoreGive( xBtnPresSem);
      }
      else if(averageBatteryVoltage < FULL_VOLT_ON_BATTERY && averageBatteryVoltage > MID_VOLT_ON_BATTERY )
      {
        VoltagesOnBattery = MID_VOLT_ON_BATTERY;
        xSemaphoreGive( xBtnPresSem);
      }
      else if(averageBatteryVoltage < MID_VOLT_ON_BATTERY && averageBatteryVoltage > LOW_VOLT_ON_BATTERY )
      {
        VoltagesOnBattery = LOW_VOLT_ON_BATTERY;
        xSemaphoreGive( xBtnPresSem);
      }
      else if(averageBatteryVoltage < LOW_VOLT_ON_BATTERY )
      {
        VoltagesOnBattery = EMPTY_VOLT_ON_BATTERY;
        xSemaphoreGive( xBtnPresSem);
      }
      //Проверка напряжения на батарее, если меньше EMPTY_VOLT_ON_BATTERY, то выключаемся
      if(averageBatteryVoltage != 0 && averageBatteryVoltage < EMPTY_VOLT_ON_BATTERY)
      {
#if defined (CHECK_BATT_VOLT)
        vTaskSuspendAll();
        //Выводим сообщение
        disp_LOWBATT();
        //И засыпаем
        enterStandBy();
#endif
      }else//Иначе сбрасываем измерения и начинаем заного
      {
        averageBatteryVoltage = 0;
        voltageCountReads = 0;
      }
    }
    
    vPortGetHeapStats( &Stats );
    vTaskDelay(50);
  }
}
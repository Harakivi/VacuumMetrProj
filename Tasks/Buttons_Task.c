#include "main.h"

extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];

extern uint8_t UP_BTN;
extern uint8_t ENTER_BTN;
extern uint8_t DOWN_BTN;
extern uint8_t LEFT_BTN;
extern uint8_t RIGHT_BTN;

//Бинарный семафор нажатой кнопки
xSemaphoreHandle xBtnPresSem = NULL;

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
    //Создание задачи обновления дисплея
    xTaskCreate(vBUTTONSCheck_Task, "ButtonsCheck", BUTTONS_STACK_SIZE, NULL, BUTTONS_TASK_PRIORITY, NULL);
  }
}

//Задача обновления дислпея
void vBUTTONSCheck_Task (void *pvParameters)
{
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
      xQueueSend( xBtnPresQueue, ( void * )&UP_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case ENTER_BTN_CASE:
      while (GetButtons() == ENTER_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      xQueueSend( xBtnPresQueue, ( void * )&ENTER_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case DOWN_BTN_CASE:
      while (GetButtons() == DOWN_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      xQueueSend( xBtnPresQueue, ( void * )&DOWN_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case LEFT_BTN_CASE:
      while (GetButtons() == LEFT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);      
      }
      xQueueSend( xBtnPresQueue, ( void * )&LEFT_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case RIGHT_BTN_CASE:
      while (GetButtons() == RIGHT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      xQueueSend( xBtnPresQueue, ( void * )&RIGHT_BTN, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    }
    vTaskDelay(50);
  }
}
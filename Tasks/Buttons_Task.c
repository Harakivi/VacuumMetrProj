#include "main.h"

extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];

//Бинарный семафор нажатой кнопки
xSemaphoreHandle xBtnPresSem = NULL;

//Очередь нажатых кнопок 
QueueHandle_t xBtnPresQueue = NULL;

uint8_t pressedBtn[5] = {0};

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
    case UP_BTN:
      while (GetButtons() == UP_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      pressedBtn[0] = UP_BTN;
      xQueueSend( xBtnPresQueue, ( void * )pressedBtn, ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case ENTER_BTN:
      while (GetButtons() == ENTER_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      pressedBtn[1] = ENTER_BTN;
      xQueueSend( xBtnPresQueue, ( void * )(pressedBtn + 1), ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case DOWN_BTN:
      while (GetButtons() == DOWN_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      pressedBtn[2] = DOWN_BTN;
      xQueueSend( xBtnPresQueue, ( void * )(pressedBtn + 2), ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case LEFT_BTN:
      while (GetButtons() == LEFT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);      
      }
      pressedBtn[3] = LEFT_BTN;
      xQueueSend( xBtnPresQueue, ( void * )(pressedBtn + 3), ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    case RIGHT_BTN:
      while (GetButtons() == RIGHT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      pressedBtn[4] = RIGHT_BTN;
      xQueueSend( xBtnPresQueue, ( void * )(pressedBtn + 4), ( TickType_t ) 10 );
      xSemaphoreGive( xBtnPresSem);
      break;
    }
    vTaskDelay(50);
  }
}
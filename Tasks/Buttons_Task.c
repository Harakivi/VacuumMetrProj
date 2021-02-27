#include "main.h"

extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];

//Бинарный семафор нажатой кнопки
xSemaphoreHandle xButtonPressed = NULL;

//Инициализация задачи и переферии
void Buttons_Task_init(void)
{
  //Инициализируем пины GPIO к которым подключены кнопки
  //initButt();
  
  //Создаём семафор для обновления дисплея
  vSemaphoreCreateBinary( xButtonPressed );  
  
  //Проверяем создался ли семафор
  if(xButtonPressed != NULL)
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
  //Забираем семафор
  xSemaphoreTake( xButtonPressed, portMAX_DELAY );
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
      menuBtnProc(UP_BTN);
      break;
    case ENTER_BTN:
      while (GetButtons() == ENTER_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      menuBtnProc(ENTER_BTN);
      break;
    case DOWN_BTN:
      while (GetButtons() == DOWN_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      menuBtnProc(DOWN_BTN);
      break;
    case LEFT_BTN:
      while (GetButtons() == LEFT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);      
      }
      menuBtnProc(LEFT_BTN);
      break;
    case RIGHT_BTN:
      while (GetButtons() == RIGHT_BTN)
      {
        pressedTime += 50;
        vTaskDelay(50);
      }
      menuBtnProc(RIGHT_BTN);
      break;
    }
    vTaskDelay(50);
  }
}
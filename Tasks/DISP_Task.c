#include "main.h"

extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];

//Бинарный семафор обновления дислпея
xSemaphoreHandle xDISPUpdate = NULL;

//Инициализация задачи и переферии
void DISP_Task_init(void)
{
  //Инициализируем дисплей и переферию
  DISP_Init();
  
  //Создаём семафор для обновления дисплея
  vSemaphoreCreateBinary( xDISPUpdate );  
  
  //Проверяем создался ли семафор
  if(xDISPUpdate != NULL)
  {
    //Создание задачи обновления дисплея
    xTaskCreate(vDISPUpdate_Task, "DisplayUpdate", DISP_STACK_SIZE, NULL, DISP_TASK_PRIORITY, NULL);
  }
}

//Задача обновления дислпея
void vDISPUpdate_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  //Забираем семафор
  xSemaphoreTake( xDISPUpdate, portMAX_DELAY );
  //один раз обновляем дислпей
  DISP_Send_DMA(bender);
  vTaskDelay(2000);
  menuVBUFDraw();
  for (;;)
  {
    //Пытаемся взять семафор
    xSemaphoreTake( xDISPUpdate, portMAX_DELAY );
    //Если семафор свободен, то обновляем экран
    DISP_Send_DMA(VBUF);
  }
}

//Функция отдающая семафор для выполнения задачи загрузки видео буфера в дислпей
void DISP_Update(void)
{
  //Отдаем семафор, чтобы обновить дисплей
  xSemaphoreGive( xDISPUpdate );
}
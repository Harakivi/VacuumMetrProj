//-------------------Инклюды--------------------------
#include "main.h"
#include "Buttons.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern VDC_Struct SensorVoltageStruct;

TaskHandle_t  xScopeHandle = NULL;
TaskHandle_t  xAccumHandle = NULL;

SCOPE_Struct SCOPE = {
                     0x0, //PDE_Array;
                      84, //Data_CNT
                      1,  //Time_Delay
                      1,  //Lim_Div
                      0,  //Average
                        };
                          
//Бинарный семафор конца накопления данных
xSemaphoreHandle xAccumDone = NULL;
//Бинарный семафор конца отрисовки данных
xSemaphoreHandle xAccumDispDone = NULL;

//Инициализация задачи
void SCOPE_Task_init(void)
{
  taskENTER_CRITICAL();
  //Создаём семафор 
  if(xAccumDone == NULL)
  {
    xAccumDone = xSemaphoreCreateBinary();
  }
  if(xAccumDispDone == NULL)
  {
    xAccumDispDone = xSemaphoreCreateBinary();
  }
    
  xTaskCreate(vSCOPE_Task, "SCOPE", SCOPE_STACK_SIZE, NULL, SCOPE_TASK_PRIORITY, &xScopeHandle);
  xTaskCreate(vACCUM_Task, "ACCUM", ACCUM_STACK_SIZE, NULL, ACCUM_TASK_PRIORITY, &xAccumHandle);
    
  //Сначала выдаём семафор на обновление экрана
  xSemaphoreGive(xAccumDone);
    
  taskEXIT_CRITICAL();
}

//Удаление задачи
void SCOPE_Task_Deinit(void)
{
  taskENTER_CRITICAL();
  if(xScopeHandle != NULL)
  {
    vTaskDelete(xScopeHandle);
    xScopeHandle = NULL;
  }
  if(xAccumHandle != NULL)
  {
    vTaskDelete(xAccumHandle);
    xAccumHandle = NULL;
  }
  if(SCOPE.PDE_Array != NULL)
  {
    vPortFree(SCOPE.PDE_Array);
    SCOPE.PDE_Array = NULL;
  }
  taskEXIT_CRITICAL();
}

//Задача обновления дислпея
void vSCOPE_Task(void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  for (;;)
  {
    //Берем семафор и ждём
    xSemaphoreTake( xAccumDone, portMAX_DELAY );
    VBUF_Clear();
      
    for(int i = 0; i < SCOPE.Data_CNT; i++)
    {
      VBUF_Draw_Line(i, 24 + ((SCOPE.PDE_Array[i] - SCOPE.Average) / SCOPE.Lim_Div),  1, 1);
    }
      
    DISP_Update();
    xSemaphoreGive(xAccumDispDone);
    vTaskDelay(32);
  }
}

void vACCUM_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  SCOPE.PDE_Array = pvPortMalloc(sizeof(int16_t) * SCOPE.Data_CNT);
  for (;;)
  {
   
    //Берем семафор и ждём
    xSemaphoreTake( xAccumDispDone, portMAX_DELAY );
    SCOPE.Average = 0;
    for(int i = 0; i < SCOPE.Data_CNT; i++)
    {
      SCOPE.PDE_Array[i] =  SensorVoltageStruct.PDE1_VDC;
      SCOPE.Average += SCOPE.PDE_Array[i];
      vTaskDelay(SCOPE.Time_Delay);
    }
    SCOPE.Average /= SCOPE.Data_CNT;
    xSemaphoreGive(xAccumDone);
  }
}
#include "main.h"
#include <stdio.h>

extern VDC_Struct SensorVoltageStruct;

TaskHandle_t xMeterHandle;

//Инициализация задачи
void METER_Task_init(void)
{
  //Создание задачи обновления дисплея
  xTaskCreate(vMETER_Task, "METER", DISP_STACK_SIZE, NULL, DISP_TASK_PRIORITY, &xMeterHandle);
  vTaskSuspend(xMeterHandle);
}

//Задача обновления дислпея
void vMETER_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  for (;;)
  {
    char string[84] = {0};
    sprintf(string, "%u", SensorVoltageStruct.PDE1_VDC);
    VBUF_Write_String(27,11,string);
    sprintf(string, "%u", SensorVoltageStruct.PDE2_VDC);
    VBUF_Write_String(27,20,string);
    sprintf(string, "%u", SensorVoltageStruct.PDE3_VDC);
    VBUF_Write_String(27,29,string);
    sprintf(string, "%u", SensorVoltageStruct.PDE4_VDC);
    VBUF_Write_String(27,38,string);
    DISP_Update();
    vTaskDelay(40);
  }
}
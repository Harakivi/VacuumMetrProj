#include "main.h"
#include <stdio.h>

extern VDC_Struct SensorVoltageStruct;

TaskHandle_t  xMeterHandle = NULL;

//Инициализация задачи
void METER_Task_init(void)
{
  xTaskCreate(vMETER_Task, "METER", DISP_STACK_SIZE, NULL, DISP_TASK_PRIORITY, &xMeterHandle);
}

//Задача обновления дислпея
void vMETER_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  
  for (;;)
  {
    char string[84] = {0};
    VBUF_Clear();
    VBUF_Draw_Line(19, 0, -48, 1);
    sprintf(string, "%i", SensorVoltageStruct.PDE1_VDC / 10);
    VBUF_Draw_Line(3, 9, -37, 1);
    VBUF_Draw_Line(4, 10, 1, 8);
    VBUF_Draw_Line(12, 9, -37, 1);
    VBUF_Draw_Line(4, 46, 1, 8);
    VBUF_Draw_Line(5, 28, SensorVoltageStruct.PDE1_VDC / 30, 6);
    VBUF_Write_String(0,0,string);
    VBUF_Draw_Line(41, 0, -48, 1);
    sprintf(string, "%i", SensorVoltageStruct.PDE2_VDC / 10);
    VBUF_Draw_Line(23, 9, -37, 1);
    VBUF_Draw_Line(24, 10, 1, 8);
    VBUF_Draw_Line(32, 9, -37, 1);
    VBUF_Draw_Line(24, 46, 1, 8);
    VBUF_Draw_Line(25, 28, SensorVoltageStruct.PDE2_VDC / 30, 6);
    VBUF_Write_String(22,0,string);
    VBUF_Draw_Line(63, 0, -48, 1);
    sprintf(string, "%i", SensorVoltageStruct.PDE3_VDC / 10);
    VBUF_Draw_Line(45, 9, -37, 1);
    VBUF_Draw_Line(46, 10, 1, 8);
    VBUF_Draw_Line(54, 9, -37, 1);
    VBUF_Draw_Line(46, 46, 1, 8);
    VBUF_Draw_Line(47, 28, SensorVoltageStruct.PDE3_VDC / 30, 6);
    VBUF_Write_String(44,0,string);
    sprintf(string, "%i", SensorVoltageStruct.PDE4_VDC / 10);
    VBUF_Draw_Line(66, 9, -37, 1);
    VBUF_Draw_Line(67, 10, 1, 8);
    VBUF_Draw_Line(75, 9, -37, 1);
    VBUF_Draw_Line(67, 46, 1, 8);
    VBUF_Draw_Line(68, 28, SensorVoltageStruct.PDE4_VDC / 30, 6);
    VBUF_Write_String(66,0,string);
    DISP_Update();
    vTaskDelay(32);
  }
}
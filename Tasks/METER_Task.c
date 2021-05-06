//-------------------Инклюды--------------------------
#include "main.h"
#include "Buttons.h"
#include <stdio.h>
#include <stdlib.h>

extern VDC_Struct SensorVoltageStruct;
HeapStats_t pxHeapStats;

TaskHandle_t  xMeterHandle = NULL;
FILT_Struct FILT = {
                      10,
                      0,
                      0,
                      0,
                      0
                        };

void* memoryToFiltAlloc(uint8_t sizeToAlloc)
{
  //return malloc(sizeToAlloc);
  return pvPortMalloc(sizeToAlloc);
}

int16_t filtAverage(int16_t* pointer, uint8_t size)
{
  int32_t average = 0;
  for(int i = 0; i < size; i++)
  {
    average += *pointer++;
  }
  average /= size;
  return (int16_t)average;
}

void memoryToFiltFree(void* memoryToFree)
{
  //free(memoryToFree);
  vPortFree(memoryToFree);
}

//Инициализация задачи
void METER_Task_init(void)
{
  xTaskCreate(vMETER_Task, "METER", METER_STACK_SIZE, NULL, METER_TASK_PRIORITY, &xMeterHandle);
  xTaskCreate(vFILTER_Task, "FILTER", METER_STACK_SIZE, NULL, METER_TASK_PRIORITY, NULL);
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
    sprintf(string, "%i", FILT.PDE1_Value / 10);
    VBUF_Draw_Line(3, 9, -37, 1);
    VBUF_Draw_Line(4, 10, 1, 8);
    VBUF_Draw_Line(12, 9, -37, 1);
    VBUF_Draw_Line(4, 46, 1, 8);
    VBUF_Draw_Line(5, 28, FILT.PDE1_Value / 37, 6);
    VBUF_Write_String(0,0,string);
    VBUF_Draw_Line(41, 0, -48, 1);
    sprintf(string, "%i", FILT.PDE2_Value / 10);
    VBUF_Draw_Line(23, 9, -37, 1);
    VBUF_Draw_Line(24, 10, 1, 8);
    VBUF_Draw_Line(32, 9, -37, 1);
    VBUF_Draw_Line(24, 46, 1, 8);
    VBUF_Draw_Line(25, 28, FILT.PDE2_Value / 37, 6);
    VBUF_Write_String(22,0,string);
    VBUF_Draw_Line(63, 0, -48, 1);
    sprintf(string, "%i", FILT.PDE3_Value / 10);
    VBUF_Draw_Line(45, 9, -37, 1);
    VBUF_Draw_Line(46, 10, 1, 8);
    VBUF_Draw_Line(54, 9, -37, 1);
    VBUF_Draw_Line(46, 46, 1, 8);
    VBUF_Draw_Line(47, 28, FILT.PDE3_Value / 37, 6);
    VBUF_Write_String(44,0,string);
    sprintf(string, "%i", FILT.PDE4_Value / 10);
    VBUF_Draw_Line(66, 9, -37, 1);
    VBUF_Draw_Line(67, 10, 1, 8);
    VBUF_Draw_Line(75, 9, -37, 1);
    VBUF_Draw_Line(67, 46, 1, 8);
    VBUF_Draw_Line(68, 28, FILT.PDE4_Value / 37, 6);
    VBUF_Write_String(66,0,string);
    DISP_Update();
    vTaskDelay(32);
  }
}

//Задача фильтрации данных
void vFILTER_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  uint8_t currentSizeFilt = FILT.SizeFilt;
  FILT.pPDE1_Array = memoryToFiltAlloc(currentSizeFilt * sizeof(int16_t));
  FILT.pPDE2_Array = memoryToFiltAlloc(currentSizeFilt * sizeof(int16_t));
  FILT.pPDE3_Array = memoryToFiltAlloc(currentSizeFilt * sizeof(int16_t));
  FILT.pPDE4_Array = memoryToFiltAlloc(currentSizeFilt * sizeof(int16_t));
  uint8_t currentArrayPos = 0;
  for (;;)
  {
    FILT.pPDE1_Array[currentArrayPos] = SensorVoltageStruct.PDE1_VDC;
    FILT.pPDE2_Array[currentArrayPos] = SensorVoltageStruct.PDE2_VDC;
    FILT.pPDE3_Array[currentArrayPos] = SensorVoltageStruct.PDE3_VDC;
    FILT.pPDE4_Array[currentArrayPos] = SensorVoltageStruct.PDE4_VDC;
    FILT.PDE1_Value = filtAverage(FILT.pPDE1_Array, currentSizeFilt);
    FILT.PDE2_Value = filtAverage(FILT.pPDE2_Array, currentSizeFilt);
    FILT.PDE3_Value = filtAverage(FILT.pPDE3_Array, currentSizeFilt);
    FILT.PDE4_Value = filtAverage(FILT.pPDE4_Array, currentSizeFilt);
    if(currentArrayPos == currentSizeFilt)
    {
      currentArrayPos = 0;
    }else { currentArrayPos++;}
    if(currentSizeFilt != FILT.SizeFilt)
    {
      taskENTER_CRITICAL();
      memoryToFiltFree((void*)FILT.pPDE1_Array);
      memoryToFiltFree((void*)FILT.pPDE2_Array);
      memoryToFiltFree((void*)FILT.pPDE3_Array);
      memoryToFiltFree((void*)FILT.pPDE4_Array);
      currentSizeFilt = FILT.SizeFilt;
      FILT.pPDE1_Array = memoryToFiltAlloc(sizeof(FILT.PDE1_Value) * currentSizeFilt);
      FILT.pPDE2_Array = memoryToFiltAlloc(sizeof(FILT.PDE2_Value) * currentSizeFilt);
      FILT.pPDE3_Array = memoryToFiltAlloc(sizeof(FILT.PDE3_Value) * currentSizeFilt);
      FILT.pPDE4_Array = memoryToFiltAlloc(sizeof(FILT.PDE4_Value) * currentSizeFilt);
      taskEXIT_CRITICAL();
    }
    vPortGetHeapStats(&pxHeapStats);
    vTaskDelay(10);
  }
}
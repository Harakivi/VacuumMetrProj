//-------------------Инклюды--------------------------
#include "main.h"
#include "Buttons.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern VDC_Struct SensorVoltageStruct;

//Бинарный семафор конца работы фильтра
xSemaphoreHandle xFilterDone = NULL;

TaskHandle_t  xMeterHandle = NULL;
TaskHandle_t  xFilterHandle = NULL;

FILT_Struct FILT = {
                      0, //PDE1_Value
                      0, //PDE2_Value
                      0, //PDE3_Value
                      0, //PDE4_Value
                    0x0, //PDE1_Array;
                    0x0, //PDE2_Array;
                    0x0, //PDE3_Array;
                    0x0, //PDE4_Array;
                      1, //Data_CNT
                      TWO_DIGITS  //Dig_CNT
                        };

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

//Инициализация задачи
void METER_Task_init(void)
{
  taskENTER_CRITICAL();
  //Создаём семафор 
  if(xFilterDone == NULL)
  {
    xFilterDone = xSemaphoreCreateBinary();
  }
    
  xTaskCreate(vMETER_Task, "METER", METER_STACK_SIZE, NULL, METER_TASK_PRIORITY, &xMeterHandle);
  xTaskCreate(vFILTER_Task, "FILTER", FILTER_STACK_SIZE, NULL, FILTER_TASK_PRIORITY, &xFilterHandle);
  
  taskEXIT_CRITICAL();
}

//Удаление задачи
void METER_Task_Deinit(void)
{
  taskENTER_CRITICAL();
  if(xMeterHandle != NULL)
  {
    vTaskDelete(xMeterHandle);
    xMeterHandle = NULL;
  }
  if(xFilterHandle != NULL)
  {
    vTaskDelete(xFilterHandle);
    xFilterHandle = NULL;
  }
  if(FILT.PDE1_Array != NULL)
  {
    vPortFree(FILT.PDE1_Array);
    FILT.PDE1_Array = NULL;
  }
  if(FILT.PDE2_Array != NULL)
  {
    vPortFree(FILT.PDE2_Array);
    FILT.PDE2_Array = NULL;
  }
  if(FILT.PDE3_Array != NULL)
  {
    vPortFree(FILT.PDE3_Array);
    FILT.PDE3_Array = NULL;
  }
  if(FILT.PDE4_Array != NULL)
  {
    vPortFree(FILT.PDE4_Array);
    FILT.PDE4_Array = NULL;
  }
  taskEXIT_CRITICAL();
}

////Задача обновления дислпея
//void vMETER_Task (void *pvParameters)
//{
//  //Ожидается что pvParameters будет равен 1
//  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
//  for (;;)
//  {
//    char string[84] = {0};
//    VBUF_Clear();
//    //VBUF_Draw_Line(19, 0, -48, 1); //Разделитель
//    //-------------------Цифровое значение PDE1--------------------------
//    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE1_Value) : FILT.PDE1_Value / 10);
//    VBUF_Write_String(0,0,string);
//    //-------------------Рамка визуальной индикации PDE1--------------------------
//    VBUF_Draw_Line(0, 9, -37, 1);//Вертикальная линия слева
//    VBUF_Draw_Line(1, 10, 1, 8);//Горизонтальная линия линия сверху
//    VBUF_Draw_Line(9, 9, -37, 1);//Вертикальная линия справа
//    VBUF_Draw_Line(1, 46, 1, 8);//Горизонтальная линия линия снизу
//    //-------------------Визуальная индикация PDE1--------------------------
//    VBUF_Draw_Line(2, 28, FILT.PDE1_Value / 37, 6);
//    //VBUF_Draw_Line(41, 0, -48, 1); //Разделитель
//    //-------------------Цифровое значение PDE2--------------------------
//    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE2_Value) : FILT.PDE2_Value / 10);
//    VBUF_Write_String(22,0,string);
//    //-------------------Рамка визуальной индикации PDE2--------------------------
//    VBUF_Draw_Line(22, 9, -37, 1);//Вертикальная линия слева
//    VBUF_Draw_Line(23, 10, 1, 8);//Горизонтальная линия линия сверху
//    VBUF_Draw_Line(31, 9, -37, 1);//Вертикальная линия справа
//    VBUF_Draw_Line(23, 46, 1, 8);//Горизонтальная линия линия снизу
//    //-------------------Визуальная индикация PDE2--------------------------
//    VBUF_Draw_Line(24, 28, FILT.PDE2_Value / 37, 6);
//    //VBUF_Draw_Line(63, 0, -48, 1); //Разделитель
//    //-------------------Цифровое значение PDE3--------------------------
//    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE3_Value) : FILT.PDE3_Value / 10);
//    VBUF_Write_String_Right(56,0,string);
//    //-------------------Рамка визуальной индикации PDE3--------------------------
//    VBUF_Draw_Line(51, 9, -37, 1);//Вертикальная линия слева
//    VBUF_Draw_Line(52, 10, 1, 8);//Горизонтальная линия линия сверху
//    VBUF_Draw_Line(60, 9, -37, 1);//Вертикальная линия справа
//    VBUF_Draw_Line(52, 46, 1, 8);//Горизонтальная линия линия снизу
//    //-------------------Визуальная индикация PDE3--------------------------
//    VBUF_Draw_Line(53, 28, FILT.PDE3_Value / 37, 6);
//    //-------------------Цифровое значение PDE4--------------------------
//    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE4_Value) : FILT.PDE4_Value / 10);
//    VBUF_Write_String_Right(79,0,string);
//    //-------------------Рамка визуальной индикации PDE4--------------------------
//    VBUF_Draw_Line(74, 9, -37, 1);//Вертикальная линия слева
//    VBUF_Draw_Line(75, 10, 1, 8);//Горизонтальная линия линия сверху
//    VBUF_Draw_Line(83, 9, -37, 1);//Вертикальная линия справа
//    VBUF_Draw_Line(75, 46, 1, 8);//Горизонтальная линия линия снизу
//    //-------------------Визуальная индикация PDE4--------------------------
//    VBUF_Draw_Line(76, 28, FILT.PDE4_Value / 37, 6);
//    
//    //-------------------Визуализация усреднения--------------------------
//    VBUF_Draw_Line(40, 44, FILT.FILT_Delay / 5, 2);
//    VBUF_Draw_Line(38, 19, -27, 1);//Вертикальная линия слева
//    VBUF_Draw_Line(39, 20, 1, 4);//Горизонтальная линия линия сверху
//    VBUF_Draw_Line(43, 19, -27, 1);//Вертикальная линия справа
//    VBUF_Draw_Line(39, 46, 1, 4);//Горизонтальная линия линия снизу
//    
//    
//    DISP_Update();
//    vTaskDelay(32);
//  }
//}
//
////Задача фильтрации данных
//void vFILTER_Task (void *pvParameters)
//{
//  //Ожидается что pvParameters будет равен 1
//  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
//  uint8_t currentArrayPos = 0;
//  int16_t pPDE1_Array[FILTER_DATA_SIZE] = {0};
//  int16_t pPDE2_Array[FILTER_DATA_SIZE] = {0};
//  int16_t pPDE3_Array[FILTER_DATA_SIZE] = {0};
//  int16_t pPDE4_Array[FILTER_DATA_SIZE] = {0};
//  for (;;)
//  {
//    pPDE1_Array[currentArrayPos] =  SensorVoltageStruct.PDE1_VDC;
//    pPDE2_Array[currentArrayPos] =  SensorVoltageStruct.PDE2_VDC;
//    pPDE3_Array[currentArrayPos] =  SensorVoltageStruct.PDE3_VDC;
//    pPDE4_Array[currentArrayPos] =  SensorVoltageStruct.PDE4_VDC;
//    FILT.PDE1_Value = filtAverage(pPDE1_Array, FILTER_DATA_SIZE);
//    FILT.PDE2_Value = filtAverage(pPDE2_Array, FILTER_DATA_SIZE);
//    FILT.PDE3_Value = filtAverage(pPDE3_Array, FILTER_DATA_SIZE);
//    FILT.PDE4_Value = filtAverage(pPDE4_Array, FILTER_DATA_SIZE);
//    if(currentArrayPos++ == FILTER_DATA_SIZE)
//    {
//      currentArrayPos = 0;
//    }
//    vTaskDelay(FILT.FILT_Delay);
//  }
//}

//Задача обновления дислпея
void vMETER_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  for (;;)
  {
    xSemaphoreTake( xFilterDone, portMAX_DELAY );
    char string[84] = {0};
    VBUF_Clear();
    //VBUF_Draw_Line(19, 0, -48, 1); //Разделитель
    //-------------------Цифровое значение PDE1--------------------------
    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE1_Value) : FILT.PDE1_Value / 10);
    VBUF_Write_String(0,0,string);
    //-------------------Рамка визуальной индикации PDE1--------------------------
    VBUF_Draw_Line(0, 9, -37, 1);//Вертикальная линия слева
    VBUF_Draw_Line(1, 10, 1, 8);//Горизонтальная линия линия сверху
    VBUF_Draw_Line(9, 9, -37, 1);//Вертикальная линия справа
    VBUF_Draw_Line(1, 46, 1, 8);//Горизонтальная линия линия снизу
    //-------------------Визуальная индикация PDE1--------------------------
    VBUF_Draw_Line(2, 28, FILT.PDE1_Value / 37, 6);
    //VBUF_Draw_Line(41, 0, -48, 1); //Разделитель
    //-------------------Цифровое значение PDE2--------------------------
    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE2_Value) : FILT.PDE2_Value / 10);
    VBUF_Write_String(22,0,string);
    //-------------------Рамка визуальной индикации PDE2--------------------------
    VBUF_Draw_Line(22, 9, -37, 1);//Вертикальная линия слева
    VBUF_Draw_Line(23, 10, 1, 8);//Горизонтальная линия линия сверху
    VBUF_Draw_Line(31, 9, -37, 1);//Вертикальная линия справа
    VBUF_Draw_Line(23, 46, 1, 8);//Горизонтальная линия линия снизу
    //-------------------Визуальная индикация PDE2--------------------------
    VBUF_Draw_Line(24, 28, FILT.PDE2_Value / 37, 6);
    //VBUF_Draw_Line(63, 0, -48, 1); //Разделитель
    //-------------------Цифровое значение PDE3--------------------------
    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE3_Value) : FILT.PDE3_Value / 10);
    VBUF_Write_String_Right(56,0,string);
    //-------------------Рамка визуальной индикации PDE3--------------------------
    VBUF_Draw_Line(51, 9, -37, 1);//Вертикальная линия слева
    VBUF_Draw_Line(52, 10, 1, 8);//Горизонтальная линия линия сверху
    VBUF_Draw_Line(60, 9, -37, 1);//Вертикальная линия справа
    VBUF_Draw_Line(52, 46, 1, 8);//Горизонтальная линия линия снизу
    //-------------------Визуальная индикация PDE3--------------------------
    VBUF_Draw_Line(53, 28, FILT.PDE3_Value / 37, 6);
    //-------------------Цифровое значение PDE4--------------------------
    sprintf(string, "%i", FILT.Dig_CNT ? abs(FILT.PDE4_Value) : FILT.PDE4_Value / 10);
    VBUF_Write_String_Right(79,0,string);
    //-------------------Рамка визуальной индикации PDE4--------------------------
    VBUF_Draw_Line(74, 9, -37, 1);//Вертикальная линия слева
    VBUF_Draw_Line(75, 10, 1, 8);//Горизонтальная линия линия сверху
    VBUF_Draw_Line(83, 9, -37, 1);//Вертикальная линия справа
    VBUF_Draw_Line(75, 46, 1, 8);//Горизонтальная линия линия снизу
    //-------------------Визуальная индикация PDE4--------------------------
    VBUF_Draw_Line(76, 28, FILT.PDE4_Value / 37, 6);
    
    //-------------------Визуализация усреднения--------------------------
    VBUF_Draw_Line(41, 44, FILT.Data_CNT / 8, 2);
    VBUF_Draw_Line(39, 17, -29, 1);//Вертикальная линия слева
    VBUF_Draw_Line(40, 18, 1, 4);//Горизонтальная линия линия сверху
    VBUF_Draw_Line(44, 17, -29, 1);//Вертикальная линия справа
    VBUF_Draw_Line(40, 46, 1, 4);//Горизонтальная линия линия снизу
    
    
    DISP_Update();
    vTaskDelay(32);
  }
}

//Задача фильтрации данных
void vFILTER_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  FILT.PDE1_Array = pvPortMalloc(sizeof(int16_t) * FILT.Data_CNT);
  FILT.PDE2_Array = pvPortMalloc(sizeof(int16_t) * FILT.Data_CNT);
  FILT.PDE3_Array = pvPortMalloc(sizeof(int16_t) * FILT.Data_CNT);
  FILT.PDE4_Array = pvPortMalloc(sizeof(int16_t) * FILT.Data_CNT);
  xSemaphoreGive(xFilterDone);
  for (;;)
  {
    for(int i = 0; i < FILT.Data_CNT; i++)
    {
      FILT.PDE1_Array[i] =  SensorVoltageStruct.PDE1_VDC;
      FILT.PDE2_Array[i] =  SensorVoltageStruct.PDE2_VDC;
      FILT.PDE3_Array[i] =  SensorVoltageStruct.PDE3_VDC;
      FILT.PDE4_Array[i] =  SensorVoltageStruct.PDE4_VDC;
      vTaskDelay(10);
    }
    FILT.PDE1_Value = filtAverage(FILT.PDE1_Array, FILT.Data_CNT);
    FILT.PDE2_Value = filtAverage(FILT.PDE2_Array, FILT.Data_CNT);
    FILT.PDE3_Value = filtAverage(FILT.PDE3_Array, FILT.Data_CNT);
    FILT.PDE4_Value = filtAverage(FILT.PDE4_Array, FILT.Data_CNT);
    xSemaphoreGive(xFilterDone);
  }
}
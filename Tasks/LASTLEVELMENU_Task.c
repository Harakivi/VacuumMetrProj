#include "main.h"

TaskHandle_t xLASTLEVELMENUHandle = NULL;
TaskHandle_t xGameDrawHandle = NULL;
TaskHandle_t xGameBtnHandle = NULL;

Config_Struct tempConfig;

BTN_Struct calibBtnArray[2] = { 0 };
extern uint8_t yBtnPosArr[];
extern uint8_t xBtnPosArr[];
extern xSemaphoreHandle xBtnPresSem;
extern TaskHandle_t xMenuHandle;
extern MENU_Struct menuStruct;
extern VDC_Struct SensorVoltageStruct;
extern FILT_Struct FILT;
extern char build[];

//size: 5x8
extern uint8_t ArrowUP[];   // Стрелка вверх
//size: 5x8
extern uint8_t ArrowDOWN[];   // Стрелка вниз

//Инвертирует кнопку относительно текущего положения в меню настроек
void invertLastLevelCurrentButton(int8_t relBtnOffset)
{
  VBUF_Invert_Line(xBtnPosArr[menuStruct.lastLevelPosition + relBtnOffset] - 1, yBtnPosArr[menuStruct.lastLevelPosition + relBtnOffset] - 1, - 9, calibBtnArray[menuStruct.lastLevelPosition + relBtnOffset].pixelCount + 1);
}

//Внутри функции можно добавлять/удалять кнопки
void lastlevelButtArrayInit(void)
{  
  calibBtnArray[BIAS_CALIB] = ButtCreate("Bias Calib");
  calibBtnArray[AMPF_CALIB] = ButtCreate("Ampf Calib");
}

void LASTLEVELMENU_Task_init(void)
{
  //Создание задачи обновления дисплея в меню настроек
  xTaskCreate(vLASTLEVELMENU_Task, "LASTLEVELMENU", LASTLEVELMENU_STACK_SIZE, NULL, LASTLEVELMENU_TASK_PRIORITY, &xLASTLEVELMENUHandle);
  //После создания задачи сразу же останавливаем её
  vTaskSuspend(xLASTLEVELMENUHandle);
}

void vLASTLEVELMENU_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  
  char string[14] = {0};
  
  lastlevelButtArrayInit();
  
  for (;;)
  {
    VBUF_Clear();
    switch(menuStruct.menuPosition + menuStruct.menuOffset)
    {
    case MENU_METER_SETS_POS:
      {
        VBUF_Write_String(2, 2, "Meter:");
        VBUF_Write_String(37, 2, FILT.Dig_CNT ? "3Digs" : "2Digs");
        VBUF_Write_String(0,39, "<Back");
        VBUF_Write_String(55,39, "Save>");
        DISP_Update();
        vTaskDelay(100);
        break;
      }
    case MENU_BRIGHT_POS:
      {
        static double battVolt;
        static uint32_t averVolt;
        static uint8_t countReads;
        averVolt += SensorVoltageStruct.BATT_VDC;
        if(countReads == 10)
        {
          averVolt /= countReads + 1;
          countReads = 0;
        }
        if(!countReads++)
        {
          battVolt = averVolt / 1000.0;
        }
        VBUF_Write_String(2, 2, "Batt:");
        sprintf(string, "%4.2f", battVolt);
        VBUF_Write_String(31,2,string);
        VBUF_Write_String(2, 14, "Bright:");
        sprintf(string, "%lu", DISPLAYBRIGHT);
        VBUF_Write_String(43,14, string);
        VBUF_Write_String(0,39, "<Back");
        VBUF_Write_String(55,39, "Save>");
        DISP_Update();
        vTaskDelay(100);
      }
      break;
    case MENU_CALIBRATE_POS:
      if(menuStruct.lastLevelOffset == CALIB_FIRST_PAGE)
      {
        VBUF_Write_String(2, 2, "Bias1:");
        sprintf(string, "%i", tempConfig.BIAS_PDE1);
        VBUF_Write_String(37,2,string);
        VBUF_Write_String(2, 14, "Bias2:");
        sprintf(string, "%i", tempConfig.BIAS_PDE2);
        VBUF_Write_String(37,14,string);
        VBUF_Write_String(2, 26, "Bias2:");
        sprintf(string, "%i", tempConfig.BIAS_PDE3);
        VBUF_Write_String(37,26,string);
        VBUF_Draw_Image(79,30,5,8,ArrowDOWN);//Стрелка вниз
      }
      if(menuStruct.lastLevelOffset == CALIB_SECOND_PAGE)
      {
        VBUF_Write_String(2, 2, "Bias4:");
        sprintf(string, "%i", tempConfig.BIAS_PDE4);
        VBUF_Write_String(37,2,string);
        VBUF_Write_String(2, 14, "Ampf1:");
        sprintf(string, "%i", tempConfig.AMPF_PDE1);
        VBUF_Write_String(37,14,string);
        VBUF_Write_String(2, 26, "Ampf2:");
        sprintf(string, "%i", tempConfig.AMPF_PDE2);
        VBUF_Write_String(37,26,string);
        VBUF_Draw_Image(79,0,5,8,ArrowUP);//Стрелка вверх
        VBUF_Draw_Image(79,30,5,8,ArrowDOWN);//Стрелка вниз
      }
      if(menuStruct.lastLevelOffset == CALIB_THIRD_PAGE)
      {
        VBUF_Write_String(2, 2, "Ampf3:");
        sprintf(string, "%i", tempConfig.AMPF_PDE3);
        VBUF_Write_String(37,2,string);
        VBUF_Write_String(2, 14, "Ampf4:");
        sprintf(string, "%i", tempConfig.AMPF_PDE4);
        VBUF_Write_String(37,14,string);
        VBUF_Write_String(2, 26, "Flag:");
        VBUF_Write_String(31, 26, "0x");
        sprintf(string, "%X", tempConfig.CalibComplete);
        VBUF_Write_String(43,26,string);
        VBUF_Draw_Image(79,0,5,8,ArrowUP);//Стрелка вверх
        VBUF_Draw_Image(79,30,5,8,ArrowDOWN);//Стрелка вниз
      }
      if(menuStruct.lastLevelOffset == CALIB_BTN_PAGE)
      {
        switch(menuStruct.lastLevelPosition)
        {
        case BIAS_CALIB:
          VBUF_Write_String(xBtnPosArr[0],yBtnPosArr[0],calibBtnArray[0].Text);
          VBUF_Write_String(xBtnPosArr[1],yBtnPosArr[1],calibBtnArray[1].Text);
          invertLastLevelCurrentButton(0);
          break;
        case AMPF_CALIB:
          VBUF_Write_String(xBtnPosArr[0],yBtnPosArr[0],calibBtnArray[0].Text);
          VBUF_Write_String(xBtnPosArr[1],yBtnPosArr[1],calibBtnArray[1].Text);
          invertLastLevelCurrentButton(0);
          break;
        }
        VBUF_Draw_Image(79,0,5,8,ArrowUP);//Стрелка вверх
      }
      VBUF_Write_String(0,39, "<Back");
      VBUF_Write_String(55,39, "Save>");
      DISP_Update();
      vTaskDelay(100);
      break;
    case MENU_GAME1_POS:
      vTaskSuspend(xMenuHandle);
      vSnake_Start();
      vTaskResume(xMenuHandle);
      vTaskDelete(xGameDrawHandle);
      vTaskDelete(xGameBtnHandle);
      xGameDrawHandle = NULL;
      xGameBtnHandle = NULL;
      menuStruct.menuDepth--;
      vTaskResume(xMenuHandle);
      xSemaphoreGive( xBtnPresSem);
      vTaskDelay(100);
      break;
    case MENU_GAME2_POS:
      vTaskSuspend(xMenuHandle);
      vTetris_Start();
      vTaskResume(xMenuHandle);
      vTaskDelete(xGameDrawHandle);
      vTaskDelete(xGameBtnHandle);
      xGameDrawHandle = NULL;
      xGameBtnHandle = NULL;
      menuStruct.menuDepth--;
      vTaskResume(xMenuHandle);
      xSemaphoreGive( xBtnPresSem);
      vTaskDelay(100);
      break;
    case MENU_ABOUT_POS:
      VBUF_Write_String(2, 2, "Build:");
      extern char BuildVersion[];
      VBUF_Write_String(2, 14, BuildVersion);
      VBUF_Write_String(2, 26, "Date:");
      extern char BuildDate[];
      VBUF_Write_String(31, 26, BuildDate);
      VBUF_Write_String(0,39, "<Back");
      DISP_Update();
      vTaskDelay(100);
      break;
    }
  }
}
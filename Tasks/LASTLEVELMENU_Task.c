#include "main.h"

TaskHandle_t xLASTLEVELMENUHandle = NULL;
TaskHandle_t xGameDrawHandle = NULL;
TaskHandle_t xGameBtnHandle = NULL;

Config_Struct tempConfig;
BTN_Struct calibBtnArray[2] = { 0 };
BTN_Struct dispSetBtnArray[3] = { 0 };

extern uint8_t yBtnPosArr[];
extern uint8_t xBtnPosArr[];
extern xSemaphoreHandle xBtnPresSem;
extern TaskHandle_t xMenuHandle;
extern MENU_Struct menuStruct;
extern VDC_Struct SensorVoltageStruct;
extern FILT_Struct FILT;
extern char build[];
extern DISP_SETS_Struct _dispSets;

//size: 5x8
extern uint8_t ArrowUP[];   // Стрелка вверх
//size: 5x8
extern uint8_t ArrowDOWN[];   // Стрелка вниз

//Инвертирует кнопку относительно текущего положения в меню настроек
void invertLastLevelCurrentButton(int8_t relBtnOffset, BTN_Struct* BtnArray)
{
  VBUF_Invert_Line(xBtnPosArr[menuStruct.lastLevelPosition + relBtnOffset] - 1, yBtnPosArr[menuStruct.lastLevelPosition + relBtnOffset] - 1, - 9, BtnArray[menuStruct.lastLevelPosition + relBtnOffset].pixelCount + 1);
}

//Инвертирует настраеваемый параметр
void invertLastLevelCurrentSetParam(int8_t relBtnOffset, BTN_Struct* BtnArray, uint8_t paramPxlCnt)
{
  VBUF_Invert_Line(xBtnPosArr[menuStruct.lastLevelPosition + relBtnOffset] - 1 + BtnArray[menuStruct.lastLevelPosition + relBtnOffset].pixelCount + 1,
                   yBtnPosArr[menuStruct.lastLevelPosition + relBtnOffset] - 1, - 9, paramPxlCnt + 1);
}

//Внутри функции можно добавлять/удалять кнопки
void lastlevelButtArrayInit(void)
{  
  calibBtnArray[BIAS_CALIB] = ButtCreate("Bias Calib");
  calibBtnArray[AMPF_CALIB] = ButtCreate("Ampf Calib");
  dispSetBtnArray[BRIGHTNESS_DISP_SET] = ButtCreate("Bright:");
  dispSetBtnArray[TIME_BRIGHT_OFF_DISP_SET] = ButtCreate("DimmerOff:");
  dispSetBtnArray[TIME_DISP_OFF_DISP_SET] = ButtCreate("DeviceOff:");
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
    case MENU_METER_SET_POS:
      {
        VBUF_Write_String(2, 2, "Meter:");
        VBUF_Write_String(37, 2, FILT.Dig_CNT ? "3Digs" : "2Digs");
        VBUF_Write_String(0,39, "<Back");
        VBUF_Write_String(55,39, "Save>");
        DISP_Update();
        vTaskDelay(100);
        break;
      }
    case MENU_DISPLAY_SET_POS:
      {
#if !defined (CHECK_BATT_VOLT)
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
        sprintf(string, "%u", DISPLAYBRIGHT);
        VBUF_Write_String(43,14, string);
#else
        VBUF_Write_String(xBtnPosArr[0],yBtnPosArr[0],dispSetBtnArray[0].Text);
        uint8_t symbCntBright = sprintf(string, "%u", DISPLAYBRIGHT_REG);
        VBUF_Write_String(dispSetBtnArray[0].pixelCount + 3,yBtnPosArr[0], string);
        VBUF_Write_String(xBtnPosArr[1],yBtnPosArr[1],dispSetBtnArray[1].Text);
        uint8_t symbCntDimmer = sprintf(string, "%1.1f", _dispSets.dimmerOff / 60000.0);
        VBUF_Write_String(dispSetBtnArray[1].pixelCount + 3,yBtnPosArr[1], string);
        VBUF_Write_String(xBtnPosArr[2],yBtnPosArr[2],dispSetBtnArray[2].Text);
        uint8_t symbCntDevice = sprintf(string, "%1.1f", _dispSets.deviceOff / 60000.0);
        VBUF_Write_String(dispSetBtnArray[2].pixelCount + 3,yBtnPosArr[2], string);
        switch(menuStruct.lastLevelPosition)
        {
        case BRIGHTNESS_DISP_SET:
          {
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              invertLastLevelCurrentButton(0, dispSetBtnArray);
              break;
            case SET_LEVEL:
              invertLastLevelCurrentSetParam(0, dispSetBtnArray, symbCntBright * 6);
              break;
            }
          }
          break;
        case TIME_BRIGHT_OFF_DISP_SET:
          {
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              invertLastLevelCurrentButton(0, dispSetBtnArray);
              break;
            case SET_LEVEL:
              invertLastLevelCurrentSetParam(0, dispSetBtnArray, symbCntDimmer * 6);
              break;
            }
          }
          break;
        case TIME_DISP_OFF_DISP_SET:
          {
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              invertLastLevelCurrentButton(0, dispSetBtnArray);
              break;
            case SET_LEVEL:
              invertLastLevelCurrentSetParam(0, dispSetBtnArray, symbCntDevice * 6);
              break;
            }
          }
          break;
        default:
          menuStruct.lastLevelPosition = BRIGHTNESS_DISP_SET;
          break;
        }
#endif  
        switch (menuStruct.lastLevelDepth)
        {
        case DISP_LEVEL:
          VBUF_Write_String(0,39, "<Back");
          VBUF_Write_String(55,39, "Save>");
          break;
        case SET_LEVEL:
          VBUF_Draw_Image(0,39,5,8,ArrowUP);//Стрелка вверх
          VBUF_Draw_Image(79,39,5,8,ArrowDOWN);//Стрелка вниз
          VBUF_Write_String(6,39, "Incr");
          VBUF_Write_String(54,39, "Decr");
          break;
        }
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
      else if(menuStruct.lastLevelOffset == CALIB_SECOND_PAGE)
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
      else if(menuStruct.lastLevelOffset == CALIB_THIRD_PAGE)
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
      else if(menuStruct.lastLevelOffset == CALIB_BTN_PAGE)
      {
        switch(menuStruct.lastLevelPosition)
        {
        case BIAS_CALIB:
          VBUF_Write_String(xBtnPosArr[0],yBtnPosArr[0],calibBtnArray[0].Text);
          VBUF_Write_String(xBtnPosArr[1],yBtnPosArr[1],calibBtnArray[1].Text);
          if(menuStruct.lastLevelDepth == DISP_LEVEL)
          {
            invertLastLevelCurrentButton(0, calibBtnArray);
          }
          break;
        case AMPF_CALIB:
          VBUF_Write_String(xBtnPosArr[0],yBtnPosArr[0],calibBtnArray[0].Text);
          VBUF_Write_String(xBtnPosArr[1],yBtnPosArr[1],calibBtnArray[1].Text);
          if(menuStruct.lastLevelDepth == DISP_LEVEL)
          {
            invertLastLevelCurrentButton(0, calibBtnArray);
          }
          break;
        default:
          menuStruct.lastLevelPosition = BIAS_CALIB;
          break;
        }
        VBUF_Draw_Image(79,0,5,8,ArrowUP);//Стрелка вверх
      }
      if(menuStruct.lastLevelDepth == DISP_LEVEL)
      {
        VBUF_Write_String(0,39, "<Back");
        VBUF_Write_String(55,39, "Save>");
      }
      else
      {
        VBUF_Write_String(0,39, "Calibrating...");
      }
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
      char build_number[] = {build[14], build[15], build[16], build[17], 0};
      VBUF_Write_String(37, 2, build_number);
      VBUF_Write_String(2, 14, "Date:");
      char build_date[] = {build[0], build[1], '.', build[3], build[4], '.',
      build[8], build[9], 0};
      VBUF_Write_String(31, 14, build_date);
      VBUF_Write_String(0,39, "<Back");
      DISP_Update();
      vTaskDelay(100);
      break;
    }
  }
}
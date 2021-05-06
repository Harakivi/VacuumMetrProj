#include "main.h"
#include <stdio.h>

MENU_Struct menuStruct = { 0 };
BTN_Struct btnArray[7] = { 0 };

extern FILT_Struct FILT;
extern VDC_Struct SensorVoltageStruct;
extern Config_Struct* Config;
extern Config_Struct tempConfig;

TaskHandle_t xMenuHandle = NULL;
extern TaskHandle_t xMeterHandle;
extern TaskHandle_t xLASTLEVELMENUHandle;
extern xSemaphoreHandle xBtnPresSem;
extern QueueHandle_t xBtnPresQueue;

extern uint8_t UP_BTN;
extern uint8_t ENTER_BTN;
extern uint8_t DOWN_BTN;
extern uint8_t LEFT_BTN;
extern uint8_t RIGHT_BTN;

uint8_t yBtnPosArr[] = {MENU_Y_BTN_POS1, MENU_Y_BTN_POS2, MENU_Y_BTN_POS3, MENU_Y_BTN_POS4};
uint8_t xBtnPosArr[] = {MENU_X_BTN_POS1, MENU_X_BTN_POS2, MENU_X_BTN_POS3, MENU_X_BTN_POS4};

//size: 5x8
extern uint8_t ArrowUP[];   // Стрелка вверх
//size: 5x8
extern uint8_t ArrowDOWN[];   // Стрелка вниз

//Функция инициализации кнопки
BTN_Struct ButtCreate(char *characters)
{
  BTN_Struct _button = { 0 };
  
  uint8_t textPos = 0;
  
  while(*characters)
  {
    _button.Text[textPos++] = *characters++;
    _button.pixelCount += 6;
  }
  return _button;
}

//Инвертирует кнопку относительно текущего положения в меню
void invertMenuCurrentButton(int8_t relBtnOffset)
{
  VBUF_Invert_Line(xBtnPosArr[menuStruct.menuPosition + relBtnOffset] - 1, yBtnPosArr[menuStruct.menuPosition + relBtnOffset] - 1, - 9, btnArray[menuStruct.menuPosition + menuStruct.menuOffset + relBtnOffset].pixelCount + 1);
}

//Внутри функции можно добавлять/удалять кнопки
void menuButtArrayInit(void)
{  
  btnArray[MENU_METER_POS] = ButtCreate("Meter");
  btnArray[MENU_BRIGHT_POS] = ButtCreate("Brightness");
  btnArray[MENU_CALIBRATE_POS] = ButtCreate("Calibrate");
  btnArray[MENU_GAME1_POS] = ButtCreate("Snake");
  btnArray[MENU_GAME2_POS] = ButtCreate("Tetris");
  btnArray[MENU_TURNOFF_POS] = ButtCreate("Turn off");
}

void Menu_Task_init(void)
{  
  //Инициализация массива кнопок
  menuButtArrayInit();
  //Создание задачи отрисовки меню
  xTaskCreate(vMENU_Task, "Menu", MENU_STACK_SIZE, NULL, MENU_TASK_PRIORITY, &xMenuHandle);
}



void vMENU_Task(void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  uint8_t btn = 0;
  for(;;)
  {
    xSemaphoreTake( xBtnPresSem, portMAX_DELAY );
    //Вытягиваем кнопку из очереди
    if(xQueueReceive( xBtnPresQueue, &btn, ( TickType_t ) 10 ) == pdPASS)
    {
      //Обработка нажатой кнопки в меню
      switch(btn)
      {
      case UP_BTN_CASE:
        switch(menuStruct.menuDepth)
        {
        case TOP_LEVEL:
          if(FILT.SizeFilt <= 240)
          {
            FILT.SizeFilt += 10;
          } 
          break;
        case MENU_LEVEL:
        if(menuStruct.menuPosition != BTN_FIRST_POS)
        {
          menuStruct.menuPosition--;
        }else if( menuStruct.menuOffset != BTN_MIN_OFFSET)
        {
          menuStruct.menuOffset--;
        }    
        break;
      case LAST_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_BRIGHT_POS:
          if(BRIGHT <= 90)
          {
            BRIGHT += 10;
          }
          break;
        case MENU_CALIBRATE_POS:
          if(menuStruct.lastLevelOffset > CALIB_MIN_OFFSET && menuStruct.lastLevelPosition == CALIB_MIN_POS)
          {
            menuStruct.lastLevelOffset--;
          }
          else if(menuStruct.lastLevelOffset == CALIB_MAX_OFFSET && menuStruct.lastLevelPosition > CALIB_MIN_POS)
          {
            menuStruct.lastLevelPosition--;
          }
          break;
        }
        break;
      }
      break;
    case ENTER_BTN_CASE:
      switch(menuStruct.menuDepth)
      {
      case TOP_LEVEL:
        menuStruct.menuPosition = 0;
        menuStruct.menuOffset = 0;
        menuStruct.lastLevelPosition = 0;
        menuStruct.lastLevelOffset = 0;
        menuStruct.menuDepth++;
        break;
      case MENU_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_METER_POS:
          menuStruct.menuPosition = 0;
          menuStruct.menuOffset = 0;
          menuStruct.lastLevelPosition = 0;
          menuStruct.lastLevelOffset = 0;
          menuStruct.menuDepth--;
          break;
        case MENU_BRIGHT_POS:
          menuStruct.menuDepth++; 
          break;
        case MENU_GAME1_POS:
          menuStruct.menuDepth++;
          break;
        case MENU_GAME2_POS:
          menuStruct.menuDepth++;
          break;
        case MENU_CALIBRATE_POS:
          tempConfig = *Config;
          menuStruct.menuDepth++;
          break;
        case MENU_TURNOFF_POS:
          DISP_Clear();
          enterStandBy();
          break;   
        }   
        break;
      case LAST_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_CALIBRATE_POS:
          if(menuStruct.lastLevelOffset == CALIB_BTN_PAGE && menuStruct.lastLevelPosition == BIAS_CALIB)
          {
            tempConfig = biasCalibrate();
          }
          break;
        }
        break;
      }
      break;
    case DOWN_BTN_CASE:
      switch(menuStruct.menuDepth)
      {
      case TOP_LEVEL:
        if(FILT.SizeFilt >= 10)
          {
            FILT.SizeFilt -= 10;
          } 
        break;
      case MENU_LEVEL:
        if(menuStruct.menuPosition != BTN_LAST_POS)
        {
          menuStruct.menuPosition++;
        }else if( menuStruct.menuOffset != BTN_MAX_OFFSET)
        {
          menuStruct.menuOffset++;
        }
        break;
      case LAST_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_BRIGHT_POS:
          if(BRIGHT >= 10)
          {
            BRIGHT -= 10;
          }
          break;
        case MENU_CALIBRATE_POS:
          if(menuStruct.lastLevelOffset < CALIB_MAX_OFFSET && menuStruct.lastLevelPosition == CALIB_MIN_POS)
          {
            menuStruct.lastLevelOffset++;
          }
          else if(menuStruct.lastLevelOffset == CALIB_MAX_OFFSET && menuStruct.lastLevelPosition < CALIB_MAX_POS)
          {
            menuStruct.lastLevelPosition++;
          }
          break;
        }    
        break;
      }
      break;
    case LEFT_BTN_CASE:
      switch(menuStruct.menuDepth)
      {
      case LAST_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_BRIGHT_POS:
          if(Config->Bright > 100)
          {
            BRIGHT = 100;
          }else{ BRIGHT = Config->Bright;}
          menuStruct.menuDepth--;
          menuVBUFDraw();    
          break;    
        case MENU_CALIBRATE_POS:
          menuStruct.lastLevelPosition = 0;
          menuStruct.lastLevelOffset = 0;
          menuStruct.menuDepth--;
          break;
        }
      }
      break;
    case RIGHT_BTN_CASE:
      switch(menuStruct.menuDepth)
      {
      case LAST_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_BRIGHT_POS:
          if(BRIGHT > 100)
          {
            SaveBright(100);
          }else{SaveBright(BRIGHT);}
          menuStruct.menuDepth--;   
          break;    
        case MENU_CALIBRATE_POS:
          SaveCongig(&tempConfig);
          menuStruct.lastLevelPosition = 0;
          menuStruct.lastLevelOffset = 0;
          menuStruct.menuDepth--;
          break;
        }
      }
      break;
    }
    }
    
    menuVBUFDraw();
  }
}

void menuVBUFDraw()
{
  switch(menuStruct.menuDepth)
  {
  case TOP_LEVEL: 
    VBUF_Clear();
    vTaskResume(xMeterHandle);
    break;
  case MENU_LEVEL:
    vTaskSuspend(xMeterHandle);
    vTaskSuspend(xLASTLEVELMENUHandle);
    VBUF_Clear();
    for(int i = menuStruct.menuOffset; i <= BTN_LAST_POS + menuStruct.menuOffset; i++)
    {
      VBUF_Write_String(xBtnPosArr[i - menuStruct.menuOffset],yBtnPosArr[i - menuStruct.menuOffset],btnArray[i].Text);
    }
    if(menuStruct.menuOffset != BTN_MIN_OFFSET)
    {
      VBUF_Draw_Image(79,0,5,8,ArrowUP);//Стрелка вверх
    }
    if(menuStruct.menuOffset != BTN_MAX_OFFSET)
    {
      VBUF_Draw_Image(79,40,5,8,ArrowDOWN);//Стрелка вниз
    }
    invertMenuCurrentButton(0);
    DISP_Update();
    break;
  case LAST_LEVEL:
    vTaskResume(xLASTLEVELMENUHandle);
    break;
  }
}
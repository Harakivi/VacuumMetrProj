#include "main.h"
#include <stdio.h>

MENU_Struct menuStruct = { 0 };
BTN_Struct btnArray[7] = { 0 };

extern VDC_Struct SensorVoltageStruct;
extern Config_Struct* Config;
extern Config_Struct tempConfig;

extern TaskHandle_t xMeterHandle;
extern TaskHandle_t xLASTLEVELMENUHandle;
extern xSemaphoreHandle xBtnPresSem;
extern QueueHandle_t xBtnPresQueue;

uint8_t yBtnPosArr[] = {MENU_Y_BTN_POS1, MENU_Y_BTN_POS2, MENU_Y_BTN_POS3, MENU_Y_BTN_POS4};
uint8_t xBtnPosArr[] = {MENU_X_BTN_POS1, MENU_X_BTN_POS2, MENU_X_BTN_POS3, MENU_X_BTN_POS4};

//size: 5x8
extern uint8_t ArrowUP[];   // Стрелка вверх
//size: 5x8
extern uint8_t ArrowDOWN[];   // Стрелка вниз

//Функция инициализации кнопки
BTN_Struct menuButtInit(char *characters)
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
void invertCurrentButton(int8_t relBtnOffset)
{
  VBUF_Invert_Line(xBtnPosArr[menuStruct.menuPosition + relBtnOffset] - 1, yBtnPosArr[menuStruct.menuPosition + relBtnOffset] - 1, - 9, btnArray[menuStruct.menuPosition + menuStruct.menuOffset + relBtnOffset].pixelCount + 1);
}

//Внутри функции можно добавлять/удалять кнопки
void menuButtArrayInit(void)
{  
  btnArray[MENU_METER_POS] = menuButtInit("Meter");
  btnArray[MENU_BRIGHT_POS] = menuButtInit("Brightness");
  btnArray[MENU_CALIBRATE_POS] = menuButtInit("Calibrate");
  btnArray[MENU_GAME1_POS] = menuButtInit("Game1");
  btnArray[MENU_GAME2_POS] = menuButtInit("Game2");
  btnArray[MENU_TURNOFF_POS] = menuButtInit("Turn off");
}

void Menu_Task_init(void)
{  
  //Инициализация массива кнопок
  menuButtArrayInit();
  //Создание задачи отрисовки меню
  xTaskCreate(vMENU_Task, "Menu", MENU_STACK_SIZE, NULL, MENU_TASK_PRIORITY, NULL);
}



void vMENU_Task(void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  uint8_t btn = 0;
  for(;;)
  {
    xSemaphoreTake( xBtnPresSem, portMAX_DELAY );
    //Обработка нажатой кнопки в меню
    xQueueReceive( xBtnPresQueue, &btn, ( TickType_t ) 10 );
    switch(btn)
    {
    case UP_BTN:
      switch(menuStruct.menuDepth)
      {
      case TOP_LEVEL:
        break;
      case MENU_LEVEL:
        if(menuStruct.menuPosition != BTN_FIRST_POS)
        {
          menuStruct.menuPosition--;
          invertCurrentButton(1);
          invertCurrentButton(0);
          DISP_Update();
        }else if( menuStruct.menuOffset != BTN_MIN_OFFSET)
        {
          menuStruct.menuOffset--;
          menuVBUFDraw();
        }    
        break;
      case SETTINGS_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_BRIGHT_POS:
          if(TIM3->CCR3 < 4000)
          {
            TIM3->CCR3 += 500;
          }
          break;
        case MENU_CALIBRATE_POS:
          tempConfig = biasCalibrate();
          menuVBUFDraw();
          break;
        }
        break;
      }
      break;
    case ENTER_BTN:
      switch(menuStruct.menuDepth)
      {
      case TOP_LEVEL:
        menuStruct.menuPosition = 0;
        menuStruct.menuOffset = 0;
        menuStruct.menuDepth++;
        menuVBUFDraw();
        break;
      case MENU_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_METER_POS:
          menuStruct.menuPosition = 0;
          menuStruct.menuDepth--;
          menuVBUFDraw();
          break;
        case MENU_BRIGHT_POS:
          menuStruct.menuDepth++;
          menuVBUFDraw();
          break;     
        case MENU_CALIBRATE_POS:
          tempConfig = *Config;
          menuStruct.menuDepth++;
          menuVBUFDraw();
          break;
        case MENU_TURNOFF_POS:
          DISP_Clear();
          enterStandBy();
          break;   
        }   
        break;
      case SETTINGS_LEVEL:      
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_BRIGHT_POS:
          menuStruct.menuPosition = 0;
          menuStruct.menuDepth--;
          menuVBUFDraw();    
          break;    
        case MENU_CALIBRATE_POS:
          SaveCongig(&tempConfig);
          menuStruct.menuDepth--;
          menuVBUFDraw();
          break;
        }   
      }
      break;
    case DOWN_BTN:
      switch(menuStruct.menuDepth)
      {
      case TOP_LEVEL:
        break;
      case MENU_LEVEL:
        if(menuStruct.menuPosition != BTN_LAST_POS)
        {
          menuStruct.menuPosition++;
          invertCurrentButton(-1);
          invertCurrentButton(0);
          DISP_Update();
        }else if( menuStruct.menuOffset != BTN_MAX_OFFSET)
        {
          menuStruct.menuOffset++;
          menuVBUFDraw();
        }
        break;
      case SETTINGS_LEVEL:
        switch(menuStruct.menuPosition + menuStruct.menuOffset)
        {
        case MENU_BRIGHT_POS:
          if(TIM3->CCR3 > 0)
          {
            TIM3->CCR3 -= 500;
          }
          break;
        }    
        break;
      }
      break;
    case LEFT_BTN:
      break;
    case RIGHT_BTN:
      break;
    }
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
    invertCurrentButton(0);
    DISP_Update();
    break;
  case SETTINGS_LEVEL:
    vTaskResume(xLASTLEVELMENUHandle);
    break;
  }
}
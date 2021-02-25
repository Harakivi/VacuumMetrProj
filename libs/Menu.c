#include "main.h"
#include <stdio.h>

MENU_Struct menuStruct = { 0 };
BTN_Struct btnArray[7] = { 0 };

extern VDC_Struct SensorVoltageStruct;
extern Config_Struct* Config;
Config_Struct tempConfig;

TaskHandle_t xSettingsHandle = NULL;
extern TaskHandle_t  xMeterHandle ;

uint8_t yBtnPosArr[] = {MENU_Y_BTN_POS1, MENU_Y_BTN_POS2, MENU_Y_BTN_POS3, MENU_Y_BTN_POS4};
uint8_t xBtnPosArr[] = {MENU_X_BTN_POS1, MENU_X_BTN_POS2, MENU_X_BTN_POS3, MENU_X_BTN_POS4};

extern TaskHandle_t xMeterHandle;
TaskHandle_t xSettingsHandle;

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
  btnArray[0] = menuButtInit("Meter");
  btnArray[1] = menuButtInit("Brightness");
  btnArray[2] = menuButtInit("Calibrate");
  btnArray[3] = menuButtInit("Return");
  btnArray[4] = menuButtInit("Turn off");
}


void menuBtnProc(uint8_t btn)
{
  //Обработка нажатой кнопки в меню
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
      case MENU_BRIGHT_POS:
        menuStruct.menuDepth++;
        menuVBUFDraw();
        break;     
      case MENU_CALIBRATE_POS:
        tempConfig = *Config;
        menuStruct.menuDepth++;
        menuVBUFDraw();
        break;
      case MENU_RETURN_POS:
        menuStruct.menuPosition = 0;
        menuStruct.menuDepth--;
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
    vTaskSuspend(xSettingsHandle);
    VBUF_Clear();
    for(int i = menuStruct.menuOffset; i <= BTN_LAST_POS + menuStruct.menuOffset; i++)
    {
      VBUF_Write_String(xBtnPosArr[i - menuStruct.menuOffset],yBtnPosArr[i - menuStruct.menuOffset],btnArray[i].Text);
    }
    if(menuStruct.menuOffset != BTN_MIN_OFFSET)
    {
      VBUF_Write_String(70,2,"v");
    }
    invertCurrentButton(0);
    DISP_Update();
    break;
  case SETTINGS_LEVEL:
    vTaskResume(xSettingsHandle);
    break;
  }
}

void MenuDISP_Task_init(void)
{
  //Инициализация массива кнопок
  menuButtArrayInit();
  //Создание задачи обновления дисплея в меню настроек
  xTaskCreate(vSettingsRefrash_Task, "SettingsRefrash", BUTTONS_STACK_SIZE, NULL, BUTTONS_TASK_PRIORITY, &xSettingsHandle);
  //После создания задачи сразу же останавливаем её
  vTaskSuspend(xSettingsHandle);
}

void vSettingsRefrash_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  
  char string[14] = {0};
  
  for (;;)
  {
    VBUF_Clear();
    switch(menuStruct.menuPosition + menuStruct.menuOffset)
    {
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
        sprintf(string, "%u", TIM3->CCR3);
        VBUF_Write_String(43,14, string);
        break;
      }
    case MENU_CALIBRATE_POS:
      sprintf(string, "%i", tempConfig.BIAS_PDE1);
      VBUF_Write_String(27,11,string);
      sprintf(string, "%i", tempConfig.BIAS_PDE2);
      VBUF_Write_String(27,20,string);
      sprintf(string, "%i", tempConfig.BIAS_PDE3);
      VBUF_Write_String(27,29,string);
      sprintf(string, "%i", tempConfig.BIAS_PDE4);
      VBUF_Write_String(27,38,string);
      break;
    }
    DISP_Update();
    vTaskDelay(100);
  }
}
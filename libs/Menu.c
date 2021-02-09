#include "main.h"
#include <stdio.h>

MENU_Struct menuStruct;

uint8_t yBtnPosArr[] = {MENU_Y_BTN_POS1, MENU_Y_BTN_POS2, MENU_Y_BTN_POS3, MENU_Y_BTN_POS4};
uint8_t xBtnPosArr[] = {MENU_X_BTN_POS1, MENU_X_BTN_POS2, MENU_X_BTN_POS3, MENU_X_BTN_POS4};
uint8_t xBtnLenArr[] = {MENU_X_BTN_LEN1, MENU_X_BTN_LEN2, MENU_X_BTN_LEN3, MENU_X_BTN_LEN4};

extern TaskHandle_t xMeterHandle;

void menuBtnProc(uint8_t btn)
{
  char string[84] = {0};
  //Обработка нажатой кнопки в меню
  switch(btn)
  {
  case UP_BTN:
    switch(menuStruct.menuDepth)
    {
    case TOP_LEVEL:
      break;
    case MENU_LEVEL:
      if(menuStruct.menuPosition != MENU_FIRST_POS)
      {
        menuStruct.menuPosition--;
        VBUF_Invert_Line(xBtnPosArr[menuStruct.menuPosition + 1] - 1, yBtnPosArr[menuStruct.menuPosition + 1] - 1, 9, xBtnLenArr[menuStruct.menuPosition + 1] + 1);
        VBUF_Invert_Line(xBtnPosArr[menuStruct.menuPosition] - 1, yBtnPosArr[menuStruct.menuPosition] - 1, 9, xBtnLenArr[menuStruct.menuPosition] + 1);
        DISP_Update();
      }    
      break;
    case SETTINGS_LEVEL:
      switch(menuStruct.menuPosition)
      {
        case MENU_BRIGHT_POS:
          if(TIM3->CCR3 < 4000)
          {
            TIM3->CCR3 += 500;
          }
          sprintf(string, "%u", TIM3->CCR3);
          VBUF_Write_String(27,11,string);
          DISP_Update();
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
      menuStruct.menuDepth++;
      menuVBUFDraw();
      break;
    case MENU_LEVEL:
      switch(menuStruct.menuPosition)
      {
        case MENU_BRIGHT_POS:
          menuStruct.menuDepth++;
          menuVBUFDraw();
          break;
      case MENU_LAST_POS:
          menuStruct.menuPosition = 0;
          menuStruct.menuDepth--;
          menuVBUFDraw();
          break;
      }   
      break;
    case SETTINGS_LEVEL:
      menuStruct.menuPosition = 0;
      menuStruct.menuDepth--;
      menuVBUFDraw();    
      break;
    }
    break;
  case DOWN_BTN:
    switch(menuStruct.menuDepth)
    {
    case TOP_LEVEL:
      break;
    case MENU_LEVEL:
      if(menuStruct.menuPosition != MENU_LAST_POS)
      {
        menuStruct.menuPosition++;
        VBUF_Invert_Line(xBtnPosArr[menuStruct.menuPosition - 1] - 1, yBtnPosArr[menuStruct.menuPosition - 1] - 1, 9, xBtnLenArr[menuStruct.menuPosition - 1] + 1);
        VBUF_Invert_Line(xBtnPosArr[menuStruct.menuPosition] - 1, yBtnPosArr[menuStruct.menuPosition] - 1, 9, xBtnLenArr[menuStruct.menuPosition] + 1);
        DISP_Update();
      }   
      break;
    case SETTINGS_LEVEL:
      switch(menuStruct.menuPosition)
      {
        case MENU_BRIGHT_POS:
          if(TIM3->CCR3 > 0)
          {
            TIM3->CCR3 -= 500;
          }
          sprintf(string, "%u", TIM3->CCR3);
          VBUF_Write_String(27,11,string);
          DISP_Update();
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
  char string[84] = {0};
  switch(menuStruct.menuDepth)
  {
  case TOP_LEVEL:
    VBUF_Clear();
    VBUF_Write_String(27,2,"Meter");
    DISP_Update();
    vTaskResume(xMeterHandle);
    break;
  case MENU_LEVEL:
    vTaskSuspend(xMeterHandle);
    VBUF_Clear();
    VBUF_Write_String(xBtnPosArr[0],yBtnPosArr[0],"Meter");
    VBUF_Write_String(xBtnPosArr[1],yBtnPosArr[1],"Brightness");
    VBUF_Write_String(xBtnPosArr[2],yBtnPosArr[2],"Calibrate");
    VBUF_Write_String(xBtnPosArr[3] ,yBtnPosArr[3],"Return");
    VBUF_Invert_Line(xBtnPosArr[menuStruct.menuPosition] - 1, yBtnPosArr[menuStruct.menuPosition] - 1, 9, xBtnLenArr[menuStruct.menuPosition] + 1);
    DISP_Update();
    break;
  case SETTINGS_LEVEL:
    VBUF_Clear();
    
    switch(menuStruct.menuPosition)
    {
    case MENU_BRIGHT_POS:
      sprintf(string, "%u", TIM3->CCR3);
      VBUF_Write_String(27,11,string);
      break;
    }
    DISP_Update();
    break;
  }
}
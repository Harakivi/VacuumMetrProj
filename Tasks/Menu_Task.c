#include "main.h"
#include <stdio.h>

MENU_Struct menuStruct = { 0 };
BTN_Struct btnArray[MENU_BTN_CNT] = { 0 };

DISP_SETS_Struct _dispSets;

extern FILT_Struct FILT;
extern SCOPE_Struct SCOPE;
extern VDC_Struct SensorVoltageStruct;
extern Config_Struct* Config;
extern Config_Struct tempConfig;
extern VoltagesOnBatteryEnum VoltagesOnBattery;

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
//size: 10x8
extern uint8_t fullBATT [];// Символ полной батареи
//size: 10x8
extern uint8_t midBATT []; // Символ средней батареи
//size: 10x8
extern uint8_t lowBATT []; // Символ низкой батареи
//size: 10x8
extern uint8_t emptyBATT []; // Символ разряженной батареи

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
  btnArray[MENU_METER_SET_POS] = ButtCreate("Meter Set");
  btnArray[MENU_SCOPE_POS] = ButtCreate("Scope");
  btnArray[MENU_DISPLAY_SET_POS] = ButtCreate("Display Set");
  btnArray[MENU_CALIBRATE_POS] = ButtCreate("Calibrate");
  btnArray[MENU_GAME1_POS] = ButtCreate("Snake");
  btnArray[MENU_GAME2_POS] = ButtCreate("Tetris");
  btnArray[MENU_ABOUT_POS] = ButtCreate("About");
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
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_METER_POS:
            if(FILT.Data_CNT < MAX_FILTER_DATA_SIZE + 1)
            {
              METER_Task_Deinit();
              
              FILT.Data_CNT += FILTER_DATA_INCR_SIZE;
              
              METER_Task_init();
            }
            break;
          case MENU_SCOPE_POS:
            SCOPE.Lim_Div++;
            break;
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
          case MENU_DISPLAY_SET_POS:
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              if(menuStruct.lastLevelOffset == DISP_SET_MIN_OFFSET && menuStruct.lastLevelPosition > DISP_SET_MIN_POS)
              {
                menuStruct.lastLevelPosition--;
              }
              break;
            case SET_LEVEL:
              switch (menuStruct.lastLevelPosition)
              {
              case BRIGHTNESS_DISP_SET:
                if(DISPLAYBRIGHT_REG <= 90)
                {
                  DISPLAYBRIGHT_REG += 10;
                }
                break;
              case TIME_BRIGHT_OFF_DISP_SET:
                switch(_dispSets.dimmerOff)
                {
                case 1200000:
                  _dispSets.dimmerOff = 1800000;
                  break;
                case 900000:
                  _dispSets.dimmerOff = 1200000;
                  break;
                case 600000:
                  _dispSets.dimmerOff = 900000;
                  break;
                case 300000:
                  _dispSets.dimmerOff = 600000;
                  break;
                case 150000:
                  _dispSets.dimmerOff = 300000;
                  break;
                case 60000:
                  _dispSets.dimmerOff = 150000;
                  break;
                }
                break;
              case TIME_DISP_OFF_DISP_SET:
                switch(_dispSets.deviceOff)
                {
                case 1200000:
                  _dispSets.deviceOff = 1800000;
                  break;
                case 900000:
                  _dispSets.deviceOff = 1200000;
                  break;
                case 600000:
                  _dispSets.deviceOff = 900000;
                  break;
                case 300000:
                  _dispSets.deviceOff = 600000;
                  break;
                case 150000:
                  _dispSets.deviceOff = 300000;
                  break;
                case 60000:
                  _dispSets.deviceOff = 150000;
                  break;
                }
                break;
              }
              break;
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
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_METER_POS:
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.menuDepth++;
            METER_Task_Deinit();
            break;
          case MENU_SCOPE_POS:
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.menuDepth++;
            SCOPE_Task_Deinit();
            break;
          }
          break;
        case MENU_LEVEL:
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_METER_POS:
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.menuDepth--;
            METER_Task_init();
            break;
          case MENU_METER_SET_POS:
            menuStruct.menuDepth++; 
            break;
          case MENU_SCOPE_POS:
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.menuDepth--;
            SCOPE_Task_init();
            break;
          case MENU_DISPLAY_SET_POS:
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.lastLevelDepth = 0;
            _dispSets.deviceOff = Config->deviceOff;
            _dispSets.dimmerOff = Config->dimmerOff;
            menuStruct.menuDepth++; 
            break;
          case MENU_GAME1_POS:
            menuStruct.menuDepth++;
            break;
          case MENU_GAME2_POS:
            menuStruct.menuDepth++;
            break;
          case MENU_CALIBRATE_POS:
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.lastLevelDepth = 0;
            tempConfig = *Config;
            menuStruct.menuDepth++;
            break;
          case MENU_ABOUT_POS:
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
              menuStruct.lastLevelDepth++;
              tempConfig = biasCalibrate();
              menuStruct.lastLevelDepth--;
            }
            if(menuStruct.lastLevelOffset == CALIB_BTN_PAGE && menuStruct.lastLevelPosition == AMPF_CALIB)
            {
              menuStruct.lastLevelDepth++;
              tempConfig = ampfCalibrate();
              menuStruct.lastLevelDepth--;
            }
            break;
          case MENU_DISPLAY_SET_POS:
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              menuStruct.lastLevelDepth++;
              break;
            case SET_LEVEL:
              menuStruct.lastLevelDepth--;
              break;
            }
            break;
          case MENU_METER_SET_POS:
            FILT.Dig_CNT == THREE_DIGITS ? (FILT.Dig_CNT = TWO_DIGITS) : (FILT.Dig_CNT = THREE_DIGITS);
            break;
          }
          break;
        }
        break;
      case DOWN_BTN_CASE:
        switch(menuStruct.menuDepth)
        {
        case TOP_LEVEL:
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_METER_POS:
            if(FILT.Data_CNT >= FILTER_DATA_INCR_SIZE + 1)
            {
              METER_Task_Deinit();
              
              FILT.Data_CNT -= FILTER_DATA_INCR_SIZE;
              
              METER_Task_init();
            }
            break;
          case MENU_SCOPE_POS:
            SCOPE.Lim_Div--;
            break;
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
          case MENU_DISPLAY_SET_POS:
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              if(menuStruct.lastLevelOffset == DISP_SET_MAX_OFFSET && menuStruct.lastLevelPosition < DISP_SET_MAX_POS)
              {
                menuStruct.lastLevelPosition++;
              }
              break;
            case SET_LEVEL:
              switch (menuStruct.lastLevelPosition)
              {
              case BRIGHTNESS_DISP_SET:
                if(DISPLAYBRIGHT_REG >= 10)
                {
                  DISPLAYBRIGHT_REG -= 10;
                }
                break;
              case TIME_BRIGHT_OFF_DISP_SET:
                switch(_dispSets.dimmerOff)
                {
                case 1800000:
                  _dispSets.dimmerOff = 1200000;
                  break;
                case 1200000:
                  _dispSets.dimmerOff = 900000;
                  break;
                case 900000:
                  _dispSets.dimmerOff = 600000;
                  break;
                case 600000:
                  _dispSets.dimmerOff = 300000;
                  break;
                case 300000:
                  _dispSets.dimmerOff = 150000;
                  break;
                case 150000:
                  _dispSets.dimmerOff = 60000;
                  break;
                }
                break;
              case TIME_DISP_OFF_DISP_SET:
                switch(_dispSets.deviceOff)
                {
                case 1800000:
                  _dispSets.deviceOff = 1200000;
                  break;
                case 1200000:
                  _dispSets.deviceOff = 900000;
                  break;
                case 900000:
                  _dispSets.deviceOff = 600000;
                  break;
                case 600000:
                  _dispSets.deviceOff = 300000;
                  break;
                case 300000:
                  _dispSets.deviceOff = 150000;
                  break;
                case 150000:
                  _dispSets.deviceOff = 60000;
                  break;
                }
                break;
              }
              break;
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
        case TOP_LEVEL:
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_METER_POS:
            break;
          case MENU_SCOPE_POS:
            SCOPE.Time_Delay--;
            break;
          }
          break;
        case LAST_LEVEL:
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_DISPLAY_SET_POS:
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              if(Config->Bright > 100)
              {
                DISPLAYBRIGHT_REG = 100;
              }else{ DISPLAYBRIGHT_REG = Config->Bright;}
              menuStruct.lastLevelPosition = 0;
              menuStruct.lastLevelOffset = 0;
              menuStruct.menuDepth--;
              break;
            case SET_LEVEL:
              break;
            }
            break;    
          case MENU_CALIBRATE_POS:
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.menuDepth--;
            break;
          case MENU_METER_SET_POS:
            menuStruct.menuDepth--;
            break;
          case MENU_ABOUT_POS:
            menuStruct.menuDepth--;
            break;
          }
        }
        break;
      case RIGHT_BTN_CASE:
        switch(menuStruct.menuDepth)
        {
        case TOP_LEVEL:
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_METER_POS:
            break;
          case MENU_SCOPE_POS:
            SCOPE.Time_Delay++;
            break;
          }
          break;
        case LAST_LEVEL:
          switch(menuStruct.menuPosition + menuStruct.menuOffset)
          {
          case MENU_DISPLAY_SET_POS:
            
            switch (menuStruct.lastLevelDepth)
            {
            case DISP_LEVEL:
              {
                _dispSets.bright = DISPLAYBRIGHT_REG;
                SaveDispSets(&_dispSets);
                menuStruct.menuDepth--;
              }
              break;
            case SET_LEVEL:
              break;
            }
            break;    
          case MENU_CALIBRATE_POS:
            SaveCongig(&tempConfig);
            menuStruct.lastLevelPosition = 0;
            menuStruct.lastLevelOffset = 0;
            menuStruct.menuDepth--;
            break;
          case MENU_METER_SET_POS:
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
    //vTaskResume(xMeterHandle);
    break;
  case MENU_LEVEL:
    //vTaskSuspend(xMeterHandle);
    
    vTaskSuspend(xLASTLEVELMENUHandle);
    VBUF_Clear();
    for(int i = menuStruct.menuOffset; i <= BTN_LAST_POS + menuStruct.menuOffset; i++)
    {
      VBUF_Write_String(xBtnPosArr[i - menuStruct.menuOffset],yBtnPosArr[i - menuStruct.menuOffset],btnArray[i].Text);
    }
    if(menuStruct.menuOffset != BTN_MIN_OFFSET)
    {
      VBUF_Draw_Image(79,8,5,8,ArrowUP);//Стрелка вверх
    }
    if(menuStruct.menuOffset != BTN_MAX_OFFSET)
    {
      VBUF_Draw_Image(79,40,5,8,ArrowDOWN);//Стрелка вниз
    }
    if(VoltagesOnBattery == FULL_VOLT_ON_BATTERY)
    {
      VBUF_Draw_Image(74,0,10,8,fullBATT);//Символ полной батареи
    }
    else if(VoltagesOnBattery == MID_VOLT_ON_BATTERY)
    {
      VBUF_Draw_Image(74,0,10,8,midBATT);//Символ средней батареи
    }
    else if(VoltagesOnBattery == LOW_VOLT_ON_BATTERY)
    {
      VBUF_Draw_Image(74,0,10,8,lowBATT);//Символ низкой батареи
    }
    else if(VoltagesOnBattery == EMPTY_VOLT_ON_BATTERY)
    {
      VBUF_Draw_Image(74,0,10,8,emptyBATT);//Символ средней батареи
    }
    
    invertMenuCurrentButton(0);
    DISP_Update();
    break;
  case LAST_LEVEL:
    vTaskResume(xLASTLEVELMENUHandle);
    break;
  }
}
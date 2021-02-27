#include "main.h"

TaskHandle_t xLASTLEVELMENUHandle = NULL;

Config_Struct tempConfig;

extern MENU_Struct menuStruct;

extern VDC_Struct SensorVoltageStruct;


void LASTLEVELMENU_Menu_Task_init(void)
{
  //Создание задачи обновления дисплея в меню настроек
  xTaskCreate(vLASTLEVELMENU_Menu_Task, "Settings_Menu", LASTLEVELMENU_STACK_SIZE, NULL, LASTLEVELMENU_TASK_PRIORITY, &xLASTLEVELMENUHandle);
  //После создания задачи сразу же останавливаем её
  vTaskSuspend(xLASTLEVELMENUHandle);
}

void vLASTLEVELMENU_Menu_Task (void *pvParameters)
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
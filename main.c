#include "main.h"

extern uint8_t bender[84 * 48 / 8];
extern uint8_t VBUF[84 * 48 / 8];

void vBlinkDisplay( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. */
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        lcd5110_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
        vTaskDelay(500);
        lcd5110_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED);
        vTaskDelay(500);
    }
}

void vClearDisp( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. */
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        lcd5110_Clear_DMA();
        vTaskDelay(100);
        VBUF_Send_DMA(bender);
        vTaskDelay(1000);
    }
}

int main()
{
  //Инициализация всей периферии
  initAll();
  xTaskCreate(vBlinkDisplay, "BlinkDisplay", 128, NULL, 5, NULL);
  xTaskCreate(vClearDisp, "ClearDisplay", 128, NULL, 5, NULL);
  vTaskStartScheduler();
  while(1)
  {
    //Сюда попадать не должны
  }
}

void vApplicationIdleHook(void){ }

void vApplicationTickHook(void){ }

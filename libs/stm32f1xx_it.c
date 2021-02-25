#include "main.h"
#include "stm32f1xx_it.h"

extern VDC_Struct SensorVoltageStruct;

extern Config_Struct* Config;

extern uint16_t ADCBuff[ADC_BUFFSIZE * CHAN_COUNT]; // Массив значений с АЦП. Данные с 4х каналов идут последовательно.

extern uint8_t flagCalib;


void DMA1_Channel3_IRQHandler()
{
  //Прерывание по концу передачи или из-за ошибки передачи
  if((DMA1->ISR & DMA_ISR_TCIF3) || (DMA1->ISR & DMA_ISR_TEIF3))
  {
    //Сбрасываем линию CHIP SELECT упрвления lcd5110
    CS_HIGH;
    //Сбрасываем флаги по прерываниям
    DMA1->IFCR |= DMA_IFCR_CTCIF3 | DMA_IFCR_CTEIF3;
  }
}

void TIM4_IRQHandler()
{
  //Прерывания по флагу обновления
  if(TIM4->SR & TIM_SR_UIF)
  {
    //DMA1_Channel1->CCR &= ~DMA_CCR_EN; //Останавливаем запись новых значений в буфер на время расчёта
    uint8_t buffPos = 0;
    int16_t* tempPointer = (int16_t*) &SensorVoltageStruct;
    uint32_t tempADCval[CHAN_COUNT] = {0};
    for(int i = 0; i < ADC_BUFFSIZE * CHAN_COUNT; i++)
    {
      tempADCval[i - (buffPos * CHAN_COUNT)] += ADCBuff[i];
      if(((i + 1) % CHAN_COUNT == 0))
      {
        buffPos++;
      }
    }
    for(int i = 0; i < CHAN_COUNT - 1; i++)
    {
      tempADCval[i] /= ADC_BUFFSIZE;
      tempPointer[i] = ((((int16_t)tempADCval[i] * 3300) / 4096) - 1630) - (flagCalib == 1 ? 0 :((int16_t*)Config)[i * 2]);
    }
    tempADCval[CHAN_COUNT - 1] /= ADC_BUFFSIZE;
    SensorVoltageStruct.BATT_VDC = ((tempADCval[CHAN_COUNT - 1] * 3300) / 4096) * 5286 / 1000;
    if(SensorVoltageStruct.BATT_VDC != 0 && SensorVoltageStruct.BATT_VDC < 5000)
    {
      enterStandBy();
    }
    //DMA1_Channel1->CCR |= DMA_CCR_EN; //Повторно запускаем запись
    TIM4->SR &= ~TIM_SR_UIF;
  }
}
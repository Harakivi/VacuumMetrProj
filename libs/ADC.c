#include "main.h"

VDC_Struct SensorVoltageStruct;

uint16_t ADCBuff[ADC_BUFFSIZE * CHAN_COUNT] = {0}; // Массив значений с АЦП. Данные с 4х каналов идут последовательно.

//Инициализация 5ти каналов АЦП 
void ADC1Init(void){
  RCC->CFGR &= ~(RCC_CFGR_ADCPRE_1 | RCC_CFGR_ADCPRE_0); //ADCPRE 00: PCLK2 divided by 2
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~(GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1);
  GPIOA->CRL &= ~(GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);
  GPIOA->CRL &= ~(GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF2_0 | GPIO_CRL_CNF2_1);
  GPIOA->CRL &= ~(GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF3_0 | GPIO_CRL_CNF3_1);
  GPIOA->CRL &= ~(GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
  ADC1->CR2 |= ADC_CR2_CONT;
  ADC1->CR1 |= ADC_CR1_SCAN; //1: Scan mode enabled
  ADC1->SQR3 =
    0x7 << ADC_SQR3_SQ1_Pos // Bits 4:0 SQ1[4:0]: first conversion in regular sequence = 00111(CH7)
  | 0x1 << ADC_SQR3_SQ2_Pos // Bits 9:5 SQ2[4:0]: first conversion in regular sequence = 00001(CH1)
  | 0x2 << ADC_SQR3_SQ3_Pos // Bits 14:10 SQ3[4:0]: first conversion in regular sequence = 00010(CH2)
  | 0x3 << ADC_SQR3_SQ4_Pos// Bits 19:15 SQ4[4:0]: first conversion in regular sequence = 00011(CH3)
  | 0x5 << ADC_SQR3_SQ5_Pos;// Bits 24:20 SQ5[4:0]: first conversion in regular sequence = 00101(CH5)
  ADC1->SQR1 = 0x4 << ADC_SQR1_L_Pos;// Regular channel sequence length = 0100: 5 conversions 
  ADC1->SMPR2 =
    0x7 << ADC_SMPR2_SMP7_Pos // 111: 239.5 cycles CH7   
  | 0x7 << ADC_SMPR2_SMP1_Pos // 111: 239.5 cycles CH1
  | 0x7 << ADC_SMPR2_SMP2_Pos // 111: 239.5 cycles CH2
  | 0x7 << ADC_SMPR2_SMP3_Pos // 111: 239.5 cycles CH3
  | 0x7 << ADC_SMPR2_SMP5_Pos; // 111: 239.5 cycles CH5
  ADC1->CR2 |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2 | ADC_CR2_EXTTRIG; //EXTTRIG 0: Conversion on external event disabled
  ADC1->CR2 |= ADC_CR2_DMA;//1: DMA mode enabled
  ADC1->CR2 |= ADC_CR2_ADON;
  ADC1->CR2 |= ADC_CR2_RSTCAL;
  ADC1->CR2 |= ADC_CR2_SWSTART;
  while ((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_RSTCAL){}
  ADC1->CR2 |= ADC_CR2_CAL;
  while ((ADC1->CR2 & ADC_CR2_CAL) == ADC_CR2_CAL){}
}

//Инициализация DMA для работы АЦП в непрерывном режиме
void DMA_ADCInit(void){
  ADC1Init();
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel1->CPAR = (uint32_t)(&ADC1->DR);
  DMA1_Channel1->CMAR = (uint32_t)ADCBuff;
  DMA1_Channel1->CCR = 
    0x0 << DMA_CCR_MEM2MEM_Pos //0: Memory to memory mode disabled
    | 0x00 << DMA_CCR_PL_Pos //00: Low
    | 0x01 << DMA_CCR_MSIZE_Pos //01: 16-bits
    | 0x01 << DMA_CCR_PSIZE_Pos //01: 16-bits
    | 0x1 << DMA_CCR_MINC_Pos //1: Memory increment mode enabled 
    | 0x0 << DMA_CCR_PINC_Pos //0: Peripheral increment mode disabled
    | 0x1 << DMA_CCR_CIRC_Pos //1: Circular mode enabled
    | 0x0 << DMA_CCR_DIR_Pos; //0: Read from peripheral
  DMA1_Channel1->CNDTR = ADC_BUFFSIZE * CHAN_COUNT; //Number of data to transfer
  DMA1_Channel1->CCR |= DMA_CCR_EN; //1: Channel enabled
  ADCTIMinit();
}

//Таймер для переодического усреднения значений буфера АЦП
void ADCTIMinit(){
  //Расчёт таймера:
  //-Тактирование от HCLK = 8MГц
  //-
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //1: Timer 1 clock enabled
  TIM4->CR1 |= TIM_CR1_CKD_1;//
  TIM4->CR1 &= ~TIM_CR1_CKD_0;//10: tDTS = 4 × tCK_INT
  TIM4->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS_1 | TIM_CR1_CMS_0); //0: Counter used as upcounter
  TIM4->CR1 &= ~TIM_CR1_OPM; //0: Counter is not stopped at update event
  TIM4->ARR = 1000;
  TIM4->PSC = 10;
  TIM4->EGR |= TIM_EGR_UG; //1: Re-initialize the counter and generates an update of the registers. Note that the prescaler
                           //counter is cleared too (anyway the prescaler ratio is not affected). The counter is cleared if
                           //the center-aligned mode is selected or if DIR=0 (upcounting), else it takes the auto-reload
                           //value (TIMx_ARR) if DIR=1 (downcounting).
  TIM4->DIER |= TIM_DIER_UIE;
  //NVIC_SetPriority(TIM4_IRQn, uint32_t priority)
  NVIC_EnableIRQ(TIM4_IRQn);
  TIM4->CR1 |= TIM_CR1_CEN;
}
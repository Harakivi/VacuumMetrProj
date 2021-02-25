#include "main.h"


extern uint8_t bender[84 * 48 / 8];

void DISP_Init()
{
  gpioInit();
  SPI2Init();
  bltInit();
  RST_LOW;
  for(int i=0; i < 1000; i++){}
  RST_HIGH;
  DISP_setBias(0x04);
  DISP_setContrast(94);
  // normal mode
  DISP_command(PCD8544_FUNCTIONSET);
  // Set display to Normal
  DISP_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
  //Очищаем мусор в памяти дисплея
  DISP_Clear_DMA();
}

void DISP_command(int data)
{
  while(!((GPIOB->IDR & GPIO_IDR_IDR4)>> 3U)){}
  DC_LOW;
  CS_LOW;
  SPI1->DR = data;
  while((SPI1->SR & SPI_SR_BSY) > 0){}
  CS_HIGH;
}

void DISP_data(uint8_t data)
{
  while(!((GPIOB->IDR & GPIO_IDR_IDR4)>> 3U)){}
  DC_HIGH;
  CS_LOW;
  SPI1->DR = data;
  while((SPI1->SR & SPI_SR_BSY) > 0){}
  CS_HIGH;
}

void DISP_Send_DMA(uint8_t* _VBUF)
{
  while(!((GPIOB->IDR & GPIO_IDR_IDR4)>> 3U)){}
  //Ставим указатель на начало экрана
  DISP_SetPos(0, 0);
  //Переключаем на lcd DATA
  DC_HIGH;
  CS_LOW;
  SPI1->CR2 |= SPI_CR2_TXDMAEN;
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel3->CCR &= ~DMA_CCR_EN;
  DMA1_Channel3->CPAR = (uint32_t)(&SPI1->DR);
  DMA1_Channel3->CMAR = (uint32_t)_VBUF;
  DMA1_Channel3->CCR = 
    0x0 << DMA_CCR_MEM2MEM_Pos //0: Memory to memory mode disabled
    | 0x00 << DMA_CCR_PL_Pos //00: Low
    | 0x00 << DMA_CCR_MSIZE_Pos //00: 8-bits
    | 0x00 << DMA_CCR_PSIZE_Pos //00: 8-bits
    | 0x1 << DMA_CCR_MINC_Pos //0: Memory increment mode disabled
    | 0x0 << DMA_CCR_PINC_Pos //0: Peripheral increment mode disabled
    | 0x0 << DMA_CCR_CIRC_Pos //0: Circular mode disabled
    | 0x1 << DMA_CCR_DIR_Pos //1: Write to peripheral
    | 0x1 << DMA_CCR_TCIE_Pos //1: Transfer complete interrupt enabled
    | 0x1 << DMA_CCR_TEIE_Pos;//1: Transfer error interrupt enabled
  DMA1_Channel3->CNDTR = 84 * 48 / 8; //Number of data to transfer
  NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  DMA1_Channel3->CCR |= DMA_CCR_EN; //1: Channel enabled
}

void DISP_setContrast(int val) 
{
  if (val > 0x7f) {
    val = 0x7f;
  }
  DISP_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  DISP_command(PCD8544_SETVOP | val);
  DISP_command(PCD8544_FUNCTIONSET);

 }


void DISP_setBias(int val) 
{
  if (val > 0x07) {
    val = 0x07;
  }
  DISP_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  DISP_command(PCD8544_SETBIAS | val);
  DISP_command(PCD8544_FUNCTIONSET);
}

void DISP_Send(uint8_t* _VBUF)
{
  DISP_SetPos(0,0);
  for(uint8_t p = 0; p < 6; p++)
  {
    for(uint16_t col = 0; col <= 83; col++)
    {
      DISP_data(_VBUF[(84*p) + col]);
    }
  }
}

void DISP_Clear()
{
  DISP_SetPos(0,0);
  for(uint8_t p = 0; p < 6; p++)
  {
    for(uint16_t col = 0; col <= 83; col++)
    {
      DISP_data(0);
    }
  }
}

void DISP_Clear_DMA()
{
  while(!((GPIOB->IDR & GPIO_IDR_IDR4)>> 3U)){}
  static uint8_t zero = 0x00; 
  //Ставим указатель на начало экрана
  DISP_SetPos(0, 0);
  //Переключаем на lcd DATA
  DC_HIGH;
  CS_LOW;
  SPI1->CR2 |= SPI_CR2_TXDMAEN;
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel3->CCR &= ~DMA_CCR_EN;
  DMA1_Channel3->CPAR = (uint32_t)(&SPI1->DR);
  DMA1_Channel3->CMAR = (uint32_t)&zero;
  DMA1_Channel3->CCR = 
    0x0 << DMA_CCR_MEM2MEM_Pos //0: Memory to memory mode disabled
    | 0x00 << DMA_CCR_PL_Pos //00: Low
    | 0x00 << DMA_CCR_MSIZE_Pos //00: 8-bits
    | 0x00 << DMA_CCR_PSIZE_Pos //00: 8-bits
    | 0x0 << DMA_CCR_MINC_Pos //0: Memory increment mode disabled
    | 0x0 << DMA_CCR_PINC_Pos //0: Peripheral increment mode disabled
    | 0x0 << DMA_CCR_CIRC_Pos //0: Circular mode disabled
    | 0x1 << DMA_CCR_DIR_Pos //1: Write to peripheral
    | 0x1 << DMA_CCR_TCIE_Pos //1: Transfer complete interrupt enabled
    | 0x1 << DMA_CCR_TEIE_Pos;//1: Transfer error interrupt enabled
  DMA1_Channel3->CNDTR = 84 * 48 / 8; //Number of data to transfer
  NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  DMA1_Channel3->CCR |= DMA_CCR_EN; //1: Channel enabled
}

void DISP_SetPos(uint8_t xPos, uint8_t yPos)
{
  DISP_command(PCD8544_SETYADDR | yPos);
  DISP_command(PCD8544_SETXADDR | xPos);
}

void gpioInit(){
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;// GPIOA clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;// GPIOB clock enable
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;// AFIO clock enable
  GPIOB->CRL |= GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1 |// D/C MODE CONFIG 11: Maximum output speed 50 MHz
                GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1 |// RST MODE CONFIG 11: Maximum output speed 50 MHz 
                GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1 |// CLK MODE CONFIG 11: Maximum output speed 50 MHz
                GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1 |// CHIP SELECT MODE CONFIG 11: Maximum output speed 50 MHz
                GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1 ;// MOSI MODE CONFIG 11: Maximum output speed 50 MHz
  GPIOB->CRL &= ~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);// D/C CNF CONFIG 00: GPIO Push-pull
  GPIOB->ODR |= GPIO_ODR_ODR1;// D/C SET TO HIGH
  GPIOB->CRL &= ~(GPIO_CRL_CNF2_0 | GPIO_CRL_CNF2_1);// RST CNF CONFIG 00: GPIO Push-pull
  GPIOB->ODR |= GPIO_ODR_ODR2;//RST SET TO HIGH
  GPIOB->CRL &= ~(GPIO_CRL_CNF4_0 | GPIO_CRL_CNF4_1);// CHIP SELECT CNF CONFIG 00: GPIO Push-pull
  GPIOB->ODR |= GPIO_ODR_ODR4;// CHIP SELECT SET TO LOW
  AFIO->MAPR |= AFIO_MAPR_SPI1_REMAP;
  GPIOB->CRL &= ~GPIO_CRL_CNF5_0;//
  GPIOB->CRL |= GPIO_CRL_CNF5_1;// MOSI CNF CONFIG 10: AF Push-pull
  GPIOB->CRL &= ~GPIO_CRL_CNF3_0;//
  GPIOB->CRL |= GPIO_CRL_CNF3_1;// CLK CNF CONFIG 10: AF Push-pull
}

void SPI2Init(){
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN ;// SPI2 clock enable
  SPI1->CR1 = 1 << SPI_CR1_BIDIMODE_Pos
            | 1 << SPI_CR1_BIDIOE_Pos
            | 0 << SPI_CR1_DFF_Pos
            | 0 << SPI_CR1_RXONLY_Pos
            | 0 << SPI_CR1_LSBFIRST_Pos
          | 000 << SPI_CR1_BR_Pos// 000: fPCLK/2 8/2=4Mhz
            | 1 << SPI_CR1_SSM_Pos | 1 << SPI_CR1_SSI_Pos
            | 1 << SPI_CR1_CPOL_Pos
            | 1 << SPI_CR1_CPHA_Pos
            | 1 << SPI_CR1_MSTR_Pos
            | 1 << SPI_CR1_SPE_Pos;
}

void bltInit(){
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //1: Timer 3 clock enabled
  TIM3->CR1 &= ~TIM_CR1_CKD_0;//
  TIM3->CR1 &= ~TIM_CR1_CKD_1;//00: tDTS=tCK_INT
  TIM3->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS_1 | TIM_CR1_CMS_0); //0: Counter used as upcounter
  TIM3->CR1 &= ~TIM_CR1_OPM; //0: Counter is not stopped at update event
  TIM3->ARR = 4096;
  TIM3->PSC = 6;
  TIM3->EGR |= TIM_EGR_UG; //1: Re-initialize the counter and generates an update of the registers. Note that the prescaler
                           //counter is cleared too (anyway the prescaler ratio is not affected). The counter is cleared if
                           //the center-aligned mode is selected or if DIR=0 (upcounting), else it takes the auto-reload
                           //value (TIMx_ARR) if DIR=1 (downcounting).
  TIM3->CR1 &= ~TIM_CR1_ARPE; //0: TIMx_ARR register is notDIR: buffered
  TIM3->SMCR &= ~TIM_SMCR_SMS_0; // 
  TIM3->SMCR &= ~TIM_SMCR_SMS_1; // 
  TIM3->SMCR &= ~TIM_SMCR_SMS_2; // 000: Slave mode disabled - if CEN = �1� then the prescaler is clocked directly by the internal clock
  TIM3->SMCR &= ~TIM_SMCR_ECE;//
  TIM3->CCMR2 &= ~TIM_CCMR2_OC3PE; //0: Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written at anytime, the new value is taken in account immediately
  TIM3->CCER |= TIM_CCER_CC3E; //0: Off - OC1 is not active. 
  TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;//
  TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;//110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive. In downcounting, channel 1 is inactive (OC1REF=�0�) as long as TIMx_CNT>TIMx_CCR1 else active (OC1REF=�1�).
  TIM3->CCMR2 &= ~(TIM_CCMR2_CC3S_0 | TIM_CCMR2_CC3S_1); //00: CC1 channel is configured as output
  TIM3->CCER &= ~TIM_CCER_CC3P; //0: OC1 active high
  TIM3->CCR3 = 0;
  TIM3->CCMR2 &= ~TIM_CCMR2_OC3FE; //Output compare 1 fast enable
                                   //This bit is used to accelerate the effect of an event on the trigger in input on the CC output.
                                   //0: CC1 behaves normally depending on counter and CCR1 values even when the trigger is
                                   //ON. The minimum delay to activate CC1 output when an edge occurs on the trigger input is
                                   //5 clock cycles.
  TIM3->CR2 &= ~TIM_CR2_MMS_0; //000: Reset 
  TIM3->CR2 &= ~TIM_CR2_MMS_1; //000: Reset 
  TIM3->CR2 &= ~TIM_CR2_MMS_2; //000: Reset 
  TIM3->SMCR &= ~TIM_SMCR_MSM; //Master/Slave mode 0: No action
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  GPIOB->CRL &= ~GPIO_CRL_CNF0_0;//
  GPIOB->CRL |= GPIO_CRL_CNF0_1;//10: AF Push-pull
  GPIOB->CRL |= GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1 ;//11: Maximum output speed 50 MHz
  TIM3->CR1 |= TIM_CR1_CEN;//1: Counter enabled
}
#include "main.h"
#include "BoardConfig.h"

//width 11 / height 32
extern  uint8_t chel_1 [];
//width 11 / height 32
extern  uint8_t chel_2 [];
//width 23 / height 32
extern uint8_t chel_smile[];

//Бендер ¯ \ _ (ツ) _ / ¯ 
extern uint8_t bender[84 * 48 / 8];

//size: 16x24
extern uint8_t H_symb[];
//size: 16x24
extern uint8_t A_symb[];
//size: 16x24
extern uint8_t R_symb[];
//size: 16x24
extern uint8_t K_symb[];
//size: 16x24
extern uint8_t I_symb[];
//size: 16x24
extern uint8_t V_symb[];
//size: 16x19
extern uint8_t b_symb[];
//size: 16x19
extern uint8_t y_symb[];

//Обьявление функций
//void SysTick_Init(void);//НЕАКТУАЛЬНО ПРИ РАБОТЕ FREERTOS

void disp_LOGO()
{
  VBUF_Clear();
  for(int16_t i = 83; i >= -128; i--)
  {
    VBUF_Clear();
    VBUF_Draw_Image(i, 0, 16, 24, H_symb);
    VBUF_Draw_Image(i + 16, 0, 16, 24, A_symb);
    VBUF_Draw_Image(i + 32, 0, 16, 24, R_symb);
    VBUF_Draw_Image(i + 48, 0, 16, 24, A_symb);
    VBUF_Draw_Image(i + 64, 0, 16, 24, K_symb);
    VBUF_Draw_Image(i + 80, 0, 16, 24, I_symb);
    VBUF_Draw_Image(i + 96, 0, 16, 24, V_symb);
    VBUF_Draw_Image(i + 112, 0, 16, 24, I_symb);
    DISP_Update();
    uint32_t timeoutLOGO = 50000;
    while(timeoutLOGO)
    {
      timeoutLOGO--;
    }
  }
  VBUF_Clear();
  VBUF_Write_String(16, 10, "CarbSynch");
  VBUF_Write_String(40, 20, "V0.02");
  VBUF_Write_String(20, 40, "by Harakivi");
  DISP_Update();
  //Висим несколько секунд, чтобы отобразить лого
  uint32_t timeoutLOGO = 1000000;
  while(timeoutLOGO && !ENTER_BTN_CHK)
  {
    timeoutLOGO--;
  }
}

void disp_oldLOGO()
{
  uint8_t pos = 84;
  while(pos != 0)
  {
    if(pos > 84)
    {
      pos = 84;
    }
    pos -= 2;
    VBUF_Clear();
    VBUF_Draw_Image(pos, 0, 11, 32, chel_1);
    VBUF_Write_String(10, 40, "by Harakivi");
    DISP_Update();
    uint32_t timeout = 200000;
    while(timeout)
    {
      timeout--;
    }
    
    if(pos == 42)
    {
      VBUF_Clear();
      VBUF_Draw_Image(pos - 2, 0, 23, 32, chel_smile);
      VBUF_Write_String(10, 40, "by Harakivi");
      DISP_Update();
      uint32_t timeout = 4000000;
      while(timeout)
      {
        timeout--;
      }
    }
    pos -= 2;
    VBUF_Clear();
    VBUF_Draw_Image(pos, 0, 11, 32, chel_2);
    VBUF_Write_String(10, 40, "by Harakivi");
    DISP_Update();
    timeout = 200000;
    while(timeout)
    {
      timeout--;
    }
  }
  VBUF_Clear();
  VBUF_Write_String(22, 10, "HARAKIVI");
  VBUF_Write_String(48, 19, "Prod.");
  DISP_Update();
  //Висим несколько секунд, чтобы отобразить лого
  uint32_t timeoutLOGO = 5000000;
  while(timeoutLOGO)
  {
    timeoutLOGO--;
  }
}

  
//Инициализация всей периферии
void initAll()
{
  //Впервую очередь инициализируем системное время
  //SysTick_Init();//НЕАКТУАЛЬНО ПРИ РАБОТЕ FREERTOS
  //Запускаем непрерырвную конвертацию АЦП и запись значений в буфер
  DISP_Init();
  disp_LOGO();
  DMA_ADCInit();
  //Если ENTER еще не отжат висим, пока не отожмут
  while(ENTER_BTN_CHK);
}

//Функция уводит контроллер в режим STANDBY, потребление в этом режиме ~3мкА
void enterStandBy(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  SCB->SCR |= (uint32_t)SCB_SCR_SLEEPDEEP_Msk ;
  PWR->CR |= PWR_CR_PDDS;
  PWR->CR |= PWR_CR_CWUF;
  WKUP_PIN_ENABLE;
  __WFI();
}

//Подать питание на внешнюю преиферию
void enablePWRDisp(uint8_t _PWR)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  GPIOB->CRH |= GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1;
  GPIOB->CRH |= GPIO_CRH_CNF10_0;
  GPIOB->CRH &= ~GPIO_CRH_CNF10_1;
  if(_PWR)
  {
    GPIOB->ODR &= ~GPIO_ODR_ODR10;
  }else {GPIOB->ODR |= GPIO_ODR_ODR10;}
  
}

//Подключить первую ступень основной батареи
void enableFirstStepMainBatt(uint8_t _PWR)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRH |= GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1;
  GPIOA->CRH |= GPIO_CRH_CNF10_0;
  GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
  if(_PWR)
  {
    GPIOA->ODR |= GPIO_ODR_ODR10;
  }else {GPIOA->ODR &= ~GPIO_ODR_ODR10;}
  
}

//Подключить вторую ступень основной батареи
void enableSecondStepMainBatt(uint8_t _PWR)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  GPIOB->CRH |= GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1;
  GPIOB->CRH |= GPIO_CRH_CNF12_0;
  GPIOB->CRH &= ~GPIO_CRH_CNF12_1;
  if(_PWR)
  {
    GPIOB->ODR &= ~GPIO_ODR_ODR12;
  }else {GPIOB->ODR |= GPIO_ODR_ODR12;}
  
}

uint16_t ReadVoltageOnMainBatt()
{
  RCC->CFGR &= ~(RCC_CFGR_ADCPRE_1 | RCC_CFGR_ADCPRE_0); //ADCPRE 00: PCLK2 divided by 2
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~(GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
  ADC1->SQR3 =
    0x5 << ADC_SQR3_SQ1_Pos;// Bits 4:0 SQ1[4:0]: first conversion in regular sequence = 00101(CH5)
  ADC1->SQR1 = 0x0 << ADC_SQR1_L_Pos;// Regular channel sequence length = 0000: 1 conversions 
  ADC1->SMPR2 =
    0x7 << ADC_SMPR2_SMP5_Pos; // 000: 1.5 cycles CH5
  ADC1->CR2 |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2 | ADC_CR2_EXTTRIG; //EXTTRIG 0: Conversion on external event disabled
  ADC1->CR2 |= ADC_CR2_ADON;
  uint16_t prevVoltage = 0;
  uint16_t currVolt = 100;
  uint16_t timeoToRead = 100;
  while(currVolt - prevVoltage > 10 && timeoToRead)
  {
    ADC1->CR2 |= ADC_CR2_SWSTART;
    uint16_t ReadTime = 100;
    while(!(ADC1->SR & ADC_SR_EOC) && ReadTime)
    {
      ReadTime--;
    }
    prevVoltage = currVolt;
    currVolt = ADC1->DR;
    timeoToRead--;
  }
  uint16_t voltage = 0;
  if(timeoToRead != 0)
  {
    voltage = ((ADC1->DR * 3300) / 4096) * 5286 / 1000;
  }
  return voltage;
}

//Функция вызывается в файле startup_stm32f103xb.s
//Вызывается самой первой для выхода из STANDBY 
void StandByCheck()
{
    //Чтобы ENTER работал отключаем режим WKUP для работы PA0 в обычном режиме GPIO
  WKUP_PIN_DISABLE;
  enableFirstStepMainBatt(1);
  //Измеряем напряжение на батарее
  uint16_t _Voltage = ReadVoltageOnMainBatt();
  //Если оно меньше допустимого
  if(_Voltage < MIN_VOLT_ON_BATTERY)
  {
    //То засыпаем
    enterStandBy();
  }
  enableSecondStepMainBatt(1);
  //Инициализируем пины GPIO к которым подключены кнопки, чтобы отслеживать нажатие кнопки ENTER
  initButt();
  //Если кнопка нажата, считаем время, если оно больше таймаута, выходим
  uint32_t timeoutEnable = 1000000;
  while(ENTER_BTN_CHK)
  {
    timeoutEnable--;
    if(!timeoutEnable)
    {
      break;
    }
  }
  //Если кнопка была нажата слишком мало времени, снова засыпаем
  if(timeoutEnable)
  {
    enterStandBy();
  }
  //Если не заснули, то инициализируем всё и начинаем работу
  //Открываем транзисторы
  enablePWRDisp(1);
}



//НЕАКТУАЛЬНО ПРИ РАБОТЕ FREERTOS
//Инициализация системного таймера для отсчёта времени и реализации задержки
//void SysTick_Init(){
//  SysTick->LOAD =  SystemCoreClock / 1000 - 1;
//  SysTick->VAL = SystemCoreClock / 1000 - 1;
//  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
//}
//НЕАКТУАЛЬНО ПРИ РАБОТЕ FREERTOS
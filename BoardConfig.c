#include "main.h"
#include "BoardConfig.h"

//Обьявление функций
//void SysTick_Init(void);//НЕАКТУАЛЬНО ПРИ РАБОТЕ FREERTOS
  
//Инициализация всей периферии
void initAll()
{
  //Впервую очередь инициализируем системное время
  //SysTick_Init();//НЕАКТУАЛЬНО ПРИ РАБОТЕ FREERTOS
  //Запускаем непрерырвную конвертацию АЦП и запись значений в буфер
  DISP_Init();
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
    GPIOA->ODR &= ~GPIO_ODR_ODR10;
  }else {GPIOA->ODR |= GPIO_ODR_ODR10;}
  
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
  ADC1->CR2 |= ADC_CR2_SWSTART;
  uint16_t timeoutRead = 100;
  while(!(ADC1->SR & ADC_SR_EOC) && timeoutRead);
  {
    timeoutRead--;
  }
  uint16_t _voltage = ((ADC1->DR * 3300) / 4096) * 5286 / 1000;
  return _voltage;
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
  //Если оно меньше допустимого, то пробуем измерить ещё раз
  if(_Voltage < MIN_VOLT_TO_START)
  {
    _Voltage = ReadVoltageOnMainBatt();
    //Если снова меньше допустимого засыпаем
    if(_Voltage < MIN_VOLT_TO_START)
    {
      enterStandBy();
    }
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
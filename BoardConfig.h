#ifndef _BOARDCONFIG_
#define _BOARDCONFIG_
/*---------------------Common Defines------------------*/
#define SOFTWARE_VERSION        {'V', build[11], '.',build[12], 0}  
#define MIN_VOLT_ON_BATTERY     5000 // 5.000 Вольт
#define CHECK_BATT_VOLT         TRUE //Проверка напряжения на батарее
#define TIMETOINACTIONSTANDBY   300000 //Время в милисекундах до перехода в режим Standby(300000 = 5 минут)
#define TIMETOINACTIONBRIGHTOFF 240000 //Время в милисекундах до отключения подстветки дисплея(300000 = 5 минут)

/*---------------------LCD5110 PINS Defines------------------*/
#define LCD_DC_PORT     GPIOB           // DATA/COMMAND GPIO PORT
#define LCD_DC_PIN      GPIO_ODR_ODR1   // DATA/COMMAND GPIO PIN
#define LCD_RST_PORT    GPIOB           // RST GPIO PORT
#define LCD_RST_PIN     GPIO_ODR_ODR2   // RST GPIO PIN
#define LCD_CS_PORT     GPIOB           // CHIP SELECT GPIO PORT
#define LCD_CS_PIN      GPIO_ODR_ODR4   // CHIP SELECT GPIO PIN
/*---------------------Buttons PINS Defines------------------*/
#define WKUP_PIN_DISABLE PWR->CSR &= ~PWR_CSR_EWUP
#define WKUP_PIN_ENABLE PWR->CSR |= PWR_CSR_EWUP
/*---------------------Buttons PINS Defines------------------*/

//------------------------------------------------
void initAll(void);
void enterStandBy(void);
void enablePWRDisp(uint8_t _PWR);
void enableFirstStepMainBatt(uint8_t _PWR);
void enableSecondStepMainBatt(uint8_t _PWR);

//------------------------------------------------
#endif /* _BOARDCONFIG_ */

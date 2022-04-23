#ifndef _BOARDCONFIG_
#define _BOARDCONFIG_
/*---------------------Common Defines------------------*/
#define SOFTWARE_VERSION        {'V', build[11], '.',build[12], 0}
//#define FULL_VOLT_ON_BATTERY    7800 // 7.800 Вольт
//#define MID_VOLT_ON_BATTERY     7200 // 7.200 Вольт
//#define MIN_VOLT_ON_BATTERY     6000 // 6.000 Вольт
//#define EMPTY_VOLT_ON_BATTERY   4000 // 4.000 Вольт
typedef enum {
    EMPTY_VOLT_ON_BATTERY = 4000,
    LOW_VOLT_ON_BATTERY = 6000,
    MID_VOLT_ON_BATTERY = 7200,
    FULL_VOLT_ON_BATTERY = 7800
}VoltagesOnBatteryEnum;
#define CHECK_BATT_VOLT//Проверка напряжения на батарее
#define MINTIMETOINACTIONSTANDBY   60000 //Минимальное время в милисекундах до перехода в режим Standby(90000 = 1.5 минуты)
#define MINTIMETOINACTIONBRIGHTOFF 60000 //Минимальное время в милисекундах до отключения подстветки дисплея(60000 = 1 минута)

typedef struct {
  uint32_t bright;
  uint32_t dimmerOff;
  uint32_t deviceOff;
}DISP_SETS_Struct;

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
void disp_LOWBATT();
void initAll(void);
void enterStandBy(void);
void enablePWRDisp(uint8_t _PWR);
void enableFirstStepMainBatt(uint8_t _PWR);
void enableSecondStepMainBatt(uint8_t _PWR);

//------------------------------------------------
#endif /* _BOARDCONFIG_ */

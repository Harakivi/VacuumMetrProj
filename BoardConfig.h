#ifndef _BOARDCONFIG_
#define _BOARDCONFIG_
/*---------------------LCD5110 PINS Defines------------------*/
#define LCD_DC_PORT     GPIOB           // DATA/COMMAND GPIO PORT
#define LCD_DC_PIN      GPIO_ODR_ODR1   // DATA/COMMAND GPIO PIN
#define LCD_RST_PORT    GPIOB           // RST GPIO PORT
#define LCD_RST_PIN     GPIO_ODR_ODR2   // RST GPIO PIN
#define LCD_CS_PORT     GPIOB           // CHIP SELECT GPIO PORT
#define LCD_CS_PIN      GPIO_ODR_ODR4   // CHIP SELECT GPIO PIN
/*---------------------Buttons PINS Defines------------------*/

/*---------------------Buttons PINS Defines------------------*/

//------------------------------------------------
void initAll(void);

//------------------------------------------------
#endif /* _BOARDCONFIG_ */

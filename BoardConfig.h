#ifndef _BOARDCONFIG_
#define _BOARDCONFIG_
/*---------------------LCD 5110 Defines------------------*/
#define DC_HIGH GPIOB->ODR |= GPIO_ODR_ODR1 /// DATA/COMMAND SET
#define DC_LOW GPIOB->ODR &= ~GPIO_ODR_ODR1 /// DATA/COMMAND RESET
#define RST_HIGH GPIOB->ODR |= GPIO_ODR_ODR2 /// RST SET
#define RST_LOW GPIOB->ODR &= ~GPIO_ODR_ODR2 /// RST RESET
#define CS_HIGH GPIOB->ODR |= GPIO_ODR_ODR12 /// CHIP SELECT SET
#define CS_LOW GPIOB->ODR &= ~GPIO_ODR_ODR12 /// CHIP SELECT RESET
/*---------------------LCD 5110 Defines------------------*/

//------------------------------------------------
void initAll(void);

//------------------------------------------------
#endif /* _BOARDCONFIG_ */

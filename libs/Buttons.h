#ifndef _BUTTONS_H_
#define _BUTTONS_H_
/*---------------------Buttons Defines------------------*/

#define UP_BTN_CASE          1
#define ENTER_BTN_CASE       1 << 1
#define DOWN_BTN_CASE        1 << 2
#define LEFT_BTN_CASE        1 << 3
#define RIGHT_BTN_CASE       1 << 4

#define UP_BTN_CHK !((GPIOA->IDR & GPIO_IDR_IDR9)>> 8U)
#define ENTER_BTN_CHK (GPIOA->IDR & GPIO_IDR_IDR0)
#define DOWN_BTN_CHK !((GPIOA->IDR & GPIO_IDR_IDR11)>> 10U)
#define LEFT_BTN_CHK !((GPIOA->IDR & GPIO_IDR_IDR12)>> 11U)
#define RIGHT_BTN_CHK !((GPIOA->IDR & GPIO_IDR_IDR15)>> 14U)

uint8_t GetButtons(void);
void initButt(void);


#endif /* _BUTTONS_H_ */
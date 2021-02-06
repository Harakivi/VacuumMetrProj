#ifndef _BUTTONS_H_
#define _BUTTONS_H_
/*---------------------Buttons Defines------------------*/
#define UP_BTN !((GPIOA->IDR & GPIO_IDR_IDR9)>> 8U)
#define ENTER_BTN !((GPIOA->IDR & GPIO_IDR_IDR10)>> 9U)
#define DOWN_BTN !((GPIOA->IDR & GPIO_IDR_IDR11)>> 10U)
#define LEFT_BTN !((GPIOA->IDR & GPIO_IDR_IDR12)>> 11U)
#define RIGHT_BTN !((GPIOA->IDR & GPIO_IDR_IDR15)>> 14U)

void initButt(void);

#endif /* _BUTTONS_H_ */
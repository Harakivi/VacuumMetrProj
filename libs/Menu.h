#ifndef _MENU_H_
#define _MENU_H_
/*---------------------MENU Defines------------------*/
//Depth
#define TOP_LEVEL                 0
#define MENU_LEVEL                1
#define SETTINGS_LEVEL            2

//Posistion
#define MENU_FIRST_POS            0
#define MENU_BRIGHT_POS           1
#define MENU_LAST_POS             3

//Позиции и размеры кнопок в меню
#define MENU_Y_BTN_POS1                2
#define MENU_Y_BTN_POS2                14
#define MENU_Y_BTN_POS3                26
#define MENU_Y_BTN_POS4                38

#define MENU_X_BTN_POS1                27
#define MENU_X_BTN_POS2                12
#define MENU_X_BTN_POS3                15
#define MENU_X_BTN_POS4                24

#define MENU_X_BTN_LEN1                30
#define MENU_X_BTN_LEN2                60
#define MENU_X_BTN_LEN3                54
#define MENU_X_BTN_LEN4                36



typedef struct {
  uint8_t menuDepth;
  uint8_t menuPosition;
}MENU_Struct;

void menuBtnProc(uint8_t btn);

void menuVBUFDraw();

#endif /* _MENU_H_ */
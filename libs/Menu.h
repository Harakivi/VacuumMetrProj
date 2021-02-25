#ifndef _MENU_H_
#define _MENU_H_
/*---------------------MENU Defines------------------*/
//Depth
#define TOP_LEVEL                 0
#define MENU_LEVEL                1
#define SETTINGS_LEVEL            2

//Button Posistions
#define BTN_FIRST_POS             0
#define BTN_LAST_POS              3

//Button Offsets
#define BTN_MIN_OFFSET             0
#define BTN_MAX_OFFSET             1


//Menu Positions
#define MENU_METER_POS            0
#define MENU_BRIGHT_POS           1
#define MENU_CALIBRATE_POS        2
#define MENU_RETURN_POS           3
#define MENU_TURNOFF_POS          4



//Позиции и размеры кнопок в меню
#define MENU_Y_BTN_POS1                2
#define MENU_Y_BTN_POS2                14
#define MENU_Y_BTN_POS3                26
#define MENU_Y_BTN_POS4                38

#define MENU_X_BTN_POS1                2
#define MENU_X_BTN_POS2                2
#define MENU_X_BTN_POS3                2
#define MENU_X_BTN_POS4                2



typedef struct {
  uint8_t menuDepth;
  uint8_t menuPosition;
  uint8_t menuOffset;
}MENU_Struct;

typedef struct {
  uint8_t pixelCount;
  char Text[14];
}BTN_Struct;

void menuBtnProc(uint8_t btn);
void menuVBUFDraw(void);
void menuButtArrayInit(void);
void MenuDISP_Task_init(void);
void vSettingsRefrash_Task(void *pvParameters);

#endif /* _MENU_H_ */
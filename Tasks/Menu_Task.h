#ifndef _MENU_TASK_H_
#define _MENU_TASK_H_
/*---------------------Menu Depth Defines------------------*/
#define TOP_LEVEL                 0
#define MENU_LEVEL                1
#define LAST_LEVEL                2
/*---------------------Last Level Depth Defines------------------*/
#define DISP_LEVEL                0
#define SET_LEVEL                 1
/*----------------Menu max values Defines--------------*/
#define MENU_BTN_CNT              9
#define BTN_FIRST_POS             0
#define BTN_LAST_POS              3
#define BTN_MIN_OFFSET            0
#define BTN_MAX_OFFSET            MENU_BTN_CNT - 4
/*----------------Menu Positions Defines--------------*/
#define MENU_METER_POS            0
#define MENU_METER_SET_POS        1
#define MENU_SCOPE_POS            2
#define MENU_DISPLAY_SET_POS      3
#define MENU_CALIBRATE_POS        4
#define MENU_GAME1_POS            5
#define MENU_GAME2_POS            6
#define MENU_ABOUT_POS            7
#define MENU_TURNOFF_POS          8
/*----------------LastLevel Calibrate Positions Defines--------------*/
#define BIAS_CALIB                0
#define AMPF_CALIB                1
#define CALIB_FIRST_PAGE          0
#define CALIB_SECOND_PAGE         1
#define CALIB_THIRD_PAGE          2
#define CALIB_BTN_PAGE            3
/*----------------LastLevel Calibrate Positions Defines--------------*/
#define BRIGHTNESS_DISP_SET       0
#define TIME_BRIGHT_OFF_DISP_SET  1
#define TIME_DISP_OFF_DISP_SET    2
/*----------------LastLevel Calibrate max values Defines--------------*/
#define CALIB_MIN_OFFSET          0
#define CALIB_MAX_OFFSET          3
#define CALIB_MIN_POS             0
#define CALIB_MAX_POS             1
/*----------------LastLevel Calibrate max values Defines--------------*/
#define DISP_SET_MIN_OFFSET       0
#define DISP_SET_MAX_OFFSET       0
#define DISP_SET_MIN_POS          0
#define DISP_SET_MAX_POS          2
/*----------------Позиции кнопок в меню--------------*/
#define MENU_Y_BTN_POS1                2
#define MENU_Y_BTN_POS2                14
#define MENU_Y_BTN_POS3                26
#define MENU_Y_BTN_POS4                38

#define MENU_X_BTN_POS1                2
#define MENU_X_BTN_POS2                2
#define MENU_X_BTN_POS3                2
#define MENU_X_BTN_POS4                2
/*----------------Описание структур--------------*/
typedef struct {
  uint8_t menuDepth;
  uint8_t menuPosition;
  uint8_t menuOffset;
  uint8_t lastLevelDepth;
  uint8_t lastLevelPosition;
  uint8_t lastLevelOffset;
}MENU_Struct;

typedef struct {
  uint8_t pixelCount;
  char Text[14];
}BTN_Struct;

/*----------------Определение функций--------------*/
BTN_Struct ButtCreate(char *characters);
void menuVBUFDraw(void);
void menuButtArrayInit(void);
void Menu_Task_init(void);
void vMENU_Task(void *pvParameters);

#endif /* _MENU_TASK_H_ */
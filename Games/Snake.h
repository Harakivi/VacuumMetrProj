#ifndef _SNAKE_H_
#define _SNAKE_H_
/*---------------------Дефайны------------------*/
typedef enum{
  SNAKE_RIGHT_DIR,
  SNAKE_UP_DIR,
  SNAKE_LEFT_DIR,
  SNAKE_DOWN_DIR
}SNAKE_Move_DIR;

#define MIN_Y                 3
#define MAX_Y                 15

#define MIN_X                 1
#define MAX_X                 27

#define INITIAL_LENGTH        3

#define SNAKE_GAME_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define SNAKE_DRAW_STACK_SIZE                (512)
#define SNAKE_BTN_STACK_SIZE                 (256)


/*---------------------Описание структур------------------*/
typedef struct _CELL_Struct{
  uint8_t xPos;
  uint8_t yPos;
  struct _CELL_Struct *prevCell;
  struct _CELL_Struct *nextCell;
}CELL_Struct;

//Двусвязный список ячеек Змеи
typedef struct {
  CELL_Struct *head;
  CELL_Struct *tail;
  SNAKE_Move_DIR moveDir;
  SNAKE_Move_DIR nextMoveDir;
  size_t length;
  uint16_t Speed;
  uint8_t GameOver;
}SNAKE_Struct;

//Яблоко, которое ест змея
typedef struct {
  uint8_t xPos;
  uint8_t yPos;
}Apple_Struct;



void vSnake_Start(void);
void vDrawSnake_Task (void *pvParameters);
void vBtnSnake_Task (void *pvParameters);

#endif /* _SNAKE_H_ */
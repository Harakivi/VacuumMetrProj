#ifndef _TETRIS_H_
#define _TETRIS_H_
/*---------------------Дефайны------------------*/
typedef enum{
  NONBLOCK,
  I_BLOCKTYPE,
  J_BLOCKTYPE,
  L_BLOCKTYPE,
  O_BLOCKTYPE,
  S_BLOCKTYPE,
  T_BLOCKTYPE,
  Z_BLOCKTYPE                 
}TETRIS_BLOCK_Type;


#define TETRIS_DISP_MIN_Y                 0
#define TETRIS_DISP_MAX_Y                 46

#define TETRIS_DISP_MIN_X                 31
#define TETRIS_DISP_MAX_X                 51

#define FIELD_MIN_Y                 0
#define FIELD_MAX_Y                 22

#define FIELD_MIN_X                 0
#define FIELD_MAX_X                 10

#define FIELD_LENGTH                 10
#define FIELD_HEIGHT                 22

#define TETRIS_GAME_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define TETRIS_DRAW_STACK_SIZE                (512)
#define TETRIS_BTN_STACK_SIZE                 (256)

//Структура, содержащая информацию о текущем блоке
typedef struct {
  TETRIS_BLOCK_Type BlockType;
  uint8_t yPosArr[4];
  uint8_t xPosArr[4];
}BLOCK_Struct;

//Структура, содержащая текущее состояние тетриса.
typedef struct{
  BOOL Field[22][10];
  uint8_t Field_Level;
  BLOCK_Struct currBlock;
  uint8_t GameOver;
}TETRIS_Struct;

void vTetris_Start(void);
void vDrawTetris_Task (void *pvParameters);
void vBtnTetris_Task (void *pvParameters);

#endif /* _TETRIS_H_ */
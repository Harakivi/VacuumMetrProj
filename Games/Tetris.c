#include "main.h"
#include <stdlib.h>
#include <string.h>

extern TaskHandle_t xLASTLEVELMENUHandle;
extern TaskHandle_t xGameDrawHandle;
extern TaskHandle_t xGameBtnHandle;

//Счётный семафор нажатой кнопки
extern xSemaphoreHandle xBtnPresSem;
extern QueueHandle_t xBtnPresQueue;

extern uint8_t VBUF[];

BLOCK_Struct gen_I_Block()
{
  BLOCK_Struct newBlock;
  newBlock.BlockType = I_BLOCKTYPE;
  newBlock.yPosArr[0] = 0;
  newBlock.yPosArr[1] = 0;
  newBlock.yPosArr[2] = 0;
  newBlock.yPosArr[3] = 0;
  newBlock.xPosArr[0] = 3;
  newBlock.xPosArr[1] = 4;
  newBlock.xPosArr[2] = 5;
  newBlock.xPosArr[3] = 6;
  return newBlock;
}

BLOCK_Struct gen_J_Block()
{
  BLOCK_Struct newBlock;
  newBlock.BlockType = J_BLOCKTYPE;
  newBlock.yPosArr[0] = 0;
  newBlock.yPosArr[1] = 0;
  newBlock.yPosArr[2] = 0;
  newBlock.yPosArr[3] = 1;
  newBlock.xPosArr[0] = 3;
  newBlock.xPosArr[1] = 4;
  newBlock.xPosArr[2] = 5;
  newBlock.xPosArr[3] = 5;
  return newBlock;
}

BLOCK_Struct gen_L_Block()
{
  BLOCK_Struct newBlock;
  newBlock.BlockType = L_BLOCKTYPE;
  newBlock.yPosArr[0] = 1;
  newBlock.yPosArr[1] = 0;
  newBlock.yPosArr[2] = 0;
  newBlock.yPosArr[3] = 0;
  newBlock.xPosArr[0] = 3;
  newBlock.xPosArr[1] = 3;
  newBlock.xPosArr[2] = 4;
  newBlock.xPosArr[3] = 5;
  return newBlock;
}

BLOCK_Struct gen_O_Block()
{
  BLOCK_Struct newBlock;
  newBlock.BlockType = O_BLOCKTYPE;
  newBlock.yPosArr[0] = 1;
  newBlock.yPosArr[1] = 1;
  newBlock.yPosArr[2] = 0;
  newBlock.yPosArr[3] = 0;
  newBlock.xPosArr[0] = 4;
  newBlock.xPosArr[1] = 5;
  newBlock.xPosArr[2] = 4;
  newBlock.xPosArr[3] = 5;
  return newBlock;
}

BLOCK_Struct gen_S_Block()
{
  BLOCK_Struct newBlock;
  newBlock.BlockType = S_BLOCKTYPE;
  newBlock.yPosArr[0] = 1;
  newBlock.yPosArr[1] = 1;
  newBlock.yPosArr[2] = 0;
  newBlock.yPosArr[3] = 0;
  newBlock.xPosArr[0] = 3;
  newBlock.xPosArr[1] = 4;
  newBlock.xPosArr[2] = 4;
  newBlock.xPosArr[3] = 5;
  return newBlock;
}

BLOCK_Struct gen_T_Block()
{
  BLOCK_Struct newBlock;
  newBlock.BlockType = T_BLOCKTYPE;
  newBlock.yPosArr[0] = 0;
  newBlock.yPosArr[1] = 0;
  newBlock.yPosArr[2] = 0;
  newBlock.yPosArr[3] = 1;
  newBlock.xPosArr[0] = 3;
  newBlock.xPosArr[1] = 4;
  newBlock.xPosArr[2] = 5;
  newBlock.xPosArr[3] = 4;
  return newBlock;
}

BLOCK_Struct gen_Z_Block()
{
  BLOCK_Struct newBlock;
  newBlock.BlockType = Z_BLOCKTYPE;
  newBlock.yPosArr[0] = 0;
  newBlock.yPosArr[1] = 0;
  newBlock.yPosArr[2] = 1;
  newBlock.yPosArr[3] = 1;
  newBlock.xPosArr[0] = 3;
  newBlock.xPosArr[1] = 4;
  newBlock.xPosArr[2] = 4;
  newBlock.xPosArr[3] = 5;
  return newBlock;
}

void Tetris_DeInit(TETRIS_Struct *TETRIS)
{
  vPortFree(TETRIS);
}

void vTetris_Start(void)
{
  //выделяем память под нужды игры
  void *memoryToStructs = pvPortMalloc(sizeof(TETRIS_Struct));
  memset(memoryToStructs, 0 ,sizeof(TETRIS_Struct));
  //Создание задач игры
  xTaskCreate(vDrawTetris_Task, "DRAW_TETRIS_TASK", TETRIS_DRAW_STACK_SIZE, memoryToStructs, TETRIS_GAME_TASK_PRIORITY, &xGameDrawHandle);
  xTaskCreate(vBtnTetris_Task, "BTN_TETRIS_TASK", TETRIS_BTN_STACK_SIZE, memoryToStructs, TETRIS_GAME_TASK_PRIORITY, &xGameBtnHandle);
  //После создания задач игры сразу же останавливаем родительскую задачу
  vTaskSuspend(xLASTLEVELMENUHandle);
}

void Tetris_Draw(TETRIS_Struct *TETRIS)
{
  VBUF_Draw_Line(TETRIS_DISP_MIN_X - 2, TETRIS_DISP_MIN_Y, 0 - ((FIELD_HEIGHT * 2) + 4), 1);
  VBUF_Draw_Line(TETRIS_DISP_MAX_X + 2, TETRIS_DISP_MIN_Y, 0 - ((FIELD_HEIGHT * 2) + 4), 1);
  VBUF_Draw_Line(TETRIS_DISP_MIN_X - 1, TETRIS_DISP_MAX_Y + 2, 1, (FIELD_LENGTH * 2) + 3);
  for(int i = 0; i < TETRIS->Field_Level; i++)
  {
    for(int j = 0; j < FIELD_LENGTH; j++)
    {
      uint8_t pixel = TETRIS->Field[FIELD_HEIGHT - i - 1][j];
      VBUF_Draw_Pixel((TETRIS_DISP_MIN_X + (j * 2)), TETRIS_DISP_MAX_Y - (i * 2), pixel);
      VBUF_Draw_Pixel((TETRIS_DISP_MIN_X + (j * 2)) + 1, TETRIS_DISP_MAX_Y - (i * 2), pixel);
      VBUF_Draw_Pixel((TETRIS_DISP_MIN_X + (j * 2)), TETRIS_DISP_MAX_Y - (i * 2) - 1, pixel);
      VBUF_Draw_Pixel((TETRIS_DISP_MIN_X + (j * 2)) + 1, TETRIS_DISP_MAX_Y - (i * 2) - 1, pixel);
      //VBUF[(((((TETRIS->Field[i][j] * 2)/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + FIELD_MIN_X))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + FIELD_MIN_Y) + 1) & (8 - 1)));
                //VBUF[(((((TETRIS->currBlock.yPosArr[i] * 2) + FIELD_MIN_Y) + 1)/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + FIELD_MIN_X + 1))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + FIELD_MIN_Y + 1)) & (8 - 1)));
               // VBUF[(((((TETRIS->currBlock.yPosArr[i] * 2) + FIELD_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + FIELD_MIN_X))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + FIELD_MIN_Y)) & (8 - 1)));
               // VBUF[(((((TETRIS->currBlock.yPosArr[i] * 2) + FIELD_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + FIELD_MIN_X + 1))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + FIELD_MIN_Y)) & (8 - 1)));
    }
  }
  for(int i = 0; i < 4; i++)
  {
    VBUF[(((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y) + 1)/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + TETRIS_DISP_MIN_X))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y) + 1) & (8 - 1)));
    VBUF[(((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y) + 1)/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + TETRIS_DISP_MIN_X + 1))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y + 1)) & (8 - 1)));
    VBUF[(((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + TETRIS_DISP_MIN_X))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y)) & (8 - 1)));
    VBUF[(((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[i] * 2) + TETRIS_DISP_MIN_X + 1))] |= (1 << ((((TETRIS->currBlock.yPosArr[i] * 2) + TETRIS_DISP_MIN_Y)) & (8 - 1)));
  }
//  VBUF[(((((TETRIS->currBlock.yPosArr[0] * 2) + FIELD_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[0] * 2) + FIELD_MIN_X))] |= (1 << (((((TETRIS->currBlock.yPosArr[0] * 2) + FIELD_MIN_Y))/8) & (8 - 1)));
//  VBUF[(((((TETRIS->currBlock.yPosArr[1] * 2) + FIELD_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[1] * 2) + FIELD_MIN_X))] |= (1 << (((((TETRIS->currBlock.yPosArr[1] * 2) + FIELD_MIN_Y))/8) & (8 - 1)));
//  VBUF[(((((TETRIS->currBlock.yPosArr[2] * 2) + FIELD_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[2] * 2) + FIELD_MIN_X))] |= (1 << (((((TETRIS->currBlock.yPosArr[2] * 2) + FIELD_MIN_Y))/8) & (8 - 1)));
//  VBUF[(((((TETRIS->currBlock.yPosArr[3] * 2) + FIELD_MIN_Y))/8) * 84) + (((TETRIS->currBlock.xPosArr[3] * 2) + FIELD_MIN_X))] |= (1 << (((((TETRIS->currBlock.yPosArr[3] * 2) + FIELD_MIN_Y))/8) & (8 - 1)));
}

void Tetris_Move(TETRIS_Struct *TETRIS)
{
  for(int i = 0; i < 4; i++)
  {
    if(TETRIS->Field[TETRIS->currBlock.yPosArr[i] + 1][TETRIS->currBlock.xPosArr[i]])
    {
      TETRIS->Field[TETRIS->currBlock.yPosArr[0]][TETRIS->currBlock.xPosArr[0]] = TRUE;
      TETRIS->Field[TETRIS->currBlock.yPosArr[1]][TETRIS->currBlock.xPosArr[1]] = TRUE;
      TETRIS->Field[TETRIS->currBlock.yPosArr[2]][TETRIS->currBlock.xPosArr[2]] = TRUE;
      TETRIS->Field[TETRIS->currBlock.yPosArr[3]][TETRIS->currBlock.xPosArr[3]] = TRUE;
      uint8_t ymin = FIELD_HEIGHT;
      uint8_t ymax = 0;
      for(int j = 0; j < 4; j++)
      {
        if(TETRIS->currBlock.yPosArr[j] < ymin)
        {
          ymin = TETRIS->currBlock.yPosArr[j];
        }
        if(TETRIS->currBlock.yPosArr[j] > ymax)
        {
          ymax = TETRIS->currBlock.yPosArr[j];
        }
      }
        TETRIS->Field_Level += (ymax - ymin) + 1;
      TETRIS->currBlock.BlockType = NONBLOCK;
      return;
    }
    else if(TETRIS->currBlock.yPosArr[i] == FIELD_HEIGHT - 1)
    {
      TETRIS->Field[TETRIS->currBlock.yPosArr[0]][TETRIS->currBlock.xPosArr[0]] = TRUE;
      TETRIS->Field[TETRIS->currBlock.yPosArr[1]][TETRIS->currBlock.xPosArr[1]] = TRUE;
      TETRIS->Field[TETRIS->currBlock.yPosArr[2]][TETRIS->currBlock.xPosArr[2]] = TRUE;
      TETRIS->Field[TETRIS->currBlock.yPosArr[3]][TETRIS->currBlock.xPosArr[3]] = TRUE;
      uint8_t ymin = FIELD_HEIGHT;
      uint8_t ymax = 0;
      for(int j = 0; j < 4; j++)
      {
        if(TETRIS->currBlock.yPosArr[j] < ymin)
        {
          ymin = TETRIS->currBlock.yPosArr[j];
        }
        if(TETRIS->currBlock.yPosArr[j] > ymax)
        {
          ymax = TETRIS->currBlock.yPosArr[j];
        }
      }
      TETRIS->Field_Level += (ymax - ymin) + 1;
      TETRIS->currBlock.BlockType = NONBLOCK;
      return;
    }
  }
  for(int i = 0; i < 4; i++)
  {
    TETRIS->currBlock.yPosArr[i]++;
  }
}

void vDrawTetris_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  
  TETRIS_Struct *TETRIS = (TETRIS_Struct*)pvParameters;;
  TETRIS->Speed = 0xff;
  
  for(;;)
  {
    if(TETRIS->currBlock.BlockType == NONBLOCK)
    {
      uint8_t BlockType = (rand() % 6) + 1;
      switch(BlockType)
      {
      case I_BLOCKTYPE:
        TETRIS->currBlock = gen_I_Block();
        break;
      case J_BLOCKTYPE:
        TETRIS->currBlock = gen_J_Block();
        break;
      case L_BLOCKTYPE:
        TETRIS->currBlock = gen_L_Block();
        break;
      case O_BLOCKTYPE:
        TETRIS->currBlock = gen_O_Block();
        break;
      case S_BLOCKTYPE:
        TETRIS->currBlock = gen_S_Block();
        break;
      case T_BLOCKTYPE:
        TETRIS->currBlock = gen_T_Block();
        break;
      case Z_BLOCKTYPE:
        TETRIS->currBlock = gen_Z_Block();
        break;
      }
    }                         
    if(!TETRIS->GameOver)
    {
      Tetris_Move(TETRIS);
      if(TETRIS->Field_Level >= FIELD_HEIGHT)
      {
        TETRIS->GameOver = TRUE;
      }
    }
    else{
      VBUF_Clear();
      VBUF_Write_String(15, 20, "Game Over");
      DISP_Update();
      vTaskDelay(TETRIS->Speed);
      
    }
    VBUF_Clear();
    Tetris_Draw(TETRIS);
    DISP_Update();
    vTaskDelay(TETRIS->Speed);
  }
}

void TetrisShiftLeft(TETRIS_Struct *TETRIS)
{
  for(int i = 0; i < 4; i++)
  {
    if(TETRIS->currBlock.xPosArr[i] == 0)
    {
      return;
    }
  }
  for(int i = 0; i < 4; i++)
  {
    TETRIS->currBlock.xPosArr[i] = TETRIS->currBlock.xPosArr[i] - 1;
  }
}

void TetrisShiftRight(TETRIS_Struct *TETRIS)
{
  for(int i = 0; i < 4; i++)
  {
    if(TETRIS->currBlock.xPosArr[i] >= FIELD_LENGTH - 1)
    {
      return;
    }
  }
  for(int i = 0; i < 4; i++)
  {
    TETRIS->currBlock.xPosArr[i] = TETRIS->currBlock.xPosArr[i] + 1;
  }
}

void vBtnTetris_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  
  TETRIS_Struct *TETRIS = (TETRIS_Struct*)pvParameters;;
  
  uint8_t btn = 0;
  
  for(;;)
  {
    xSemaphoreTake( xBtnPresSem, portMAX_DELAY );
    if(xQueueReceive( xBtnPresQueue, &btn, ( TickType_t ) 10 ) == pdPASS)
    {
      //Обработка нажатой кнопки в игре
      switch(btn)
      {
      case UP_BTN_CASE:
        break;
      case DOWN_BTN_CASE:
        break;
      case LEFT_BTN_CASE:
        TetrisShiftLeft(TETRIS);
        break;
      case RIGHT_BTN_CASE:
        TetrisShiftRight(TETRIS);
        break;
      case ENTER_BTN_CASE:
        Tetris_DeInit(TETRIS);
        vTaskResume(xLASTLEVELMENUHandle);
        break;
      }
    }
  }
}

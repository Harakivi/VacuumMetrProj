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

void Snake_Init(uint8_t _count, uint16_t _Speed, SNAKE_Struct *SNAKE, Apple_Struct *APPLE)
{
  APPLE->xPos = rand() % MAX_X;
  while( APPLE->xPos < MIN_X || APPLE->xPos > MAX_X)
  {
    APPLE->xPos = rand() % MAX_X;
  }
  APPLE->yPos = rand() % MAX_Y;
  while( APPLE->yPos < MIN_Y || APPLE->yPos > MAX_Y)
  {
    APPLE->yPos = rand() % MAX_Y;
  }
  CELL_Struct *currCell = NULL;
  CELL_Struct *headCell = malloc(sizeof(CELL_Struct));
  headCell->prevCell = currCell;
  headCell->nextCell = NULL;
  headCell->xPos = 15;
  headCell->yPos = 6;
  SNAKE->head = headCell;
  SNAKE->tail = headCell;
  SNAKE->moveDir = SNAKE_RIGHT_DIR;
  SNAKE->nextMoveDir = SNAKE_RIGHT_DIR;
  SNAKE->length++;
  SNAKE->Speed = _Speed;
  currCell = headCell;
  for(int i = 0; i < _count - 1; i++)
  {
    CELL_Struct *_Cell = malloc(sizeof(CELL_Struct));
    _Cell->prevCell = currCell;
    _Cell->prevCell->nextCell = _Cell;
    _Cell->nextCell = NULL;
    _Cell->xPos = 14 - i;
    _Cell->yPos = 6;
    currCell = _Cell;
    SNAKE->tail = _Cell;
    SNAKE->length++;
  }
}

void Snake_DeInit(SNAKE_Struct *SNAKE, Apple_Struct *APPLE)
{
  CELL_Struct *_Cell = SNAKE->head;
  CELL_Struct *_nextCell = _Cell->nextCell;
  while(_nextCell != NULL)
  {
    free(_Cell);
    _Cell = _nextCell;
    _nextCell = _nextCell->nextCell;
  }
  free(SNAKE);
  //free(APPLE);
}


void Snake_Move(SNAKE_Struct *SNAKE, Apple_Struct *APPLE)
{
  CELL_Struct *newHead = SNAKE->tail;
  CELL_Struct *newTail = SNAKE->tail->prevCell;
  newHead->prevCell = NULL;
  newTail->nextCell = NULL;
  newHead->nextCell = SNAKE->head;
  SNAKE->head->prevCell = newHead;
  SNAKE->head = newHead;
  SNAKE->tail = newTail;
  switch(SNAKE->nextMoveDir)
  {
  case SNAKE_RIGHT_DIR:
    if(SNAKE->head->nextCell->xPos != MAX_X)
    {
      SNAKE->head->xPos = SNAKE->head->nextCell->xPos + 1;
    }else{SNAKE->head->xPos = MIN_X;}
    SNAKE->head->yPos = SNAKE->head->nextCell->yPos;
    break;
  case SNAKE_UP_DIR:
    if(SNAKE->head->nextCell->yPos != MIN_Y)
    {
      SNAKE->head->yPos = SNAKE->head->nextCell->yPos - 1;
    }else{SNAKE->head->yPos = MAX_Y;}
    SNAKE->head->xPos = SNAKE->head->nextCell->xPos;
    break;
  case SNAKE_LEFT_DIR:
    if(SNAKE->head->nextCell->xPos != MIN_X)
    {
      SNAKE->head->xPos = SNAKE->head->nextCell->xPos - 1;;
    }else{SNAKE->head->xPos = MAX_X;}
    SNAKE->head->yPos = SNAKE->head->nextCell->yPos;
    break;
  case SNAKE_DOWN_DIR:
    if(SNAKE->head->nextCell->yPos != MAX_Y)
    {
      SNAKE->head->yPos = SNAKE->head->nextCell->yPos + 1;
    }else{SNAKE->head->yPos = MIN_Y;}
    SNAKE->head->xPos = SNAKE->head->nextCell->xPos;
    break;
  }
  SNAKE->moveDir = SNAKE->nextMoveDir;
  CELL_Struct *currCell = SNAKE->head->nextCell;
  while(currCell != NULL)
  {
    if(currCell->xPos == SNAKE->head->xPos)
    {
      if(currCell->yPos == SNAKE->head->yPos)
      {
        SNAKE->GameOver = 1;
        SNAKE->Speed = 500;
      }
    }
    currCell = currCell->nextCell;
  }
  if(SNAKE->head->xPos == APPLE->xPos && SNAKE->head->yPos == APPLE->yPos)
  {
    CELL_Struct *apple = malloc(sizeof(CELL_Struct));
    apple->xPos = SNAKE->tail->xPos;
    apple->yPos = SNAKE->tail->yPos;
    apple->prevCell = SNAKE->tail;
    apple->nextCell = NULL;
    SNAKE->tail->nextCell = apple;
    SNAKE->tail = apple;
    SNAKE->length++;
    APPLE->xPos = rand() % MAX_X;
    while( APPLE->xPos < MIN_X || APPLE->xPos > MAX_X )
    {
      APPLE->xPos = rand() % MAX_X;
    }
    APPLE->yPos = rand() % MAX_Y;
    while( APPLE->yPos < MIN_Y || APPLE->yPos > MAX_Y)
    {
      APPLE->yPos = rand() % MAX_Y;
    }
  }
}



void Snake_Draw(SNAKE_Struct *SNAKE, Apple_Struct *APPLE)
{
  CELL_Struct *currCell = SNAKE->head;
  while(currCell != NULL)
  {
    VBUF[((((currCell->yPos * 3) - 1)/8) * 84) + (currCell->xPos * 3)] |= (1 << (((currCell->yPos * 3) - 1) & 8 - 1));
    VBUF[((((currCell->yPos * 3) - 1)/8) * 84) + (currCell->xPos * 3) - 1] |= (1 << (((currCell->yPos * 3) - 1) & 8 - 1));
    VBUF[((((currCell->yPos * 3) - 1)/8) * 84) + (currCell->xPos * 3) + 1] |= (1 << (((currCell->yPos * 3) - 1) & 8 - 1));
    VBUF[((((currCell->yPos * 3) + 1)/8) * 84) + (currCell->xPos * 3)] |= (1 << (((currCell->yPos * 3) + 1) & 8 - 1));
    VBUF[((((currCell->yPos * 3) + 1)/8) * 84) + (currCell->xPos * 3) - 1] |= (1 << (((currCell->yPos * 3) + 1) & 8 - 1));
    VBUF[((((currCell->yPos * 3) + 1)/8) * 84) + (currCell->xPos * 3) + 1] |= (1 << (((currCell->yPos * 3) + 1) & 8 - 1));
    VBUF[(((currCell->yPos * 3)/8) * 84) + (currCell->xPos * 3)] |= (1 << ((currCell->yPos * 3) & 8 - 1));
    VBUF[(((currCell->yPos * 3)/8) * 84) + (currCell->xPos * 3) - 1] |= (1 << ((currCell->yPos * 3) & 8 - 1));
    VBUF[(((currCell->yPos * 3)/8) * 84) + (currCell->xPos * 3) + 1] |= (1 << ((currCell->yPos * 3) & 8 - 1));
    currCell = currCell->nextCell;
  } 
  VBUF[((((APPLE->yPos * 3) - 1)/8) * 84) + (APPLE->xPos * 3)] |= (1 << (((APPLE->yPos * 3) - 1) & 8 - 1));
  VBUF[((((APPLE->yPos * 3) + 1)/8) * 84) + (APPLE->xPos * 3)] |= (1 << (((APPLE->yPos * 3) + 1) & 8 - 1));
  VBUF[(((APPLE->yPos * 3)/8) * 84) + (APPLE->xPos * 3) - 1] |= (1 << ((APPLE->yPos * 3) & 8 - 1));
  VBUF[(((APPLE->yPos * 3)/8) * 84) + (APPLE->xPos * 3) + 1] |= (1 << ((APPLE->yPos * 3) & 8 - 1));
  VBUF_Draw_Line(0, 8, -40, 1);
  VBUF_Draw_Line(1, 9, 1, 82);
  VBUF_Draw_Line(83, 8, -40, 1);
  VBUF_Draw_Line(1, 48, 1, 82);
  char string[10] = {0};
  sprintf(string, "%i", (SNAKE->length - INITIAL_LENGTH) * 10);
  VBUF_Write_String(0,0,string);
}

void vSnake_Start(void)
{
  //выделяем память под нужды игры
  void *memoryToStructs = malloc(sizeof(SNAKE_Struct) + sizeof(Apple_Struct));
  memset(memoryToStructs, 0 ,sizeof(SNAKE_Struct) + sizeof(Apple_Struct)); 
  //Создание задач игры
  xTaskCreate(vDrawSnake_Task, "DRAW_SNAKE_TASK", SNAKE_DRAW_STACK_SIZE, memoryToStructs, SNAKE_GAME_TASK_PRIORITY, &xGameDrawHandle);
  xTaskCreate(vBtnSnake_Task, "BTN_SNAKE_TASK", SNAKE_BTN_STACK_SIZE, memoryToStructs, SNAKE_GAME_TASK_PRIORITY, &xGameBtnHandle);
  //После создания задач игры сразу же останавливаем родительскую задачу
  vTaskSuspend(xLASTLEVELMENUHandle);
}

void vDrawSnake_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  //configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  
  SNAKE_Struct *SNAKE = (SNAKE_Struct*)pvParameters;
  Apple_Struct *APPLE = (Apple_Struct*)(SNAKE + 1);
  
  Snake_Init(INITIAL_LENGTH, 250, SNAKE, APPLE);
  
  for(;;)
  {
    if(!SNAKE->GameOver)
    {
      Snake_Move(SNAKE, APPLE);
    }else
    {
      VBUF_Clear();
      VBUF_Write_String(15, 20, "Game Over");
      DISP_Update();
      vTaskDelay(SNAKE->Speed);
      
    }
    VBUF_Clear();
    Snake_Draw(SNAKE, APPLE);
    DISP_Update();
    vTaskDelay(SNAKE->Speed);
  }
}

void vBtnSnake_Task (void *pvParameters)
{
  //Ожидается что pvParameters будет равен 1
  //configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
  
  SNAKE_Struct *SNAKE = (SNAKE_Struct*)pvParameters;
  Apple_Struct *APPLE = (Apple_Struct*)(SNAKE + 1);
  
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
        if(SNAKE->moveDir == SNAKE_LEFT_DIR || SNAKE->moveDir == SNAKE_RIGHT_DIR)
        {
          SNAKE->nextMoveDir = SNAKE_UP_DIR;
        }
        break;
      case DOWN_BTN_CASE:
        if(SNAKE->moveDir == SNAKE_LEFT_DIR || SNAKE->moveDir == SNAKE_RIGHT_DIR)
        {
          SNAKE->nextMoveDir = SNAKE_DOWN_DIR;
        }
        break;
      case LEFT_BTN_CASE:
        if(SNAKE->moveDir == SNAKE_UP_DIR || SNAKE->moveDir == SNAKE_DOWN_DIR)
        {
          SNAKE->nextMoveDir = SNAKE_LEFT_DIR;
        }
        break;
      case RIGHT_BTN_CASE:
        if(SNAKE->moveDir == SNAKE_UP_DIR || SNAKE->moveDir == SNAKE_DOWN_DIR)
        {
          SNAKE->nextMoveDir = SNAKE_RIGHT_DIR;
        }
        break;
      case ENTER_BTN_CASE:
        Snake_DeInit(SNAKE, APPLE);
        vTaskResume(xLASTLEVELMENUHandle);
        break;
      }
    }
  }
}

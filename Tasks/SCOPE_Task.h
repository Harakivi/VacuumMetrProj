#ifndef _SCOPE_TASK_H_
#define _SCOPE_TASK_H_
//-------------------Глобальные дейфайны--------------------------
#define SCOPE_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define SCOPE_STACK_SIZE                (128)

#define ACCUM_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define ACCUM_STACK_SIZE                (256)

typedef struct{
  int16_t* PDE_Array;
  uint8_t Data_CNT;
}SCOPE_Struct;

void SCOPE_Task_init(void);
void SCOPE_Task_Deinit(void);
void vSCOPE_Task(void *pvParameters);
void vACCUM_Task(void *pvParameters);

//------------------------------------------------
#endif /* _SCOPE_TASK_H_ */
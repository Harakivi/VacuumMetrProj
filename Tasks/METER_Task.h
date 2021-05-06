#ifndef _METER_TASK_H_
#define _METER_TASK_H_
//-------------------Глобальные дейфайны--------------------------

typedef struct{
  uint8_t SizeFilt;
  int16_t* pPDE1_Array;
  int16_t* pPDE2_Array;
  int16_t* pPDE3_Array;
  int16_t* pPDE4_Array;
  int16_t PDE1_Value;
  int16_t PDE2_Value;
  int16_t PDE3_Value;
  int16_t PDE4_Value;
}FILT_Struct;

void METER_Task_init(void);
void vMETER_Task(void *pvParameters);
void vFILTER_Task(void *pvParameters);

//------------------------------------------------
#endif /* _METER_TASK_H_ */
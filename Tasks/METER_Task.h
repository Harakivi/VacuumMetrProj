#ifndef _METER_TASK_H_
#define _METER_TASK_H_
//-------------------Глобальные дейфайны--------------------------

#define FILTER_DATA_SIZE 100
#define MAX_FILT_DELAY 100

typedef enum{
  TWO_DIGITS   = 0,
  THREE_DIGITS = 1
}DIG_CNT;

typedef struct{
  uint16_t FILT_Delay;
  int16_t PDE1_Value;
  int16_t PDE2_Value;
  int16_t PDE3_Value;
  int16_t PDE4_Value;
  DIG_CNT Dig_CNT;
}FILT_Struct;

void METER_Task_init(void);
void METER_Task_Deinit(void);
void vMETER_Task(void *pvParameters);
void vFILTER_Task(void *pvParameters);

//------------------------------------------------
#endif /* _METER_TASK_H_ */
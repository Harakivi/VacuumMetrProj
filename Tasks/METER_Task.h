#ifndef _METER_TASK_H_
#define _METER_TASK_H_
//-------------------Глобальные дейфайны--------------------------

#define MAX_FILTER_DATA_SIZE 200
#define FILTER_DATA_INCR_SIZE 50

#define METER_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define METER_STACK_SIZE                (256)

#define FILTER_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define FILTER_STACK_SIZE                (256)

typedef enum{
  TWO_DIGITS   = 0,
  THREE_DIGITS = 1
}DIG_CNT;

typedef struct{
  //uint16_t FILT_Delay;
  int16_t PDE1_Value;
  int16_t PDE2_Value;
  int16_t PDE3_Value;
  int16_t PDE4_Value;
  int16_t* PDE1_Array;
  int16_t* PDE2_Array;
  int16_t* PDE3_Array;
  int16_t* PDE4_Array;
  uint8_t Data_CNT;
  DIG_CNT Dig_CNT;
}FILT_Struct;

void METER_Task_init(void);
void METER_Task_Deinit(void);
void vMETER_Task(void *pvParameters);
void vFILTER_Task(void *pvParameters);

//------------------------------------------------
#endif /* _METER_TASK_H_ */
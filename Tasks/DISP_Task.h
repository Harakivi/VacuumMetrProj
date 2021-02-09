#ifndef _DISP_TASK_H_
#define _DISP_TASK_H_
//-------------------Глобальные дейфайны--------------------------
//-------------------Инклюды--------------------------
#include "lcd5110.h"

void DISP_Task_init(void);
void vDISPUpdate_Task (void *pvParameters);
void DISP_Update(void);

//------------------------------------------------
#endif /* _DISP_TASK_H_ */
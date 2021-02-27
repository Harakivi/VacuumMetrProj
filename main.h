#ifndef _MAIN_
#define _MAIN_
//-------------------Глобальные дейфайны--------------------------

//------------------------------------------------
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "TaskPriority.h"
#include "BoardConfig.h"
#include "ADC.h"
#include "DISP_Buff.h"
#include "lcd5110.h"
#include "Configuration.h"
#include "Menu_Task.h"
#include "Buttons_Task.h"
#include "METER_Task.h"
#include "LASTLEVELMENU_Task.h"

/* -= Обязательно сначала иклюдим CMSIS, а потом FreeRTOS =- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"



//------------------------------------------------
#endif /* _MAIN_ */

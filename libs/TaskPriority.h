#ifndef _TASKPRIORITY_H_
#define _TASKPRIORITY_H_

#define METER_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define METER_STACK_SIZE                (128)

#define FILTER_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define FILTER_STACK_SIZE                (256)

#define BUTTONS_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define BUTTONS_STACK_SIZE                (32)

#define MENU_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define MENU_STACK_SIZE                (256)

#define LASTLEVELMENU_TASK_PRIORITY             (tskIDLE_PRIORITY + 3)
#define LASTLEVELMENU_STACK_SIZE                (512)

#endif /* _TASKPRIORITY_H_ */
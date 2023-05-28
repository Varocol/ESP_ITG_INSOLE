#ifndef __TASK_H
#define __TASK_H

#include "header.h"
#include "Hardware.h"

// 用于控制任务的进行和结束
typedef enum
{
    Running,
    Stop
} Task_Status;
typedef struct
{
    TaskFunction_t task;
    const char *name;
    volatile Task_Status status;
    int Priority;
} Task_Control_Block;

void LED_task(void *Parameter);
void Sleep_Timer_task(void *Parameter);
void Task_Init();

#endif // !__TASK_H

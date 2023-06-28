#ifndef __TASK_H
#define __TASK_H

#include "header.h"
#include "Hardware.h"

extern QueueHandle_t WS2812_control;
extern bool device_is_connect ;
extern bool bluetooth_is_connect ;
extern bool ready_to_sleep ;
extern bool Check_Sensor_OK;
extern unsigned int last_time;

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
    TaskHandle_t handle;
} Task_Control_Block;

void LED_task(void *Parameter);
void Sleep_Timer_task(void *Parameter);
void Task_Init();
void hardware_error();
void update_ble_data();
void set_ble_status(String status);

#endif // !__TASK_H

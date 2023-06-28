#ifndef __TASK_H
#define __TASK_H

#include "header.h"
#include "Hardware.h"

#define TASK_DELAY_TIME 1
#define TASK_DELAY delay(TASK_DELAY_TIME)
#define TASK_STACK_SIZE 10240
#define PARAM_ERR "Parameter error"
#define PARAM_EMPTY "Parameter empty"
#define TASK_NAME_ERR "Task Name error"
#define INFO_BUFF_SIZE 40

#ifdef INSOLE_DEBUG_MODE
void function_menu_Describe();
void Pressure1_GetData_Describe();
void Pressure1_Calibrate_Describe();
void Pressure2_GetData_Describe();
void Pressure2_Calibrate_Describe();
void Pressure3_GetData_Describe();
void Pressure3_Calibrate_Describe();
void Gyroscope_GetGyroRateDivisor_Describe();
void Gyroscope_SetGyroRateDivisor_Describe();
void Gyroscope_GetAccelRateDivisor_Describe();
void Gyroscope_SetAccelRateDivisor_Describe();
void Gyroscope_GetAccelRange_Describe();
void Gyroscope_SetAccelRange_Describe();
void Gyroscope_GetGyroRange_Describe();
void Gyroscope_SetGyroRange_Describe();
void Gyroscope_GetMagDataRate_Describe();
void Gyroscope_SetMagDataRate_Describe();
void Gyroscope_EnableAccelDLPF_Describe();
void Gyroscope_EnableGyrolDLPF_Describe();
void Gyroscope_GetData_Describe();
void clear_nvs_namespace_Describe();
void function_menu_Callback(void *);
void stop_task_Describe();
void show_task_Describe();
void data_json_Describe();

void Pressure1_GetData_Callback(void *);
void Pressure1_Calibrate_Callback(void *);
void Pressure2_GetData_Callback(void *);
void Pressure2_Calibrate_Callback(void *);
void Pressure3_GetData_Callback(void *);
void Pressure3_Calibrate_Callback(void *);
void Gyroscope_GetGyroRateDivisor_Callback(void *);
void Gyroscope_SetGyroRateDivisor_Callback(void *);
void Gyroscope_GetAccelRateDivisor_Callback(void *);
void Gyroscope_SetAccelRateDivisor_Callback(void *);
void Gyroscope_GetAccelRange_Callback(void *);
void Gyroscope_SetAccelRange_Callback(void *);
void Gyroscope_GetGyroRange_Callback(void *);
void Gyroscope_SetGyroRange_Callback(void *);
void Gyroscope_GetMagDataRate_Callback(void *);
void Gyroscope_SetMagDataRate_Callback(void *);
void Gyroscope_EnableAccelDLPF_Callback(void *);
void Gyroscope_EnableGyrolDLPF_Callback(void *);
void Gyroscope_GetData_Callback(void *);
void clear_nvs_namespace_Callback(void *);
void stop_task_Callback(void *);
void show_task_Callback(void *);
void data_json_Callback(void *);

void register_functions_tree();
void function_map(String user_input);
void function_map_task(void *param);
vector<String> param_separator(const String &param);
void menu();
void debug_init();
#endif

#endif // !__TASK_H

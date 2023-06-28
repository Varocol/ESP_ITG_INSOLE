#include "Task.h"

struct function_unit
{
    String func_name;
    void (*func_describe)();
    void (*func_callback)(void *);
    vector<String> params;
    volatile bool running;
};

std::map<String, function_unit> Functions_tree;

function_unit Function_unit[]
{
    {"menu",
     function_menu_Describe,
     function_menu_Callback},
#ifdef INSOLE_DEBUG_MODE
        {"Pressure1_GetData",
         Pressure1_GetData_Describe,
         Pressure1_GetData_Callback},
        {"Pressure1_Calibrate",
         Pressure1_Calibrate_Describe,
         Pressure1_Calibrate_Callback},
        {"Pressure2_GetData",
         Pressure2_GetData_Describe,
         Pressure2_GetData_Callback},
        {"Pressure2_Calibrate",
         Pressure2_Calibrate_Describe,
         Pressure2_Calibrate_Callback},
        {"Pressure3_GetData",
         Pressure3_GetData_Describe,
         Pressure3_GetData_Callback},
        {"Pressure3_Calibrate",
         Pressure3_Calibrate_Describe,
         Pressure3_Calibrate_Callback},
        {"Gyroscope_GetGyroRateDivisor",
         Gyroscope_GetGyroRateDivisor_Describe,
         Gyroscope_GetGyroRateDivisor_Callback},
        {"Gyroscope_SetGyroRateDivisor",
         Gyroscope_SetGyroRateDivisor_Describe,
         Gyroscope_SetGyroRateDivisor_Callback},
        {"Gyroscope_GetAccelRateDivisor",
         Gyroscope_GetAccelRateDivisor_Describe,
         Gyroscope_GetAccelRateDivisor_Callback},
        {"Gyroscope_SetAccelRateDivisor",
         Gyroscope_SetAccelRateDivisor_Describe,
         Gyroscope_SetAccelRateDivisor_Callback},
        {"Gyroscope_GetAccelRange",
         Gyroscope_GetAccelRange_Describe,
         Gyroscope_GetAccelRange_Callback},
        {"Gyroscope_SetAccelRange",
         Gyroscope_SetAccelRange_Describe,
         Gyroscope_SetAccelRange_Callback},
        {"Gyroscope_GetGyroRange",
         Gyroscope_GetGyroRange_Describe,
         Gyroscope_GetGyroRange_Callback},
        {"Gyroscope_SetGyroRange",
         Gyroscope_SetGyroRange_Describe,
         Gyroscope_SetGyroRange_Callback},
        {"Gyroscope_GetMagDataRate",
         Gyroscope_GetMagDataRate_Describe,
         Gyroscope_GetMagDataRate_Callback},
        {"Gyroscope_SetMagDataRate",
         Gyroscope_SetMagDataRate_Describe,
         Gyroscope_SetMagDataRate_Callback},
        {"Gyroscope_EnableAccelDLPF",
         Gyroscope_EnableAccelDLPF_Describe,
         Gyroscope_EnableAccelDLPF_Callback},
        {"Gyroscope_EnableGyrolDLPF",
         Gyroscope_EnableGyrolDLPF_Describe,
         Gyroscope_EnableGyrolDLPF_Callback},
        {"Gyroscope_GetData",
         Gyroscope_GetData_Describe,
         Gyroscope_GetData_Callback},
        {"clear_nvs_namespace",
         clear_nvs_namespace_Describe,
         clear_nvs_namespace_Callback},
#endif
        {"data_json",
         data_json_Describe,
         data_json_Callback},
        {"show_task",
         show_task_Describe,
         show_task_Callback},
        {"stop_task",
         stop_task_Describe,
         stop_task_Callback},
};

void register_functions_tree()
{
    for (function_unit el : Function_unit)
    {
        Functions_tree[el.func_name] = el;
    }
}

void function_map(String user_input)
{
    user_input.trim();
    if (user_input.isEmpty())
    {
        Message_Println(PARAM_EMPTY);
        return;
    }
    int index = user_input.indexOf('\n');
    if (index != -1)
    {
        function_map(user_input.substring(0, index));
        function_map(user_input.substring(index + 1));
    }
    else
    {
        vector<String> params = param_separator(user_input);
        // find the function unit
        auto f = Functions_tree.find(params.at(0));
        // if found
        if (f != Functions_tree.end())
        {
            // search task
            // TODO set configMAX_TASK_NAME_LEN in lib
            f->second.running = true;
            if (xTaskGetHandle(f->first.substring(0, configMAX_TASK_NAME_LEN - 1).c_str()) == NULL)
            {
                params.erase(params.begin());
                f->second.params = params;
                // create the task
                xTaskCreate(f->second.func_callback, f->first.c_str(), TASK_STACK_SIZE, (void *)&f->second.params, 2, NULL);
            }
            else
            {
                Message_Println("task existed");
            }
        }
        else
        {
            Message_Println("no command named " + params.at(0));
        }
    }
}

void function_map_task(void *param)
{
    String msg = *(String *)param;
    // Echo message
    Message_Println(msg);
    function_map(msg);
    delete (String *)param;
    vTaskDelete(NULL);
}

vector<String> param_separator(const String &param)
{
    String substr;
    vector<String> result;
    for (char el : param)
    {
        if (el != ' ' && el != '-')
        {
            substr += el;
        }
        else if (!substr.isEmpty())
        {
            result.push_back(substr);
            substr = "";
        }
    }
    if (!substr.isEmpty())
    {
        result.push_back(substr);
    }
    return result;
}

void function_menu_Describe()
{
    Message_Println(
        "menu\r\n"
        " list all functions and descriptions\r\n");
}

#ifdef INSOLE_DEBUG_MODE

void Pressure1_GetData_Describe()
{
    Message_Println(
        "Pressure1_GetData\r\n"
        " return pressure1 sensors' datas\r\n");
}

void Pressure1_Calibrate_Describe()
{
    Message_Println(
        "Pressure1_Calibrate\r\n"
        " calibrate pressure1 sensor\r\n");
}

void Pressure2_GetData_Describe()
{
    Message_Println(
        "Pressure2_GetData\r\n"
        " return pressure2 sensors' datas\r\n");
}

void Pressure2_Calibrate_Describe()
{
    Message_Println(
        "Pressure2_Calibrate\r\n"
        " calibrate pressure2 sensor\r\n");
}

void Pressure3_GetData_Describe()
{
    Message_Println(
        "Pressure3_GetData\r\n"
        " return pressure3 sensors' datas\r\n");
}

void Pressure3_Calibrate_Describe()
{
    Message_Println(
        "Pressure3_Calibrate\r\n"
        " calibrate pressure3 sensor\r\n");
}

void Gyroscope_GetGyroRateDivisor_Describe()
{
    Message_Println(
        "Gyroscope_GetGyroRateDivisor\r\n"
        " return value of gyro rate divisor\r\n");
}

void Gyroscope_SetGyroRateDivisor_Describe()
{
    Message_Println(
        "Gyroscope_SetGyroRateDivisor\r\n"
        "   -gyroratedivisor (1~256)\r\n"
        " set the gyro rate divisor\r\n");
}

void Gyroscope_GetAccelRateDivisor_Describe()
{
    Message_Println(
        "Gyroscope_GetAccelRateDivisor\r\n"
        " return value of accel rate divisor\r\n");
}

void Gyroscope_SetAccelRateDivisor_Describe()
{
    Message_Println(
        "Gyroscope_SetAccelRateDivisor\r\n"
        "   -accelratedivisor (1~4096)\r\n"
        " set the accel rate divisor\r\n");
}

void Gyroscope_GetAccelRange_Describe()
{
    Message_Println(
        "Gyroscope_GetAccelRange\r\n"
        " return the accel range\r\n");
}

void Gyroscope_SetAccelRange_Describe()
{
    Message_Println(
        "Gyroscope_SetAccelRange\r\n"
        "   -accelrange 2|4|8|16\r\n"
        " return accel range\r\n");
}

void Gyroscope_GetGyroRange_Describe()
{
    Message_Println(
        "Gyroscope_GetGyroRange\r\n"
        " return the gyro range\r\n");
}

void Gyroscope_SetGyroRange_Describe()
{
    Message_Println(
        "Gyroscope_SetGyroRange\r\n"
        "   -gyrorange 250|500|1000|2000\r\n"
        " set the gyro range\r\n");
}

void Gyroscope_GetMagDataRate_Describe()
{
    Message_Println(
        "Gyroscope_GetMagDataRate\r\n"
        " return the mag data rate\r\n");
}

void Gyroscope_SetMagDataRate_Describe()
{
    Message_Println(
        "Gyroscope_SetMagDataRate\r\n"
        "   -magdatarate SHUTDOWN|SINGLE|10|20|50|100\r\n"
        " set mag data rate\r\n");
}

void Gyroscope_EnableAccelDLPF_Describe()
{
    Message_Println(
        "Gyroscope_EnableAccelDLPF\r\n"
        "   -enable ENABLE|DISABLE\r\n"
        "   -cutoff_freq 437|246|111.4|50.4|23.9|11.5|5.7\r\n"
        " enable the accel DLPF(low pass filter)\r\n");
}

void Gyroscope_EnableGyrolDLPF_Describe()
{
    Message_Println(
        "Gyroscope_EnableGyrolDLPF\r\n"
        "  -enable enable|disable\r\n"
        "  -cutoff_freq 361.4|196.6|151.8|119.5|51.2|23.9|11.6|5.7\r\n"
        " enable the gyrol DLPF(low pass filter)\r\n");
}

void Gyroscope_GetData_Describe()
{
    Message_Println(
        "Gyroscope_GetData\r\n"
        " return the gyroscope datas\r\n");
}

void clear_nvs_namespace_Describe()
{
    Message_Println(
        "clear_nvs_namespace\r\n"
        " clear the nvs space\r\n");
}

#endif

void data_json_Describe()
{
    Message_Println(
        "data_json\r\n"
        " return the json consist of the data from all the sensors\r\n");
}

void stop_task_Describe()
{
    Message_Println(
        "stop_task\r\n"
        " -name (task name)\r\n"
        " stop task that existed\r\n");
}

void show_task_Describe()
{
    Message_Println(
        "show_task\r\n"
        " stow task that existed\r\n");
}

void function_menu_Callback(void *param)
{
    menu();
    vTaskDelete(NULL);
}

#ifdef INSOLE_DEBUG_MODE
void Pressure1_GetData_Callback(void *param)
{
    stringstream str;
    auto el = Functions_tree.find("Pressure1_GetData");
    // run the task
    while (el->second.running)
    {
        Message_Println(str.str().c_str());
        str.clear();
        str.str("");
        str << Pressure_GetData(PRESSURE1).c_str();
        TASK_DELAY;
    }
    vTaskDelete(NULL);
}

void Pressure1_Calibrate_Callback(void *param)
{
    Message_Println(Pressure_Calibrate(PRESSURE1));
    vTaskDelete(NULL);
}

void Pressure2_GetData_Callback(void *param)
{
    stringstream str;
    auto el = Functions_tree.find("Pressure2_GetData");
    // run the task
    while (el->second.running)
    {
        Message_Println(str.str().c_str());
        str.clear();
        str.str("");
        str << Pressure_GetData(PRESSURE2).c_str();
        TASK_DELAY;
    }
    vTaskDelete(NULL);
}

void Pressure2_Calibrate_Callback(void *param)
{
    Message_Println(Pressure_Calibrate(PRESSURE2));
    vTaskDelete(NULL);
}

void Pressure3_GetData_Callback(void *param)
{
    stringstream str;
    auto el = Functions_tree.find("Pressure3_GetData");
    // run the task
    while (el->second.running)
    {
        Message_Println(str.str().c_str());
        str.clear();
        str.str("");
        str << Pressure_GetData(PRESSURE3).c_str();
        TASK_DELAY;
    }
    vTaskDelete(NULL);
}

void Pressure3_Calibrate_Callback(void *param)
{
    Message_Println(Pressure_Calibrate(PRESSURE3));
    vTaskDelete(NULL);
}

void Gyroscope_GetGyroRateDivisor_Callback(void *param)
{
    Message_Println(Gyroscope_GetGyroRateDivisor());
    vTaskDelete(NULL);
}

void Gyroscope_SetGyroRateDivisor_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }

    if (Param->at(0) == "gyroratedivisor")
    {
        Param->erase(Param->begin());
    }

    int gyro_divisor = Param->at(0).toInt() - 1;
    if (gyro_divisor < 0 || gyro_divisor > 255)
    {
        Message_Println(PARAM_ERR);
    }
    else
    {
        Message_Println(Gyroscope_SetGyroRateDivisor(gyro_divisor));
    }
    vTaskDelete(NULL);
}

void Gyroscope_GetAccelRateDivisor_Callback(void *param)
{
    Message_Println(Gyroscope_GetAccelRateDivisor());
    vTaskDelete(NULL);
}

void Gyroscope_SetAccelRateDivisor_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }

    if (Param->at(0) == "accelratedivisor")
    {
        Param->erase(Param->begin());
    }

    int accel_divisor = Param->at(0).toInt() - 1;
    if (accel_divisor < 0 || accel_divisor > 4095)
    {
        Message_Println(PARAM_ERR);
    }
    else
    {
        Message_Println(Gyroscope_SetAccelRateDivisor(accel_divisor));
    }
    vTaskDelete(NULL);
}

void Gyroscope_GetAccelRange_Callback(void *param)
{
    Message_Println(Gyroscope_GetAccelRange());
    vTaskDelete(NULL);
}

void Gyroscope_SetAccelRange_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }

    if (Param->at(0) == "accelrange")
    {
        Param->erase(Param->begin());
    }

    icm20948_accel_range_t accel_range;
    switch (Param->at(0).toInt())
    {
    case 2:
        accel_range = ICM20948_ACCEL_RANGE_2_G;
        break;
    case 4:
        accel_range = ICM20948_ACCEL_RANGE_4_G;
        break;
    case 8:
        accel_range = ICM20948_ACCEL_RANGE_8_G;
        break;
    case 16:
        accel_range = ICM20948_ACCEL_RANGE_16_G;
        break;
    default:
        Message_Println(PARAM_ERR);
        vTaskDelete(NULL);
        return;
        break;
    }
    Message_Println(Gyroscope_SetAccelRange(accel_range));
    vTaskDelete(NULL);
}

void Gyroscope_GetGyroRange_Callback(void *param)
{
    Message_Println(Gyroscope_GetGyroRange());
    vTaskDelete(NULL);
}

void Gyroscope_SetGyroRange_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }

    if (Param->at(0) == "gyrorange")
    {
        Param->erase(Param->begin());
    }

    icm20948_gyro_range_t gyro_range;
    switch (Param->at(0).toInt())
    {
    case 250:
        gyro_range = ICM20948_GYRO_RANGE_250_DPS;
        break;
    case 500:
        gyro_range = ICM20948_GYRO_RANGE_500_DPS;
        break;
    case 1000:
        gyro_range = ICM20948_GYRO_RANGE_1000_DPS;
        break;
    case 2000:
        gyro_range = ICM20948_GYRO_RANGE_2000_DPS;
        break;
    default:
        Message_Println(PARAM_ERR);
        vTaskDelete(NULL);
        return;
        break;
    }
    Message_Println(Gyroscope_SetGyroRange(gyro_range));
    vTaskDelete(NULL);
}

void Gyroscope_GetMagDataRate_Callback(void *param)
{
    Message_Println(Gyroscope_GetMagDataRate());
    vTaskDelete(NULL);
}

void Gyroscope_SetMagDataRate_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }

    if (Param->at(0) == "magdatarate")
    {
        Param->erase(Param->begin());
    }

    ak09916_data_rate_t rate;
    if (Param->at(0) == "SHUTDOWN")
    {
        rate = AK09916_MAG_DATARATE_SHUTDOWN;
    }
    else if (Param->at(0) == "SINGLE")
    {
        rate = AK09916_MAG_DATARATE_SINGLE;
    }
    else
    {
        switch (Param->at(0).toInt())
        {
        case 10:
            rate = AK09916_MAG_DATARATE_10_HZ;
            break;
        case 20:
            rate = AK09916_MAG_DATARATE_20_HZ;
            break;
        case 50:
            rate = AK09916_MAG_DATARATE_50_HZ;
            break;
        case 100:
            rate = AK09916_MAG_DATARATE_100_HZ;
            break;
        default:
            Message_Println(PARAM_ERR);
            vTaskDelete(NULL);
            return;
            break;
        }
    }
    Message_Println(Gyroscope_SetMagDataRate(rate));
    vTaskDelete(NULL);
}

void Gyroscope_EnableAccelDLPF_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }

    if (Param->at(0) == "enable")
    {
        Param->erase(Param->begin());
    }

    bool enable;
    if (Param->at(0) == "ENABLE")
    {
        enable = true;
        Param->erase(Param->begin());
    }
    else if (Param->at(0) == "DISABLE")
    {
        enable = false;
        Param->erase(Param->begin());
    }
    else
    {
        Message_Println(PARAM_ERR);
        vTaskDelete(NULL);
        return;
    }

    icm20x_accel_cutoff_t cutoff_freq = ICM20X_ACCEL_FREQ_5_7_HZ;

    if (Param->at(0) == "cutoff_freq")
    {
        Param->erase(Param->begin());
    }

    double freq = Param->at(0).toDouble();
    if (freq == 437.0)
    {
        cutoff_freq = ICM20X_ACCEL_FREQ_473_HZ;
    }
    else if (freq == 246.0)
    {
        cutoff_freq = ICM20X_ACCEL_FREQ_246_0_HZ;
    }
    else if (freq == 111.4)
    {
        cutoff_freq = ICM20X_ACCEL_FREQ_111_4_HZ;
    }
    else if (freq == 50.4)
    {
        cutoff_freq = ICM20X_ACCEL_FREQ_50_4_HZ;
    }
    else if (freq == 23.9)
    {
        cutoff_freq = ICM20X_ACCEL_FREQ_23_9_HZ;
    }
    else if (freq == 11.5)
    {
        cutoff_freq = ICM20X_ACCEL_FREQ_11_5_HZ;
    }
    else if (freq == 5.7)
    {
        cutoff_freq = ICM20X_ACCEL_FREQ_5_7_HZ;
    }
    else if (enable)
    {
        Message_Println(PARAM_ERR);
        vTaskDelete(NULL);
        return;
    }
    Message_Println(Gyroscope_EnableAccelDLPF(enable, cutoff_freq));
    vTaskDelete(NULL);
}

void Gyroscope_EnableGyrolDLPF_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }

    if (Param->at(0) == "enable")
    {
        Param->erase(Param->begin());
    }

    bool enable;
    if (Param->at(0) == "ENABLE")
    {
        enable = true;
        Param->erase(Param->begin());
    }
    else if (Param->at(0) == "DISABLE")
    {
        enable = false;
        Param->erase(Param->begin());
    }
    else
    {
        Message_Println(PARAM_ERR);
        vTaskDelete(NULL);
        return;
    }

    icm20x_gyro_cutoff_t cutoff_freq = ICM20X_GYRO_FREQ_5_7_HZ;

    if (Param->at(0) == "cutoff_freq")
    {
        Param->erase(Param->begin());
    }

    double freq = Param->at(0).toDouble();
    if (freq == 361.4)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_361_4_HZ;
    }
    else if (freq == 196.6)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_196_6_HZ;
    }
    else if (freq == 151.8)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_151_8_HZ;
    }
    else if (freq == 119.5)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_119_5_HZ;
    }
    else if (freq == 51.2)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_51_2_HZ;
    }
    else if (freq == 23.9)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_23_9_HZ;
    }
    else if (freq == 11.6)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_11_6_HZ;
    }
    else if (freq == 5.7)
    {
        cutoff_freq = ICM20X_GYRO_FREQ_5_7_HZ;
    }
    else if (enable)
    {
        Message_Println(PARAM_ERR);
        vTaskDelete(NULL);
        return;
    }
    Message_Println(Gyroscope_EnableGyrolDLPF(enable, cutoff_freq));
    vTaskDelete(NULL);
}

void Gyroscope_GetData_Callback(void *param)
{
    stringstream str;
    auto el = Functions_tree.find("Gyroscope_GetData");
    // run the task
    while (el->second.running)
    {
        Message_Println(str.str().c_str());
        str.clear();
        str.str("");
        str << Gyroscope_GetData().c_str();
        TASK_DELAY;
    }
    vTaskDelete(NULL);
}

void clear_nvs_namespace_Callback(void *param)
{
    Message_Println(clear_nvs_namespace());
    vTaskDelete(NULL);
}
#endif

void data_json_Callback(void *param)
{
    stringstream str;
    auto el = Functions_tree.find("data_json");
    // run the task
    while (el->second.running)
    {
        Message_Println(str.str().c_str());
        str.clear();
        str.str("");
        str << data_json().c_str();
        TASK_DELAY;
    }
    vTaskDelete(NULL);
}

void stop_task_Callback(void *param)
{
    vector<String> *Param = (vector<String> *)param;
    if (Param->empty())
    {
        Message_Println(PARAM_EMPTY);
        vTaskDelete(NULL);
        return;
    }
    if (Param->at(0) == "name")
    {
        Param->erase(Param->begin());
    }
    stringstream str;
    if (!Param->empty())
    {
        auto f = Functions_tree.find(Param->at(0));
        // if found
        if (f != Functions_tree.end() && f->second.running)
        {
            // can't use the default delete function, cause the mutex cannot be realeased
            // vTaskDelete(handle);
            f->second.running = false;
            str << "Delete Success";
        }
        else
        {
            str << TASK_NAME_ERR;
        }
    }
    else
    {
        str << TASK_NAME_ERR;
    }

    Message_Println(str.str().c_str());
    vTaskDelete(NULL);
}

void show_task_Callback(void *param)
{
    // TODO fix the lib
    //  stringstream str;
    //  char *tasklistBuffer;
    //  char *runtimeBuffer;
    //  volatile UBaseType_t uxArraySize;
    //  uxArraySize = uxTaskGetNumberOfTasks();
    //  tasklistBuffer = (char *)pvPortMalloc(uxArraySize * INFO_BUFF_SIZE);
    //  runtimeBuffer = (char *)pvPortMalloc(uxArraySize * INFO_BUFF_SIZE);
    //  vTaskList(tasklistBuffer);
    //  vTaskGetRunTimeStats(runtimeBuffer);
    //  str << "=================================================\r\n"
    //      << "Name        Status Priority  Stack  Num \r\n"
    //      << "=================================================\r\n"
    //      << tasklistBuffer << "\r\n"
    //      << "  B:Block  R:Ready  D:Deleted  S:Suspend  X:Running \r\n"
    //      << "=================================================\r\n"
    //      << "=================================================\r\n"
    //      << "Name         RunCount        CPU \r\n"
    //      << "=================================================\r\n"
    //      << runtimeBuffer << "\r\n"
    //      << "=================================================\r\n"
    //      << "=================================================\r\n\n\n";
    //  Message_Println(str.str().c_str());
    //  vPortFree(tasklistBuffer);
    //  vPortFree(runtimeBuffer);
    vTaskDelete(NULL);
}

void menu()
{
    for (function_unit el : Function_unit)
    {
        el.func_describe();
    }
}

void debug_init()
{
    register_functions_tree();
    menu();
    // no need to call vTaskStartScheduler
    // vTaskStartScheduler();
}

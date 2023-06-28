#include "Task.h"

QueueHandle_t WS2812_control;

// 任务列表
vector<Task_Control_Block> Task_List{
    // {Load_Track_Task, "Load_Track", Running, 1},
    // {Servo_Calibrate_Task, "Servo_Calibrate", Running, 2},
    {LED_task, "LED_task", Running, 1}

};

TimerHandle_t Sleep_Timer;
bool device_is_connect = false;
bool bluetooth_is_connect = false;
bool ready_to_sleep = false;
bool Check_Sensor_OK = false;
unsigned int last_time;

void LED_task(void *param)
{
    WS2812_control = xQueueCreate(5, sizeof(WS2812_Blink_typedef));
    if (WS2812_control == NULL)
    {
        log_e("WS2812_control create failed!");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
    WS2812_Blink_typedef led_mode;
    led_mode.WS2812_Blink_Mode = Breathe;
    led_mode.r = 195;
    led_mode.g = 0;
    led_mode.b = 255;
    xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);

    Task_Control_Block *TCB = (Task_Control_Block *)param;
    while (TCB->status == Running)
    {
        WS2812_Blink_typedef element;
        if (xQueueReceive(WS2812_control, &element, portMAX_DELAY))
        {
            LED_blink(element);
            delay(300);
        }
        delay(1);
    }
    vTaskDelete(NULL);
}

void Sleep_Timer_task(void *Parameter)
{
    if (bluetooth_is_connect)
    {
        ready_to_sleep = false;
        return;
    }
    if (device_is_connect)
    {
        ready_to_sleep = false;
        return;
    }
    if (ready_to_sleep)
    {
        log_i("Go to sleep!");
        // 停止所有线程
        for (int i = 0; i < Task_List.size(); i++)
        {
            Task_List[i].status = Stop;
            vTaskDelete(Task_List[i].handle);
        }
        close_LED();
        // GPIO0-GPIO3
        const uint64_t WAKEUP_HIGH_PIN_BITMASK = 0b001111;
        for (int i = 0; i < 6; i++)
        {
            gpio_set_direction(gpio_num_t(i), GPIO_MODE_INPUT);
        }
        // 保持禁用、隔离和电源域配置功能可能没有必要
        gpio_deep_sleep_hold_dis();
        esp_sleep_config_gpio_isolate();
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
        // 低位和高位唤醒可以同时用于单独的引脚
        esp_deep_sleep_enable_gpio_wakeup(WAKEUP_HIGH_PIN_BITMASK, ESP_GPIO_WAKEUP_GPIO_LOW);
        esp_deep_sleep_start();
    }
    else
    {
        log_i("Not connect bluetooth and device, ready to sleep!");
        ready_to_sleep = true;
    }
}

/**
 * @brief  任务初始化
 * @param  None
 * @retval None
 */
void Task_Init()
{
    // 初始化任务列表中的任务
    for (int i = 0; i < Task_List.size(); i++)
    {
        log_i("Create Task %s", Task_List.at(i).name);
        xTaskCreate(Task_List.at(i).task, Task_List.at(i).name, 2048, (void *)&Task_List.at(i), Task_List.at(i).Priority, &Task_List.at(i).handle);
    }
    Sleep_Timer = xTimerCreate("Sleep_Timer",            /*任务名字*/
                               60000 / portTICK_RATE_MS, /*设置时钟周期:60000ms = 60s*/
                               pdTRUE,                   /*pdTRUE周期调用,pdFALSE:单次调用*/
                               (void *)2,                /*计时器优先级*/
                               Sleep_Timer_task);        /*定时回调函数*/
    xTimerStart(Sleep_Timer, 0);                         // 打开定时器
}

void hardware_error()
{
    Check_Sensor_OK = false;
    if (millis() - last_time > 8000)
    {
        // 每隔3秒闪烁一次LED（红色）
        WS2812_Blink_typedef led_mode;
        led_mode.WS2812_Blink_Mode = Double_flashing;
        led_mode.r = 255;
        led_mode.g = 0;
        led_mode.b = 0;
        xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
        last_time = millis();
    }
}

void set_ble_status(String status)
{
    if (!Status_sensor_Characteristic)
    {
        return;
    }
    Status_sensor_Characteristic->setValue(status);
}

void update_ble_data()
{
    if (!bluetooth_is_connect)
    {
        return;
    }

    set_ble_status("READY");

    char buff[20] = {0};
    Gyroscope_Packet gyroscope_packet = gyroscope_getData();
    sprintf(buff, "%.2f|%.2f|%.2f", gyroscope_packet.gyro.gyro.x, gyroscope_packet.gyro.gyro.y, gyroscope_packet.gyro.gyro.z);
    Gyro_sensor_Characteristic->setValue(buff);

    char buff5[20] = {0};
    gyroscope_packet = gyroscope_getData();
    sprintf(buff5, "%.2f|%.2f|%.2f", gyroscope_packet.accel.acceleration.x, gyroscope_packet.accel.acceleration.y, gyroscope_packet.accel.acceleration.z);
    Accel_sensor_Characteristic->setValue(buff5);

    char buff6[20] = {0};
    gyroscope_packet = gyroscope_getData();
    sprintf(buff6, "%.2f|%.2f|%.2f", gyroscope_packet.mag.magnetic.x, gyroscope_packet.mag.magnetic.y, gyroscope_packet.mag.magnetic.z);
    Magn_sensor_Characteristic->setValue(buff6);

    char buff7[20] = {0};
    gyroscope_packet = gyroscope_getData();
    sprintf(buff7, "%f", gyroscope_packet.temp.temperature);
    Temp_sensor_Characteristic->setValue(buff7);

    char buff2[20] = {0};
    PRESSURE_STATUS_CODE status;
    Pressure_Packet pressure_packet;
    uint16_t benchmark_Val[CHANNEL_NUM];
    if ((status = pressure1.getCalibrateVal(benchmark_Val)) >= 0)
    {
        if ((status = pressure1.getData(pressure_packet)) >= 0)
        {
            for (int i = 0; i < CHANNEL_NUM; i++)
            {
                buff2[i] = highByte(pressure_packet.datas.at(i)) - highByte(benchmark_Val[i]);
            }
        }
        else
        {
            log_e("%s", pressure1.message(status).c_str());
        }
    }
    else
    {
        log_e("%s", pressure1.message(status).c_str());
    }

    PRESS1_sensor_Characteristic->setValue(buff2);

    char buff3[20] = {0};
    PRESSURE_STATUS_CODE status2;
    Pressure_Packet pressure_packet2;
    uint16_t benchmark_Val2[CHANNEL_NUM];
    if ((status2 = pressure1.getCalibrateVal(benchmark_Val2)) >= 0)
    {
        if ((status2 = pressure1.getData(pressure_packet2)) >= 0)
        {
            for (int i = 0; i < CHANNEL_NUM; i++)
            {
                buff3[i] = highByte(pressure_packet2.datas.at(i)) - highByte(benchmark_Val2[i]);
            }
        }
        else
        {
            log_e("%s", pressure1.message(status2).c_str());
        }
    }
    else
    {
        log_e("%s", pressure1.message(status2).c_str());
    }

    PRESS2_sensor_Characteristic->setValue(buff3);

    char buff4[20] = {0};
    PRESSURE_STATUS_CODE status3;
    Pressure_Packet pressure_packet3;
    uint16_t benchmark_Val3[CHANNEL_NUM];
    if ((status3 = pressure1.getCalibrateVal(benchmark_Val3)) >= 0)
    {
        if ((status3 = pressure1.getData(pressure_packet3)) >= 0)
        {
            for (int i = 0; i < CHANNEL_NUM; i++)
            {
                buff4[i] = highByte(pressure_packet3.datas.at(i)) - highByte(benchmark_Val3[i]);
            }
        }
        else
        {
            log_e("%s", pressure1.message(status3).c_str());
        }
    }
    else
    {
        log_e("%s", pressure1.message(status3).c_str());
    }

    PRESS3_sensor_Characteristic->setValue(buff4);
}
#include "Task.h"

QueueHandle_t WS2812_control;

// 任务列表
vector<Task_Control_Block> Task_List{
    // {Load_Track_Task, "Load_Track", Running, 1},
    // {Servo_Calibrate_Task, "Servo_Calibrate", Running, 2},
    {LED_task, "LED_task", Running, 1}

};


//{mod type ,r,g,b}

TimerHandle_t Sleep_Timer;
bool device_is_connect = false;
bool bluetooth_is_connect = false;
bool ready_to_sleep = false;


void LED_task(void *param){
    WS2812_control = xQueueCreate(5, sizeof(WS2812_Blink_typedef));
    if(WS2812_control == NULL){
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
        if(xQueueReceive(WS2812_control, &element, portMAX_DELAY)){
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
        WS2812_Blink_typedef led_mode;
        led_mode.WS2812_Blink_Mode = Close;
        xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
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
        xTaskCreate(Task_List.at(i).task, Task_List.at(i).name, 2048, (void *)&Task_List.at(i), Task_List.at(i).Priority, NULL);
    }
    Sleep_Timer = xTimerCreate("Sleep_Timer",            /*任务名字*/
                               10000 / portTICK_RATE_MS, /*设置时钟周期:1000ms = 10s*/
                               pdTRUE,                   /*pdTRUE周期调用,pdFALSE:单次调用*/
                               (void *)2,                /*计时器优先级*/
                               Sleep_Timer_task);        /*定时回调函数*/
    xTimerStart(Sleep_Timer, 0);                         // 打开定时器
}

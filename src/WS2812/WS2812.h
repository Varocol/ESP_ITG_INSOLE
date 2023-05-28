#ifndef __WS2812_H
#define __WS2812_H

#include "header.h"
#include "Hardware.h"

typedef enum
{
    Breathe,            // 呼吸灯
    One_flashing,       // 单闪
    Double_flashing,    // 双闪
    Bright,             // 常亮
    Close,             // 关闭
} WS2812_Blink_Mode_enum;
typedef struct
{
    WS2812_Blink_Mode_enum WS2812_Blink_Mode;
    u8 r;
    u8 g;
    u8 b;
} WS2812_Blink_typedef;

extern Freenove_ESP32_WS2812 WS2812;
void init_LED();
void LED_blink(WS2812_Blink_typedef element);

#endif // !__WS2812_H

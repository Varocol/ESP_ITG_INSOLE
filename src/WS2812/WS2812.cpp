#include "WS2812.h"

Freenove_ESP32_WS2812 WS2812 = Freenove_ESP32_WS2812(1, WS2812_PIN, 0, TYPE_GRB);

void init_LED()
{
    WS2812.begin();
}

void close_LED()
{
    WS2812.setBrightness(0);
    WS2812.setLedColorData(0, 0, 0, 0);
    WS2812.show();
}

void LED_blink(WS2812_Blink_typedef element)
{
    switch (element.WS2812_Blink_Mode)
    {
    case Breathe:
    {
        log_i("Show LED Mod Breathe, R:%d G:%d B:%d", element.r, element.g, element.b);
        for (u8 i = 0; i < 255; i++)
        {
            WS2812.setBrightness(i);
            WS2812.setLedColorData(0, element.r, element.g, element.b);
            WS2812.show();
            delay(2);
        }
        for (u8 i = 254; i > 0; i--)
        {
            WS2812.setBrightness(i);
            WS2812.setLedColorData(0, element.r, element.g, element.b);
            WS2812.show();
            delay(1);
        }
        WS2812.setLedColorData(0, 0, 0, 0);
        WS2812.show();
    }
    break;
    case One_flashing:
    {
        log_i("Show LED Mod One_flashing, R:%d G:%d B:%d", element.r, element.g, element.b);
        WS2812.setBrightness(254);
        WS2812.setLedColorData(0, element.r, element.g, element.b);
        WS2812.show();
        delay(100);
        WS2812.setLedColorData(0, 0, 0, 0);
        WS2812.show();
    }
    break;
    case Double_flashing:
    {
        log_i("Show LED Mod Double_flashing, R:%d G:%d B:%d", element.r, element.g, element.b);
        WS2812.setBrightness(254);
        WS2812.setLedColorData(0, element.r, element.g, element.b);
        WS2812.show();
        delay(70);
        WS2812.setLedColorData(0, 0, 0, 0);
        WS2812.show();
        delay(70);
        WS2812.setLedColorData(0, element.r, element.g, element.b);
        WS2812.show();
        delay(70);
        WS2812.setLedColorData(0, 0, 0, 0);
        WS2812.show();
    }
    break;
    case Bright:
    {
        log_i("Show LED Mod Bright, R:%d G:%d B:%d", element.r, element.g, element.b);
        WS2812.setBrightness(254);
        WS2812.setLedColorData(0, element.r, element.g, element.b);
        WS2812.show();
    }
    break;
    case Close:
    {
        log_i("Show LED Mod Close");
        WS2812.setBrightness(0);
        WS2812.setLedColorData(0, 0, 0, 0);
        WS2812.show();
    }
    break;
    default:
        break;
    }
}
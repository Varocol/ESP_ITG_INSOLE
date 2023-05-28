#include "Hardware.h"
#include "Task.h"

// // TimerHandle_t Sleep_Timer;
// // bool device_is_connect = false;
// // bool bluetooth_is_connect = false;

// void setup()
// {
//   // if (Hardware_Init())
//   // {
//   //   Task_Init();
//   // }

//   // Serial_Init();
//   // bluetooth_init();
//   // Wire_Init();
// }

// void loop()
// {

//   // if (Check_Sensor())
//   // {
//   //   // 外部设备连接正常
//   //   if (certification)
//   //   {
//   //     // 蓝牙认证通过

//   //     // 陀螺仪数据：2|X|Y|Z|Temp
//   //     Gyroscope_Packet gyroscope_packet;
//   //     gyroscope_packet = gyroscope_getData();
//   //     String all_buff = "2";
//   //     String buffer = "";
//   //     char *byte = (char *)&(gyroscope_packet.gyro.gyro.x);
//   //     for (int i = 0; i < sizeof(gyroscope_packet.gyro.gyro.x); i++, byte++)
//   //     {
//   //       buffer = String((uint8_t)*byte, 16) + buffer;
//   //     }
//   //     all_buff = all_buff + buffer;

//   //     buffer = "";
//   //     char *byte_2 = (char *)&(gyroscope_packet.gyro.gyro.y);
//   //     for (int i = 0; i < sizeof(gyroscope_packet.gyro.gyro.y); i++, byte_2++)
//   //     {
//   //       buffer = String((uint8_t)*byte_2, 16) + buffer;
//   //     }
//   //     all_buff = all_buff + buffer;

//   //     buffer = "";
//   //     char *byte_3 = (char *)&(gyroscope_packet.gyro.gyro.z);
//   //     for (int i = 0; i < sizeof(gyroscope_packet.gyro.gyro.z); i++, byte_3++)
//   //     {
//   //       buffer = String((uint8_t)*byte_3, 16) + buffer;
//   //     }
//   //     all_buff = all_buff + buffer;

//   //     buffer = "";
//   //     char *byte_4 = (char *)&(gyroscope_packet.gyro.temperature);
//   //     for (int i = 0; i < sizeof(gyroscope_packet.gyro.temperature); i++, byte_4++)
//   //     {
//   //       buffer = String((uint8_t)*byte_4, 16) + buffer;
//   //     }
//   //     all_buff = all_buff + buffer;
//   //     bluetooth_sendmessage(all_buff);

//   //   }
//   // }
//   // else
//   // {
//   //   log_e("Check_Sensor error");
//   //   // 很鲜明这里是无法检测到传感器
//   // }
// }

// #include "Hardware.h"
// #include <esp_sleep.h>

// 鞋垫插上去 INT会
void setup()
{
  Hardware_Init();
  Task_Init();
}

void loop()
{
  delay(1000);
}

// #include "Freenove_WS2812_Lib_for_ESP32.h"

// #define LEDS_COUNT  8
// #define LEDS_PIN	6
// #define CHANNEL		0

// Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

// void setup() {
//   strip.begin();
//   strip.setBrightness(20);
// }

// void loop() {
//   for (int j = 0; j < 255; j += 2) {
//     for (int i = 0; i < LEDS_COUNT; i++) {
//       strip.setLedColorData(i, strip.Wheel((i * 256 / LEDS_COUNT + j) & 255));
//     }
//     strip.show();
//     delay(10);
//   }
// }




// #include <FastLED.h>

// // How many leds in your strip?
// #define NUM_LEDS 1

// // For led chips like WS2812, which have a data line, ground, and power, you just
// // need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// // ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// // Clock pin only needed for SPI based chipsets when not using hardware SPI
// #define DATA_PIN 6

// // Define the array of leds
// CRGB leds[NUM_LEDS];

// void setup() { 
//     // Uncomment/edit one of the following lines for your leds arrangement.
//     // ## Clockless types ##
//     // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
//     // FastLED.addLeds<SM16703, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<TM1812, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<UCS1904, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<UCS2903, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
//     // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
//     FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
//     // FastLED.addLeds<GS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
//     // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<GE8822, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<LPD1886, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<LPD1886_8BIT, DATA_PIN, RGB>(leds, NUM_LEDS);
//     // ## Clocked (SPI) types ##
//     // FastLED.addLeds<LPD6803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
//     // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
//     // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<WS2803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
//     // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
//     // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
//     // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
//     // FastLED.addLeds<SK9822, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
// }

// void loop() { 
//   // Turn the LED on, then pause
//   leds[0] = CRGB::Red;
//   FastLED.show();
//   delay(500);
//   // Now turn the LED off, then pause
//   leds[0] = CRGB::Black;
//   FastLED.show();
//   delay(500);
// }


// #include "Freenove_WS2812_Lib_for_ESP32.h"

// #define WS2812_PIN	6

// Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(1, WS2812_PIN, 0, TYPE_GRB);

// u8 m_color[5][3] = { {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}, {0, 0, 0} };
// int delayval = 100;

// void setup() {
// 	strip.begin();
// 	strip.setBrightness(10);	
// }
// void loop() {
// 	for (int j = 0; j < 5; j++) {
//     strip.setLedColorData(0, m_color[j][0], m_color[j][1], m_color[j][2]);
//     strip.show();
//     delay(delayval);
// 		delay(500);
// 	}
// }
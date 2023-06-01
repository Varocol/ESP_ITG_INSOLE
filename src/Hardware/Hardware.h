#ifndef __HARDWARD_H
#define __HARDWARD_H

#include "header.h"
#include "Task.h"
#include "Gyroscope.h"
#include "Pressure.h"
#include "Bluetooth.h"
#include "WS2812.h"


#define SERIAL_BAUDRATE 115200
#define SDA_PIN 7
#define SCL_PIN 8
#define I2C_BAUDRATE 1000000UL

#define WS2812_PIN 6


// Hardware Initialization
void Wire_Init();
bool Pressure_Init();
bool Gyroscope_Init();
void Hardware_Init();
bool Check_Sensor();
void check_Pressure_Calibrate();
// Serial_Callback
// Output Interface

void Message_Println(const String &msg);
void Message_Printf(const String &msg);

extern bool certification;
String data_json();
#endif // !__HARDWARD_H

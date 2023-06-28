#ifndef __HARDWARD_H
#define __HARDWARD_H

#include "header.h"
#include "Task.h"
#include "Gyroscope.h"
#include "Pressure.h"
#include "Bluetooth.h"

#define SERIAL_BAUDRATE 115200
#define SDA_PIN 12
#define SCL_PIN 1
#define I2C_BAUDRATE 1000000UL
#define INT_PIN 18

// Hardware Initialization
void Serial_Init();
void Bluetooth_Init();
bool Wire_Init();
bool Pressure_Init();
bool Gyroscope_Init();
bool Hardware_Init();

// Serial_Callback
void Serial_Callback(void);

// Output Interface
void Message_Println(const String &msg);
void Message_Printf(const String &msg);

// Outside Interrupt
void Interrupt_Init();

#ifdef INSOLE_DEBUG_MODE
// Debug Function
String Pressure_GetData(const Pressure_Id &pressure_id);
String Pressure_Calibrate(const Pressure_Id &pressure_id);
String Gyroscope_GetGyroRateDivisor();
String Gyroscope_SetGyroRateDivisor(const uint8_t &new_gyro_divisor);
String Gyroscope_GetAccelRateDivisor();
String Gyroscope_SetAccelRateDivisor(const uint16_t &new_accel_divisor);
String Gyroscope_GetAccelRange();
String Gyroscope_SetAccelRange(const icm20948_accel_range_t &new_accel_range);
String Gyroscope_GetGyroRange();
String Gyroscope_SetGyroRange(const icm20948_gyro_range_t &new_gyro_range);
String Gyroscope_GetMagDataRate();
String Gyroscope_SetMagDataRate(const ak09916_data_rate_t &rate);
String Gyroscope_EnableAccelDLPF(const bool &enable, const icm20x_accel_cutoff_t &cutoff_freq);
String Gyroscope_EnableGyrolDLPF(const bool &enable, const icm20x_gyro_cutoff_t &cutoff_freq);
String Gyroscope_GetData();
String clear_nvs_namespace();
#endif

String data_json();
#endif // !__HARDWARD_H

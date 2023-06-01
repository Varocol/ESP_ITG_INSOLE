#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "header.h"
#include "Task.h"
#include "WS2812.h"

#define DEVICE_NAME "SKI SMART TRAVELER"
#define SERVICE_UUID "a8eda7cf-4f72-43ff-8d0b-10c592128526"
#define Status_sensor_CHARACTERISTIC_UUID "dc4f0000-4684-4a1d-abef-3e924004250c"
#define Gyro_sensor_CHARACTERISTIC_UUID   "dc4f0001-df8a-4c3d-8141-45f613e178f3"
#define PRESS1_sensor_CHARACTERISTIC_UUID "dc4f0002-385e-45d7-af46-906b4561cbe0"
#define PRESS2_sensor_CHARACTERISTIC_UUID "dc4f0003-1cd7-41df-b63f-1788b29a4b80"
#define PRESS3_sensor_CHARACTERISTIC_UUID "dc4f0004-0bc8-4a9f-8a76-d5cd99a8dec9"

extern NimBLECharacteristic *Status_sensor_Characteristic;
extern NimBLECharacteristic *Gyro_sensor_Characteristic;
extern NimBLECharacteristic *PRESS1_sensor_Characteristic;
extern NimBLECharacteristic *PRESS2_sensor_Characteristic;
extern NimBLECharacteristic *PRESS3_sensor_Characteristic;

void int_NimBLE();

#endif // !__BLUETOOTH_H
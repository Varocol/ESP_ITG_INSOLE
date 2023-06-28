#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "header.h"
#include "Task.h"
#include "WS2812.h"


#define Device_Information_Service_UUID       "180A"
#define Serial_Number_CHARACTERISTIC_UUID     "2A25"
#define Firmware_Revision_CHARACTERISTIC_UUID "2A26"
#define Manufacturer_Name_CHARACTERISTIC_UUID "2A29"
#define Battery_service_UUID                  "180F"
#define Battery_Level_CHARACTERISTIC_UUID     "2A19"
#define SERVICE_UUID                          "183B"
#define Status_sensor_CHARACTERISTIC_UUID "dc4f0000-4684-4a1d-abef-3e924004250c"
#define Gyro_sensor_CHARACTERISTIC_UUID   "dc4f0001-df8a-4c3d-8141-45f613e178f3"
#define Accel_sensor_CHARACTERISTIC_UUID  "dc4f0002-df8a-4c3d-8141-45f613e178f3"
#define Magn_sensor_CHARACTERISTIC_UUID   "dc4f0003-df8a-4c3d-8141-45f613e178f3"
#define Temp_sensor_CHARACTERISTIC_UUID       "2A6E"
#define PRESS1_sensor_CHARACTERISTIC_UUID "dc4f0005-385e-45d7-af46-906b4561cbe0"
#define PRESS2_sensor_CHARACTERISTIC_UUID "dc4f0006-1cd7-41df-b63f-1788b29a4b80"
#define PRESS3_sensor_CHARACTERISTIC_UUID "dc4f0007-0bc8-4a9f-8a76-d5cd99a8dec9"

extern NimBLECharacteristic *Status_sensor_Characteristic;
extern NimBLECharacteristic *Gyro_sensor_Characteristic;
extern NimBLECharacteristic *Accel_sensor_Characteristic;
extern NimBLECharacteristic *Magn_sensor_Characteristic;
extern NimBLECharacteristic *Temp_sensor_Characteristic;
extern NimBLECharacteristic *PRESS1_sensor_Characteristic;
extern NimBLECharacteristic *PRESS2_sensor_Characteristic;
extern NimBLECharacteristic *PRESS3_sensor_Characteristic;

void int_NimBLE();

#endif // !__BLUETOOTH_H
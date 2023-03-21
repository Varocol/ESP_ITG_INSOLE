#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "header.h"
#include "Hardware.h"

#define DEVICE_NAME "ESP32_ITG_INSOLE"
#define SERVICE_UUID "a8eda7cf-4f72-43ff-8d0b-10c592128526"
#define CHARACTERISTIC_UUID_RX "171d7456-df8a-4c3d-8141-45f613e178f3"
#define CHARACTERISTIC_UUID_TX "846c1746-2bd0-410e-a961-b74f6e65105d"

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer);
    void onDisconnect(BLEServer *pServer);
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic);
};

void bluetooth_init();
void bluetooth_sendmessage(String msg);

#endif // !__BLUETOOTH_H
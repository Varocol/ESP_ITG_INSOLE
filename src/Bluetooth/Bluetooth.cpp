#include "Bluetooth.h"

// bluetooth message
String *ble_msg;

BLEServer *pServer = NULL;                   // BLEServer
BLEService *pService = NULL;                 // BLEService
BLECharacteristic *pTxCharacteristic = NULL; // BLETxCharacteristic
BLECharacteristic *pRxCharacteristic = NULL; // BLERxCharacteristic

volatile bool deviceConnected = false; // current connection status

/**
 * @brief  Bluetooth Initialization
 * @param  None
 * @retval None
 */
void bluetooth_init()
{
    // Create a BLE device
    BLEDevice::init(DEVICE_NAME);
    // Create a BLE server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    pService = pServer->createService(SERVICE_UUID);

    // Create a BLE characteristic
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());
    // Start the service and the server
    pService->start();
    pServer->getAdvertising()->start();
}

/**
 * @brief  Bluetooth sendmessage
 * @param  msg              message
 * @retval None
 */
void bluetooth_sendmessage(String msg)
{
    // deviceConnected 已连接
    if (deviceConnected)
    {
        pTxCharacteristic->setValue((uint8_t *)msg.c_str(), msg.length());
        pTxCharacteristic->notify();
    }
}

void MyServerCallbacks::onConnect(BLEServer *pServer)
{
    deviceConnected = true;
    Message_Println("[BLE]:Connected.");
}

void MyServerCallbacks::onDisconnect(BLEServer *pServer)
{
    deviceConnected = false;
    delay(500);
    Serial.println("[BLE]:Disconnected.");
    Serial.println("[BLE]:Start Advertising.");
    pServer->getAdvertising()->start();
}

void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    ble_msg = new String(pCharacteristic->getValue().c_str());
    xTaskCreate(function_map_task, "function_map", TASK_STACK_SIZE, (void *)ble_msg, 1, NULL);
}
#include "Bluetooth.h"

static NimBLEServer *pServer;

NimBLECharacteristic *Status_sensor_Characteristic;
NimBLECharacteristic *Gyro_sensor_Characteristic;
NimBLECharacteristic *PRESS1_sensor_Characteristic;
NimBLECharacteristic *PRESS2_sensor_Characteristic;
NimBLECharacteristic *PRESS3_sensor_Characteristic;

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *pServer)
    {
        log_i("Client connected, Multi-connect support: start advertising");
        NimBLEDevice::startAdvertising();
    };
    void onConnect(NimBLEServer *pServer, ble_gap_conn_desc *desc)
    {
        log_i("NimBLE Client address: %s", NimBLEAddress(desc->peer_ota_addr).toString().c_str());
        pServer->updateConnParams(desc->conn_handle, 24, 48, 0, 60);
        bluetooth_is_connect = true;
        // 蓝色呼吸
        WS2812_Blink_typedef led_mode;
        led_mode.WS2812_Blink_Mode = Breathe;
        led_mode.r = 0;
        led_mode.g = 150;
        led_mode.b = 255;
        xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
    };
    void onDisconnect(NimBLEServer *pServer)
    {
        log_i("Client disconnected - start advertising");
        NimBLEDevice::startAdvertising();
        bluetooth_is_connect = false;
        // 蓝色呼吸
        WS2812_Blink_typedef led_mode;
        led_mode.WS2812_Blink_Mode = Breathe;
        led_mode.r = 150;
        led_mode.g = 0;
        led_mode.b = 255;
        xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
    };
};


static NimBLECharacteristicCallbacks READ_sensor_chrCallbacks;

void int_NimBLE()
{
    NimBLEDevice::init(DEVICE_NAME);
    NimBLEDevice::setPower(ESP_PWR_LVL_P9); /** +9db */
    NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_SC);
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());
    NimBLEService *READ_sensor_Service = pServer->createService(SERVICE_UUID);

    Status_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        Status_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Status_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    Gyro_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        Gyro_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Gyro_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    PRESS1_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        PRESS1_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    PRESS1_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);
    
    PRESS2_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        PRESS2_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    PRESS2_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    PRESS3_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        PRESS3_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    PRESS3_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    READ_sensor_Service->start();
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(READ_sensor_Service->getUUID());
    pAdvertising->setScanResponse(true);
    pAdvertising->start();
}
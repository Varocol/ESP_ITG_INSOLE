#include "Bluetooth.h"

static NimBLEServer *pServer;

NimBLECharacteristic *Status_sensor_Characteristic;
NimBLECharacteristic *Gyro_sensor_Characteristic;
NimBLECharacteristic *Accel_sensor_Characteristic;
NimBLECharacteristic *Magn_sensor_Characteristic;
NimBLECharacteristic *Temp_sensor_Characteristic;
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

    NimBLEService *Device_Information_Service = pServer->createService(Device_Information_Service_UUID);
    NimBLECharacteristic *Serial_Number_Characteristic = Device_Information_Service->createCharacteristic(
        Serial_Number_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    char macStr[18] = {0};
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_BT);
    sprintf(macStr, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial_Number_Characteristic->setValue(macStr);

    NimBLECharacteristic *Firmware_Revision_Characteristic = Device_Information_Service->createCharacteristic(
        Firmware_Revision_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Firmware_Revision_Characteristic->setValue(Firmware_Revision);

    NimBLECharacteristic *Manufacturer_Name_Characteristic = Device_Information_Service->createCharacteristic(
        Manufacturer_Name_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Manufacturer_Name_Characteristic->setValue(Manufacturer_Name);

    Device_Information_Service->start();

    NimBLEService *Battery_service_Service = pServer->createService(Battery_service_UUID);

    NimBLECharacteristic *Battery_Level_Characteristic = Battery_service_Service->createCharacteristic(
        Battery_Level_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    Battery_Level_Characteristic->setValue(Manufacturer_Name);

    Battery_service_Service->start();

    NimBLEService *READ_sensor_Service = pServer->createService(SERVICE_UUID);

    Status_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        Status_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Status_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    Gyro_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        Gyro_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Gyro_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    Accel_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        Accel_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Accel_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    Magn_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        Magn_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Magn_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

    Temp_sensor_Characteristic = READ_sensor_Service->createCharacteristic(
        Temp_sensor_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ);
    Temp_sensor_Characteristic->setCallbacks(&READ_sensor_chrCallbacks);

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
    pAdvertising->addServiceUUID(Battery_service_Service->getUUID());
    pAdvertising->addServiceUUID(Device_Information_Service->getUUID());
    pAdvertising->setScanResponse(true);
    pAdvertising->start();
}
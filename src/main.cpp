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

// 鞋垫插上去 INT会
void setup()
{
  Hardware_Init();
  Task_Init();
  int_NimBLE();
}

void loop()
{
  if (digitalRead(1) == LOW)
  {
    device_is_connect = true;
    ready_to_sleep = false;
    if (Check_Sensor())
    {
      if (!Check_Sensor_OK)
      {
        Check_Sensor_OK = true;
        if (!Pressure_Init())
        {
          hardware_error();
        }
        else
        {
          check_Pressure_Calibrate();
        }
        if (!Gyroscope_Init())
        {
          hardware_error();
        }
      }
      if (Check_Sensor_OK)
      {
        if (millis() - last_time > 5000)
        {
          // 每隔5秒闪烁一次LED（绿色）
          WS2812_Blink_typedef led_mode;
          led_mode.WS2812_Blink_Mode = Double_flashing;
          led_mode.r = 0;
          led_mode.g = 255;
          led_mode.b = 0;
          xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
          last_time = millis();
        }
        update_ble_data();
      }
    }
    else
    {
      // 明显，有传感器出现错误
      hardware_error();
      delay(1);
    }
  }
  else
  {
    Check_Sensor_OK = false;
    if (millis() - last_time > 3000)
    {
      // 每隔3秒闪烁一次LED（红色）
      WS2812_Blink_typedef led_mode;
      led_mode.WS2812_Blink_Mode = Breathe;
      led_mode.r = 233;
      led_mode.g = 175;
      led_mode.b = 0;
      xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
      last_time = millis();
    }
    device_is_connect = false;
  }
}

/** NimBLE_Server Demo:
 *
 *  Demonstrates many of the available features of the NimBLE server library.
 *
 *  Created: on March 22 2020
 *      Author: H2zero
 *
 */

// static NimBLEServer* pServer;

// /**  None of these are required as they will be handled by the library with defaults. **
//  **                       Remove as you see fit for your needs                        */
// class ServerCallbacks: public NimBLEServerCallbacks {
//     void onConnect(NimBLEServer* pServer) {
//         Serial.println("Client connected");
//         Serial.println("Multi-connect support: start advertising");
//         NimBLEDevice::startAdvertising();
//     };
//     /** Alternative onConnect() method to extract details of the connection.
//      *  See: src/ble_gap.h for the details of the ble_gap_conn_desc struct.
//      */
//     void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
//         Serial.print("Client address: ");
//         Serial.println(NimBLEAddress(desc->peer_ota_addr).toString().c_str());
//         /** We can use the connection handle here to ask for different connection parameters.
//          *  Args: connection handle, min connection interval, max connection interval
//          *  latency, supervision timeout.
//          *  Units; Min/Max Intervals: 1.25 millisecond increments.
//          *  Latency: number of intervals allowed to skip.
//          *  Timeout: 10 millisecond increments, try for 5x interval time for best results.
//          */
//         pServer->updateConnParams(desc->conn_handle, 24, 48, 0, 60);
//     };
//     void onDisconnect(NimBLEServer* pServer) {
//         Serial.println("Client disconnected - start advertising");
//         NimBLEDevice::startAdvertising();
//     };
//     void onMTUChange(uint16_t MTU, ble_gap_conn_desc* desc) {
//         Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, desc->conn_handle);
//     };

// /********************* Security handled here **********************
// ****** Note: these are the same return values as defaults ********/
//     uint32_t onPassKeyRequest(){
//         Serial.println("Server Passkey Request");
//         /** This should return a random 6 digit number for security
//          *  or make your own static passkey as done here.
//          */
//         return 123456;
//     };

//     bool onConfirmPIN(uint32_t pass_key){
//         Serial.print("The passkey YES/NO number: ");Serial.println(pass_key);
//         /** Return false if passkeys don't match. */
//         return true;
//     };

//     void onAuthenticationComplete(ble_gap_conn_desc* desc){
//         /** Check that encryption was successful, if not we disconnect the client */
//         if(!desc->sec_state.encrypted) {
//             NimBLEDevice::getServer()->disconnect(desc->conn_handle);
//             Serial.println("Encrypt connection failed - disconnecting client");
//             return;
//         }
//         Serial.println("Starting BLE work!");
//     };
// };

// /** Handler class for characteristic actions */
// class CharacteristicCallbacks: public NimBLECharacteristicCallbacks {
//     void onRead(NimBLECharacteristic* pCharacteristic){
//         Serial.print(pCharacteristic->getUUID().toString().c_str());
//         Serial.print(": onRead(), value: ");
//         Serial.println(pCharacteristic->getValue().c_str());
//     };

//     void onWrite(NimBLECharacteristic* pCharacteristic) {
//         Serial.print(pCharacteristic->getUUID().toString().c_str());
//         Serial.print(": onWrite(), value: ");
//         Serial.println(pCharacteristic->getValue().c_str());
//     };
//     /** Called before notification or indication is sent,
//      *  the value can be changed here before sending if desired.
//      */
//     void onNotify(NimBLECharacteristic* pCharacteristic) {
//         Serial.println("Sending notification to clients");
//     };

//     /** The status returned in status is defined in NimBLECharacteristic.h.
//      *  The value returned in code is the NimBLE host return code.
//      */
//     void onStatus(NimBLECharacteristic* pCharacteristic, Status status, int code) {
//         String str = ("Notification/Indication status code: ");
//         str += status;
//         str += ", return code: ";
//         str += code;
//         str += ", ";
//         str += NimBLEUtils::returnCodeToString(code);
//         Serial.println(str);
//     };

//     void onSubscribe(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc, uint16_t subValue) {
//         String str = "Client ID: ";
//         str += desc->conn_handle;
//         str += " Address: ";
//         str += std::string(NimBLEAddress(desc->peer_ota_addr)).c_str();
//         if(subValue == 0) {
//             str += " Unsubscribed to ";
//         }else if(subValue == 1) {
//             str += " Subscribed to notfications for ";
//         } else if(subValue == 2) {
//             str += " Subscribed to indications for ";
//         } else if(subValue == 3) {
//             str += " Subscribed to notifications and indications for ";
//         }
//         str += std::string(pCharacteristic->getUUID()).c_str();

//         Serial.println(str);
//     };
// };

// /** Handler class for descriptor actions */
// class DescriptorCallbacks : public NimBLEDescriptorCallbacks {
//     void onWrite(NimBLEDescriptor* pDescriptor) {
//         std::string dscVal = pDescriptor->getValue();
//         Serial.print("Descriptor witten value:");
//         Serial.println(dscVal.c_str());
//     };

//     void onRead(NimBLEDescriptor* pDescriptor) {
//         Serial.print(pDescriptor->getUUID().toString().c_str());
//         Serial.println(" Descriptor read");
//     };
// };

// /** Define callback instances globally to use for multiple Charateristics \ Descriptors */
// static CharacteristicCallbacks chrCallbacks;

// void setup() {
//     Serial.begin(115200);
//     Serial.println("Starting NimBLE Server");

//     NimBLEDevice::init(DEVICE_NAME);
//     NimBLEDevice::setPower(ESP_PWR_LVL_P9); /** +9db */
//     NimBLEDevice::setSecurityAuth( BLE_SM_PAIR_AUTHREQ_SC);
//     pServer = NimBLEDevice::createServer();
//     pServer->setCallbacks(new ServerCallbacks());
//     NimBLEService* pDeadService = pServer->createService(SERVICE_UUID);
//     NimBLECharacteristic* pBeefCharacteristic = pDeadService->createCharacteristic(
//                                                READ_CHARACTERISTIC_UUID,
//                                                NIMBLE_PROPERTY::READ |
//                                                NIMBLE_PROPERTY::WRITE |
//                                /** Require a secure connection for read and write access */
//                                                NIMBLE_PROPERTY::READ_ENC |  // only allow reading if paired / encrypted
//                                                NIMBLE_PROPERTY::WRITE_ENC   // only allow writing if paired / encrypted
//                                               );
//     pBeefCharacteristic->setValue("Burger");
//     pBeefCharacteristic->setCallbacks(&chrCallbacks);
//     /** Start the services when finished creating all Characteristics and Descriptors */
//     pDeadService->start();
//     NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
//     /** Add the services to the advertisment data **/
//     pAdvertising->addServiceUUID(pDeadService->getUUID());
//     /** If your device is battery powered you may consider setting scan response
//      *  to false as it will extend battery life at the expense of less data sent.
//      */
//     pAdvertising->setScanResponse(true);
//     pAdvertising->start();
//     Serial.println("Advertising Started");
// }

// void loop() {
//   delay(2000);
// }
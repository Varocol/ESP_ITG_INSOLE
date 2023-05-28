#include "Hardware.h"

// serial message
String *serial_msg;
bool certification = false;



/**
 * @brief  Serial Devices Initialization
 * @param  None
 * @retval true on success,false on failed
 */
void Wire_Init()
{
    if (Wire.setPins(SDA_PIN, SCL_PIN))
    {
        log_i("[Wire]:Pin Set Successfully.");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
    else
    {
        log_e("[Wire]:Pin Set Failed.");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
    if (Wire.begin(-1, -1, I2C_BAUDRATE))
    {
        log_i("[Wire]:I2C Started Successfully.");
    }
    else
    {
        log_e("[Wire]:I2C Started Failed.");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
#ifdef WIRE_USE_MUTEX
    if (Wire_Mutex_Init())
    {
        log_i("[Wire]:Mutex Initialized.");
    }
    else
    {
        log_e("[Wire]:Mutex Failed.");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
#endif
}
/**
 * @brief  Pressure Sensors Initialization
 * @param  None
 * @retval true on success,false on failed
 */
bool Pressure_Init()
{
    PRESSURE_STATUS_CODE status;
    status = pressure1.init();
    log_i("%s", pressure1.message(status));
    if (status < 0)
    {
        return false;
    }
    status = pressure2.init();
    log_i("%s", pressure2.message(status));
    if (status < 0)
    {
        return false;
    }
    status = pressure3.init();
    log_i("%s", pressure3.message(status));
    if (status < 0)
    {
        return false;
    }
    return true;
}

/**
 * @brief  Gyroscope Initialization
 * @param  None
 * @retval true on success,false on failed
 */
bool Gyroscope_Init()
{
    GYROSCOPE_STATUS_CODE status = gyroscope_init();
    log_i("%s", gyroscope_message(status));
    if (status < 0)
    {
        return false;
    }
    return true;
}

/**
 * @brief  Hardware Initialization
 * @param  None
 * @retval true on success,false on failed
 */
void Hardware_Init()
{
    init_LED();
    Wire_Init();
    // if (!Wire_Init())
    // {
    //     return false;
    // }
    // if (!Pressure_Init())
    // {
    //     return false;
    // }
    // if (!Gyroscope_Init())
    // {
    //     return false;
    // }
}

/**
 * @brief  Output Interface
 * @param  msg          message
 * @retval None
 */
void Message_Println(const String &msg)
{
    Message_Printf(msg + "\r\n");
}

/**
 * @brief  Output Interface
 * @param  msg          message
 * @retval None
 */
void Message_Printf(const String &msg)
{
    // Serial side
    Serial.printf(msg.c_str());
    // Blutooth side
    bluetooth_sendmessage(msg);
}

/**
 * @brief  Gyroscope GetData
 * @param  None
 * @retval data or message
 */
String Gyroscope_GetData()
{
    return gyroscope_message(gyroscope_getData());
}

/**
 * @brief  Get data in json style(include all sensors' data)
 * @param  None
 * @retval json
 */
String data_json()
{
    Pressure_Packet pressure_packet;
    Gyroscope_Packet gyroscope_packet;
    PRESSURE_STATUS_CODE status;
    uint16_t benchmark_Val[CHANNEL_NUM];
    // json document object
    DynamicJsonDocument doc(2048);

    // add pressure1 datas
    JsonObject pressure1_json = doc.createNestedObject("PRESSURE1");
    JsonArray pressure1_data_array = pressure1_json.createNestedArray("datas");
    status = pressure1.getData(pressure_packet);
    if (status < 0)
    {
        return pressure1.message(status);
    }
    status = pressure1.getCalibrateVal(benchmark_Val);
    if (status < 0)
    {
        return pressure1.message(status);
    }
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        pressure1_data_array.add(highByte(pressure_packet.datas.at(i)) - highByte(benchmark_Val[i]));
    }

    // add pressure2 datas
    JsonObject pressure2_json = doc.createNestedObject("PRESSURE2");
    JsonArray pressure2_data_array = pressure2_json.createNestedArray("datas");
    status = pressure2.getData(pressure_packet);
    if (status < 0)
    {
        return pressure2.message(status);
    }
    status = pressure2.getCalibrateVal(benchmark_Val);
    if (status < 0)
    {
        return pressure2.message(status);
    }
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        pressure2_data_array.add(highByte(pressure_packet.datas.at(i)) - highByte(benchmark_Val[i]));
    }

    // add pressure3 datas
    JsonObject pressure3_json = doc.createNestedObject("PRESSURE3");
    JsonArray pressure3_data_array = pressure3_json.createNestedArray("datas");
    status = pressure3.getData(pressure_packet);
    if (status < 0)
    {
        return pressure3.message(status);
    }
    status = pressure3.getCalibrateVal(benchmark_Val);
    if (status < 0)
    {
        return pressure3.message(status);
    }
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        pressure3_data_array.add(highByte(pressure_packet.datas.at(i)) - highByte(benchmark_Val[i]));
    }

    // add gyroscope data
    gyroscope_packet = gyroscope_getData();
    JsonObject gyroscope_json = doc.createNestedObject("GYROSCOPE");
    JsonObject gyroscope_datas_json = gyroscope_json.createNestedObject("datas");
    JsonObject gyroscope_accel_json = gyroscope_datas_json.createNestedObject("accel");
    gyroscope_accel_json["x"] = gyroscope_packet.accel.acceleration.x;
    gyroscope_accel_json["y"] = gyroscope_packet.accel.acceleration.y;
    gyroscope_accel_json["z"] = gyroscope_packet.accel.acceleration.z;

    JsonObject gyroscope_gyro_json = gyroscope_datas_json.createNestedObject("gyro");
    gyroscope_gyro_json["x"] = gyroscope_packet.gyro.gyro.x;
    gyroscope_gyro_json["y"] = gyroscope_packet.gyro.gyro.y;
    gyroscope_gyro_json["z"] = gyroscope_packet.gyro.gyro.z;

    JsonObject gyroscope_mag_json = gyroscope_datas_json.createNestedObject("mag");
    gyroscope_mag_json["x"] = gyroscope_packet.mag.magnetic.x;
    gyroscope_mag_json["y"] = gyroscope_packet.mag.magnetic.y;
    gyroscope_mag_json["z"] = gyroscope_packet.mag.magnetic.z;

    gyroscope_datas_json["temp"] = gyroscope_packet.temp.temperature;

    String json;
    serializeJson(doc, json);
    // serializeJsonPretty(doc, json);
    return json;
}

bool Check_Sensor()
{
    byte error;
    Wire.beginTransmission(GYROSCOPE_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        return false;
    }
    else
    {
        log_e("I2C GYROSCOPE error : %d", error);
    }
    Wire.beginTransmission(PRESSURE1_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        return false;
    }
    else
    {
        log_e("I2C PRESSURE1 error : %d", error);
    }
    Wire.beginTransmission(PRESSURE2_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        return false;
    }
    else
    {
        log_e("I2C PRESSURE2 error : %d", error);
    }
    Wire.beginTransmission(PRESSURE3_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        return false;
    }
    else
    {
        log_e("I2C PRESSURE3 error : %d", error);
    }
    return true;
}


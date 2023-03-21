#include "Hardware.h"

// serial message
String *serial_msg;

/**
 * @brief  Serial Devices Initialization
 * @param  None
 * @retval None
 */
void Serial_Callback(void)
{
    while (Serial.available())
    {
        serial_msg = new String(Serial.readString());
        if (serial_msg->charAt(serial_msg->length() - 1) == '\r' || serial_msg->charAt(serial_msg->length() - 1) == '\n')
        {
            serial_msg->remove(serial_msg->length() - 1);
        }
        xTaskCreate(function_map_task, "function_map", TASK_STACK_SIZE, (void *)serial_msg, 1, NULL);
    }
}

/**
 * @brief  Serial Devices Initialization
 * @param  None
 * @retval None
 */
void Serial_Init()
{
    Serial.begin(SERIAL_BAUDRATE);
    Serial.onReceive(Serial_Callback);
}

/**
 * @brief  Bluetooth Initialization
 * @param  None
 * @retval None
 */
void Bluetooth_Init()
{
    bluetooth_init();
    Message_Println("[BLE]:Bluetooth Started Successfully.");
}

/**
 * @brief  Serial Devices Initialization
 * @param  None
 * @retval true on success,false on failed
 */
bool Wire_Init()
{
    if (Wire.setPins(SDA_PIN, SCL_PIN))
    {
        Message_Println("[Wire]:Pin Set Successfully.");
    }
    else
    {
        Message_Println("[Wire]:Pin Set Failed.");
        return false;
    }
    if (Wire.begin(-1, -1, I2C_BAUDRATE))
    {
        Message_Println("[Wire]:I2C Started Successfully.");
    }
    else
    {
        Message_Println("[Wire]:I2C Started Failed.");
        return false;
    }
#ifdef WIRE_USE_MUTEX
    if (Wire_Mutex_Init())
    {
        Message_Println("[Wire]:Mutex Initialized.");
    }
    else
    {
        Message_Println("[Wire]:Mutex Failed.");
        return false;
    }
#endif
    return true;
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
    Message_Println(pressure1.message(status));
    if (status < 0)
    {
        return false;
    }
    status = pressure2.init();
    Message_Println(pressure2.message(status));
    if (status < 0)
    {
        return false;
    }
    status = pressure3.init();
    Message_Println(pressure3.message(status));
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
    Message_Println(gyroscope_message(status));
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
bool Hardware_Init()
{
    Serial_Init();
    Bluetooth_Init();
    if (!Wire_Init())
    {
        return false;
    }
    if (!Pressure_Init())
    {
        return false;
    }
    if (!Gyroscope_Init())
    {
        return false;
    }
    return true;
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

#if Debug_Mode
/**
 * @brief  Pressure Get Data
 * @param  pressure_id      Pressure Id
 * @retval data or message
 */
String Pressure_GetData(const Pressure_Id &pressure_id)
{
    PRESSURE_STATUS_CODE status;
    Pressure *pressure;
    Pressure_Packet packet;
    int timeout = TIMEOUT;
    switch (pressure_id)
    {
    case PRESSURE1:
        pressure = &pressure1;
        break;
    case PRESSURE2:
        pressure = &pressure2;
        break;
    case PRESSURE3:
        pressure = &pressure3;
        break;
    }
    while (pressure->allStageconversionCheck() == false && timeout--)
        ;
    if (timeout == -1)
    {
        return pressure->message(STAGE_CONVERT_TIMEOUT);
    }
    status = pressure->getData(packet);
    if (status < 0)
    {
        return pressure->message(status);
    }
    else
    {
        return pressure->message(packet);
    }
}

/**
 * @brief  Pressure Calibrate
 * @param  pressure_id      Pressure Id
 * @retval data or message
 */
String Pressure_Calibrate(const Pressure_Id &pressure_id)
{
    PRESSURE_STATUS_CODE status;
    Pressure *pressure;
    String msg;
    switch (pressure_id)
    {
    case PRESSURE1:
        pressure = &pressure1;
        break;
    case PRESSURE2:
        pressure = &pressure2;
        break;
    case PRESSURE3:
        pressure = &pressure3;
        break;
    }
    status = pressure->calibration(msg);
    if (status >= 0)
    {
        return msg + "\r\n" + pressure->message(status);
    }
    return pressure->message(status);
}

/**
 * @brief  Gyroscope GetGyroRateDivisor
 * @param  None
 * @retval data or message
 */
String Gyroscope_GetGyroRateDivisor()
{
    uint8_t divisor = gyroscope_getGyroRateDivisor();
    stringstream str;
    str << "Gyro Rate Divisor|" << (int)divisor + 1 << " Gyro Data Rate|" << 1100 / (1.0 + divisor) << " Hz";
    return gyroscope_message(str.str().c_str());
}

/**
 * @brief  Gyroscope SetGyroRateDivisor
 * @param  new_gyro_divisor         Gyro Rate Divisor value
 * @retval data or message
 */
String Gyroscope_SetGyroRateDivisor(const uint8_t &new_gyro_divisor)
{
    return gyroscope_message(gyroscope_setGyroRateDivisor(new_gyro_divisor));
}

/**
 * @brief  Gyroscope GetAccelRateDivisor
 * @param  None
 * @retval data or message
 */
String Gyroscope_GetAccelRateDivisor()
{
    uint8_t divisor = gyroscope_getAccelRateDivisor();
    stringstream str;
    str << "Accel Rate Divisor|" << (int)divisor + 1 << " Accel Data Rate|" << 1125 / (1.0 + divisor) << " Hz";
    return gyroscope_message(str.str().c_str());
}

/**
 * @brief  Gyroscope SetAccelRateDivisor
 * @param  new_accel_divisor        Accel Rate Divisor value
 * @retval data or message
 */
String Gyroscope_SetAccelRateDivisor(const uint16_t &new_accel_divisor)
{
    return gyroscope_message(gyroscope_setAccelRateDivisor(new_accel_divisor));
}

/**
 * @brief  Gyroscope GetAccelRange
 * @param  None
 * @retval data or message
 */
String Gyroscope_GetAccelRange()
{
    stringstream str;
    str << "Accel Range|";
    switch (gyroscope_getAccelRange())
    {
    case ICM20948_ACCEL_RANGE_2_G:
        str << "+-2G";
        break;
    case ICM20948_ACCEL_RANGE_4_G:
        str << "+-4G";
        break;
    case ICM20948_ACCEL_RANGE_8_G:
        str << "+-8G";
        break;
    case ICM20948_ACCEL_RANGE_16_G:
        str << "+-16G";
        break;
    }
    return gyroscope_message(str.str().c_str());
}

/**
 * @brief  Gyroscope SetAccelRange
 * @param  new_accel_range      Accel Range value
 * @retval data or message
 */
String Gyroscope_SetAccelRange(const icm20948_accel_range_t &new_accel_range)
{
    return gyroscope_message(gyroscope_setAccelRange(new_accel_range));
}

/**
 * @brief  Gyroscope GetAccelRange
 * @param  None
 * @retval data or message
 */
String Gyroscope_GetGyroRange()
{
    stringstream str;
    str << "Gyro Range|";
    switch (gyroscope_getGyroRange())
    {
    case ICM20948_GYRO_RANGE_250_DPS:
        str << "250 degrees/s";
        break;
    case ICM20948_GYRO_RANGE_500_DPS:
        str << "500 degrees/s";
        break;
    case ICM20948_GYRO_RANGE_1000_DPS:
        str << "1000 degrees/s";
        break;
    case ICM20948_GYRO_RANGE_2000_DPS:
        str << "2000 degrees/s";
        break;
    }
    return gyroscope_message(str.str().c_str());
}

/**
 * @brief  Gyroscope SetGyroRange
 * @param  new_gyro_range           Gyro Range
 * @retval data or message
 */
String Gyroscope_SetGyroRange(const icm20948_gyro_range_t &new_gyro_range)
{
    return gyroscope_message(gyroscope_setGyroRange(new_gyro_range));
}

/**
 * @brief  Gyroscope GetMagDataRate
 * @param  None
 * @retval data or message
 */
String Gyroscope_GetMagDataRate()
{
    stringstream str;
    str << "Mag Data Rate|";
    switch (gyroscope_getMagDataRate())
    {
    case AK09916_MAG_DATARATE_SHUTDOWN:
        str << "Shutdown";
        break;
    case AK09916_MAG_DATARATE_SINGLE:
        str << "Single/One shot";
        break;
    case AK09916_MAG_DATARATE_10_HZ:
        str << "10 Hz";
        break;
    case AK09916_MAG_DATARATE_20_HZ:
        str << "20 Hz";
        break;
    case AK09916_MAG_DATARATE_50_HZ:
        str << "50 Hz";
        break;
    case AK09916_MAG_DATARATE_100_HZ:
        str << "100 Hz";
        break;
    }
    return gyroscope_message(str.str().c_str());
}

/**
 * @brief  Gyroscope SetMagDataRate
 * @param  rate             Mag Data Rate
 * @retval data or message
 */
String Gyroscope_SetMagDataRate(const ak09916_data_rate_t &rate)
{
    return gyroscope_message(gyroscope_setMagDataRate(rate));
}

/**
 * @brief  Gyroscope EnableAccelDLPF
 * @param  enable           enable or disable
 * @param  cutoff_freq      cutoff frequency
 * @retval data or message
 */
String Gyroscope_EnableAccelDLPF(const bool &enable, const icm20x_accel_cutoff_t &cutoff_freq)
{
    return gyroscope_message(gyroscope_enableAccelDLPF(enable, cutoff_freq));
}

/**
 * @brief  Gyroscope EnableGyrolDLPF
 * @param  enable           enable or disable
 * @param  cutoff_freq      cutoff frequency
 * @retval data or message
 */
String Gyroscope_EnableGyrolDLPF(const bool &enable, const icm20x_gyro_cutoff_t &cutoff_freq)
{
    return gyroscope_message(gyroscope_enableGyrolDLPF(enable, cutoff_freq));
}

/**
 * @brief  Pressure-clear nvs namespace
 * @param  None
 * @retval message
 */
String clear_nvs_namespace()
{
    esp_err_t status = nvs_flash_erase();
    if (status == ESP_ERR_NOT_FOUND)
    {
        return "no nvs partition";
    }
    status = nvs_flash_init();
    if (status != ESP_OK)
    {
        return "nvs flash init failed";
    }
    return "clear namespace success";
}

#endif

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
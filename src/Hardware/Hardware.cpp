#include "Hardware.h"

// serial message
String *serial_msg;
bool certification = false;

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

void check_Pressure_Calibrate()
{
    Preferences prefs;
    String Pressure_Calibrate_temp_str = "";
    if (prefs.begin(CALIBRATE_VALUE_SPACENAME) == true)
    {
        if (!prefs.isKey(PRESSURE1_CALIBRATE_VALUE_KEY))
        {
            Pressure_Calibrate_temp_str = Pressure_Calibrate(PRESSURE1);
            log_i("%s", Pressure_Calibrate_temp_str.c_str());
        }
        if (!prefs.isKey(PRESSURE2_CALIBRATE_VALUE_KEY))
        {
            Pressure_Calibrate_temp_str = Pressure_Calibrate(PRESSURE2);
            log_i("%s", Pressure_Calibrate_temp_str.c_str());
        }
        if (!prefs.isKey(PRESSURE3_CALIBRATE_VALUE_KEY))
        {
            Pressure_Calibrate_temp_str = Pressure_Calibrate(PRESSURE3);
            log_i("%s", Pressure_Calibrate_temp_str.c_str());
        }
    }
    else
    {
        Pressure_Calibrate_temp_str = Pressure_Calibrate(PRESSURE1);
        log_i("%s", Pressure_Calibrate_temp_str.c_str());
        Pressure_Calibrate_temp_str = Pressure_Calibrate(PRESSURE2);
        log_i("%s", Pressure_Calibrate_temp_str.c_str());
        Pressure_Calibrate_temp_str = Pressure_Calibrate(PRESSURE3);
        log_i("%s", Pressure_Calibrate_temp_str.c_str());
    }
}

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
    }
    else
    {
        log_e("[Wire]:Pin Set Failed.");
        WS2812_Blink_typedef led_mode;
        led_mode.WS2812_Blink_Mode = Double_flashing;
        led_mode.r = 255;
        led_mode.g = 0;
        led_mode.b = 0;
        xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
        ESP_ERROR_CHECK(ESP_FAIL);
    }
    if (Wire.begin(-1, -1, I2C_BAUDRATE))
    {
        log_i("[Wire]:I2C Started Successfully.");
    }
    else
    {
        log_e("[Wire]:I2C Started Failed.");
        WS2812_Blink_typedef led_mode;
        led_mode.WS2812_Blink_Mode = Double_flashing;
        led_mode.r = 255;
        led_mode.g = 0;
        led_mode.b = 0;
        xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
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
        WS2812_Blink_typedef led_mode;
        led_mode.WS2812_Blink_Mode = Double_flashing;
        led_mode.r = 255;
        led_mode.g = 0;
        led_mode.b = 0;
        xQueueSend(WS2812_control, &led_mode, portMAX_DELAY);
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
    PRESSURE_STATUS_CODE press_status;
    GYROSCOPE_STATUS_CODE gyro_status;
    String status_string;
    press_status = pressure1.init();
    log_d("%s", pressure1.message(press_status).c_str());
    if (press_status < 0)
    {
        set_ble_status("Press1_Error_" + press_status);
        return false;
    }
    press_status = pressure2.init();
    log_d("%s", pressure2.message(press_status).c_str());
    if (press_status < 0)
    {
        set_ble_status("Press2_Error_" + press_status);
        return false;
    }
    press_status = pressure3.init();
    log_d("%s", pressure3.message(press_status).c_str());
    if (press_status < 0)
    {
        set_ble_status("Press3_Error_" + press_status);
        return false;
    }
    gyro_status = gyroscope_init();
    log_d("%s", gyroscope_message(gyro_status).c_str());
    if (gyro_status < 0)
    {
        set_ble_status("Gyro_Error_" + gyro_status);
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
    log_d("%s", gyroscope_message(status).c_str());
    if (status < 0)
    {
        set_ble_status("Gyro_Error_");
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
    pinMode(1, INPUT_PULLUP);
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
    // bluetooth_sendmessage(msg);
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
    uint8_t error;
    Wire.beginTransmission(GYROSCOPE_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        log_e("I2C GYROSCOPE error : %d", error);
        return false;
    }
    Wire.beginTransmission(PRESSURE1_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        log_e("I2C PRESSURE1 error : %d", error);
        return false;
    }
    Wire.beginTransmission(PRESSURE2_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        log_e("I2C PRESSURE2 error : %d", error);
        return false;
    }
    Wire.beginTransmission(PRESSURE3_ADDR);
    error = Wire.endTransmission();
    if (error != 0)
    {
        log_e("I2C PRESSURE3 error : %d", error);
        return false;
    }
    return true;
}

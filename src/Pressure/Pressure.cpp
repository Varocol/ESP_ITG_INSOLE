#include "Pressure.h"

SemaphoreHandle_t wire_lock;

uint16_t STAGES_DEFAULT_CONFIG[CHANNEL_NUM][2]{
    {STAGE0_CONNECTION1_DEFAULT_CONFIG, STAGE0_CONNECTION2_DEFAULT_CONFIG},
    {STAGE1_CONNECTION1_DEFAULT_CONFIG, STAGE1_CONNECTION2_DEFAULT_CONFIG},
    {STAGE2_CONNECTION1_DEFAULT_CONFIG, STAGE2_CONNECTION2_DEFAULT_CONFIG},
    {STAGE3_CONNECTION1_DEFAULT_CONFIG, STAGE3_CONNECTION2_DEFAULT_CONFIG},
    {STAGE4_CONNECTION1_DEFAULT_CONFIG, STAGE4_CONNECTION2_DEFAULT_CONFIG},
    {STAGE5_CONNECTION1_DEFAULT_CONFIG, STAGE5_CONNECTION2_DEFAULT_CONFIG},
    {STAGE6_CONNECTION1_DEFAULT_CONFIG, STAGE6_CONNECTION2_DEFAULT_CONFIG},
    {STAGE7_CONNECTION1_DEFAULT_CONFIG, STAGE7_CONNECTION2_DEFAULT_CONFIG},
    {STAGE8_CONNECTION1_DEFAULT_CONFIG, STAGE8_CONNECTION2_DEFAULT_CONFIG},
    {STAGE9_CONNECTION1_DEFAULT_CONFIG, STAGE9_CONNECTION2_DEFAULT_CONFIG},
    {STAGEA_CONNECTION1_DEFAULT_CONFIG, STAGEA_CONNECTION2_DEFAULT_CONFIG},
    {STAGEB_CONNECTION1_DEFAULT_CONFIG, STAGEB_CONNECTION2_DEFAULT_CONFIG}};

/**
 * @brief  Pressure-transmit datas
 * @param  len              length of datas
 * @retval None
 */
void Pressure::sendDatas(const uint8_t &len, ...)
{
#ifdef WIRE_USE_MUTEX
    // cannot use the system lock, because the process should not be broken
    if (wire_lock != NULL)
    {
        if (xSemaphoreTake(wire_lock, portMAX_DELAY) == pdTRUE)
        {
#endif
            va_list ptr;
            va_start(ptr, len);
            Wire.beginTransmission(slaveaddr);
            for (uint8_t i = 0; i < len; i++)
            {
                uint16_t data = va_arg(ptr, int);
                Wire.write(highByte(data));
                Wire.write(lowByte(data));
            }
            va_end(ptr);
            Wire.endTransmission();
#ifdef WIRE_USE_MUTEX
            xSemaphoreGive(wire_lock);
        }
    }
#endif
}

/**
 * @brief  Pressure-receive datas
 * @param  len              length of datas
 * @retval datas of specific registers
 */
Pressure_Packet Pressure::receiveDatas(const uint8_t &len)
{
    Pressure_Packet packet;
#ifdef WIRE_USE_MUTEX
    if (wire_lock != NULL)
    {
        if (xSemaphoreTake(wire_lock, portMAX_DELAY) == pdTRUE)
        {
#endif
            bool flag = false;
            uint16_t temp = 0;
            if (Wire.requestFrom(slaveaddr, (size_t)(2 * len), true))
            {
                while (Wire.available())
                {
                    temp = temp << 8 | Wire.read();
                    if (flag)
                    {
                        packet.datas.push_back(temp);
                        temp = 0;
                    }
                    flag = !flag;
                }
            }
#ifdef WIRE_USE_MUTEX
            xSemaphoreGive(wire_lock);
        }
    }
#endif
    return packet;
}

/**
 * @brief  Pressure-get the calibrated values which stored in flash
 * @param  None
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::getCalibrateVal()
{
    const char *key;
    switch (slaveaddr)
    {
    case PRESSURE1_ADDR:
        key = PRESSURE1_CALIBRATE_VALUE_KEY;
        break;
    case PRESSURE2_ADDR:
        key = PRESSURE2_CALIBRATE_VALUE_KEY;
        break;
    case PRESSURE3_ADDR:
        key = PRESSURE3_CALIBRATE_VALUE_KEY;
        break;
    default:
        return SLAVEADDR_ERR;
        break;
    }
    // use preferences store datas
    Preferences prefs;
    if (prefs.begin(CALIBRATE_VALUE_SPACENAME) == false)
    {
        return NAMESPACE_FAILED;
    }
    // if the nvs partition space is insufficient, you can use the following code
    /*
        clear_nvs_namespace();
        Serial.printf("%d\r\n", prefs.freeEntries());
    */
    // get the data corresponding to the key
    prefs.getBytes(key, Calibrate_Val, sizeof(Calibrate_Val));
    prefs.end();
    return GET_CALIBRATE_VALUE_SUCCESS;
}

/**
 * @brief  Pressure-set the calibrated values datas
 * @param  None
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::setCalibrateVal()
{
    const char *key;
    switch (slaveaddr)
    {
    case PRESSURE1_ADDR:
        key = PRESSURE1_CALIBRATE_VALUE_KEY;
        break;
    case PRESSURE2_ADDR:
        key = PRESSURE2_CALIBRATE_VALUE_KEY;
        break;
    case PRESSURE3_ADDR:
        key = PRESSURE3_CALIBRATE_VALUE_KEY;
        break;
    default:
        return SLAVEADDR_ERR;
        break;
    }
    // use preferences store datas
    Preferences prefs;
    if (prefs.begin(CALIBRATE_VALUE_SPACENAME) == false)
    {
        return NAMESPACE_FAILED;
    }
    // get the data corresponding to the key
    if (prefs.putBytes(key, Calibrate_Val, sizeof(Calibrate_Val)) == 0)
    {
        prefs.end();
        return STORE_CALIBRATE_FAILED;
    }
    prefs.end();
    return SET_CALIBRATE_VALUE_SUCCESS;
}

/**
 * @brief  Pressure-check the single stage conversion status
 * @param  channel          channel number
 * @retval true or false
 */
bool Pressure::singleStageConversionCheck(const uint8_t &channel)
{
    // if the channel num incorrect, return false
    if (channel < 0 || channel > CHANNEL_NUM - 1)
    {
        return false;
    }
    sendDatas(1, STAGE_COMPLETE_STATUS_REG_ADDR);
    Pressure_Packet packet = receiveDatas(1);
    return !(packet.datas.empty() || packet.datas.at(0) != (STAGE0_COMPLETE_MASK << channel));
}

/**
 * @brief  Pressure-get the pressure address
 * @param  pressure_id      Pressure Id
 * @retval true or false
 */
uint8_t Pressure::getAddr(const Pressure_Id &pressure_id)
{
    switch (pressure_id)
    {
    case PRESSURE1:
        return PRESSURE1_ADDR;
    case PRESSURE2:
        return PRESSURE2_ADDR;
    case PRESSURE3:
        return PRESSURE3_ADDR;
    }
    return 0;
}

/**
 * @brief  Pressure-Construction method
 * @param  None
 * @retval None
 */
Pressure::Pressure()
{
}

/**
 * @brief  Pressure-Deconstruction method
 * @param  None
 * @retval None
 */
Pressure::~Pressure()
{
}

/**
 * @brief
 * @param  pressure_id  Pressure Id
 * @param  wire         I2C controll class
 * @retval None
 */
Pressure::Pressure(const Pressure_Id &pressure_id, const TwoWire &wire) : pressure_id(pressure_id), wire(wire), slaveaddr(getAddr(pressure_id))
{
}

/**
 * @brief  Pressure-Initialization
 * @param  None
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::init()
{
    PRESSURE_STATUS_CODE status;
    // verify the device id
    status = verify();
    if (status < 0)
    {
        return status;
    }
    // power on the device
    status = powerOn(FULL_POWER);
    if (status < 0)
    {
        return status;
    }
    // get the Calibrate value
    status = getCalibrateVal();
    if (status < 0)
    {
        return status;
    }
    // set up the stages
    status = configStages();
    if (status < 0)
    {
        return status;
    }
    return PRESSURE_INIT_SUCCESS;
}

/**
 * @brief  Pressure-Power Up
 * @param  powermode    full power or low power mode
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::powerOn(const PowerMode &powermode)
{
    Pressure_Packet packet;
    if (powermode == FULL_DOWN)
    {
        return POWERMODE_ERR;
    }
    uint8_t timeout = TIMEOUT;
    while (timeout--)
    {
        sendDatas(2, PWR_CONTROL_REG_ADDR, PWR_CONTROL_DEFAULT_CONFIG_MASK | powermode);
        sendDatas(1, PWR_CONTROL_REG_ADDR);
        packet = receiveDatas(1);
        if (!packet.datas.empty() && packet.datas.at(0) == (PWR_CONTROL_DEFAULT_CONFIG_MASK | powermode))
        {
            return POWERON_SUCCESS;
        }
    }
    return POWERON_FAILED;
}

/**
 * @brief  Pressure-Power Off
 * @param  None
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::powerOff()
{
    Pressure_Packet packet;
    uint8_t timeout = TIMEOUT;
    while (timeout--)
    {
        sendDatas(2, PWR_CONTROL_REG_ADDR, PWR_CONTROL_DEFAULT_CONFIG_MASK | FULL_DOWN);
        sendDatas(1, PWR_CONTROL_REG_ADDR);
        packet = receiveDatas(1);
        if (!packet.datas.empty() && packet.datas.at(0) == (PWR_CONTROL_DEFAULT_CONFIG_MASK | FULL_DOWN))
        {
            return POWEROFF_SUCCESS;
        }
    }
    return POWEROFF_FAILED;
}

/**
 * @brief  Pressure-setup stages
 * @param  None
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::configStages()
{
    int status;
    uint8_t timeout;
    Pressure_Packet packet;
    // config the stages with AFEOFFSET
    for (uint8_t i = 0; i < CHANNEL_NUM; i++)
    {
        timeout = TIMEOUT;
        while (timeout--)
        {
            sendDatas(4, STAGE_CONFIGURE_REG_BASIC_ADDR + i * STAGE_CONFIGURE_REG_STEP, STAGES_DEFAULT_CONFIG[i][0], STAGES_DEFAULT_CONFIG[i][1], STAGE_AFE_OFFSET_MASK | Calibrate_Val[i].offset);
            sendDatas(1, STAGE_CONFIGURE_REG_BASIC_ADDR + i * STAGE_CONFIGURE_REG_STEP);
            packet = receiveDatas(3);
            // verify the data writed
            if (!packet.datas.empty() && packet.datas.at(0) == STAGES_DEFAULT_CONFIG[i][0] && packet.datas.at(1) == STAGES_DEFAULT_CONFIG[i][1] && packet.datas.at(2) == (STAGE_AFE_OFFSET_MASK | Calibrate_Val[i].offset))
            {
                break;
            }
        }
        if (timeout == -1)
        {
            return CONFIG_STAGES_FAILED;
        }
    }
    return CONFIG_STAGES_SUCCESS;
}

/**
 * @brief  Pressure-Calibration
 * @param  msg              message
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::calibration(String &msg)
{
    PRESSURE_STATUS_CODE status;
    int timeout, sample_times;
    bool flag = true;
    stringstream calibrate_val;
    // 0x80 - 0xC0
    struct
    {
        uint16_t left = 0x80;
        uint16_t right = 0xC0;
        vector<uint16_t> result;
        bool flag = false;
        uint16_t sample;
    } Search_Structure[CHANNEL_NUM];
    while (flag)
    {
        flag = false;
        // update the mid val
        for (uint8_t i = 0; i < CHANNEL_NUM; i++)
        {
            if (Search_Structure[i].flag == false)
            {
                if (Search_Structure[i].left <= Search_Structure[i].right)
                {
                    Calibrate_Val[i].offset = (Search_Structure[i].left + Search_Structure[i].right) / 2;
                }
                else
                {
                    Calibrate_Val[i].offset = Calibrate_Val[i].offset == Search_Structure[i].left ? Search_Structure[i].right : Search_Structure[i].left;
                }
                Search_Structure[i].result.clear();
            }
        }
        // test the mid val
        status = configStages();
        if (status < 0)
        {
            return status;
        }
        // get relatively accurate results
        sample_times = SAMPLE_TIMES;
        while (sample_times--)
        {
            // check stages convert status
            timeout = TIMEOUT;
            while (allStageconversionCheck() == false && timeout--)
                ;
            if (timeout == -1)
            {
                return STAGE_CONVERT_TIMEOUT;
            }
            Pressure_Packet packet;
            status = getData(packet);
            if (status < 0)
            {
                return status;
            }
            for (int i = 0; i < CHANNEL_NUM; i++)
            {
                Search_Structure[i].result.push_back(packet.datas.at(i));
            }
        }
        // correct the results and revise the AFE_OFFSET
        // The sampling value increases as the capacitance value decreases
        // use the binary search to determine the AFE_OFFSET
        for (int i = 0; i < CHANNEL_NUM; i++)
        {
            // if left < right, indicate that the value need calibration
            if (Search_Structure[i].flag == false)
            {
                flag = true;
                // remove the max and min value to determine the sample result
                sort(Search_Structure[i].result.begin(), Search_Structure[i].result.end());
                // get average
                uint16_t sample = accumulate(Search_Structure[i].result.begin() + 1, Search_Structure[i].result.end() - 1, 0) / (SAMPLE_TIMES - 2);
                if (Search_Structure[i].left <= Search_Structure[i].right)
                {
                    Search_Structure[i].sample = sample;
                    // adjust search scope
                    if (Search_Structure[i].sample > CALIBRATION_BASE)
                    {
                        Search_Structure[i].left = Calibrate_Val[i].offset + 1;
                    }
                    else
                    {
                        Search_Structure[i].right = Calibrate_Val[i].offset - 1;
                    }
                }
                // if left > right, determine the AFEOFFSET
                else
                {
                    // compare the left and right sample
                    if (abs(sample - CALIBRATION_BASE) < abs(Search_Structure[i].sample - CALIBRATION_BASE))
                    {
                        Calibrate_Val[i].benchmark = sample;
                    }
                    else
                    {
                        Calibrate_Val[i].offset = Calibrate_Val[i].offset == Search_Structure[i].left ? Search_Structure[i].right : Search_Structure[i].left;
                        Calibrate_Val[i].benchmark = Search_Structure[i].sample;
                    }
                    Search_Structure[i].flag = true;
                }
            }
        }
    }
    calibrate_val << "Calibrate AFE_OFFSET|";
    for (uint8_t i = 0; i < CHANNEL_NUM; i++)
    {
        calibrate_val << "0x" << setfill('0') << setw(2) << hex << (uint16_t)Calibrate_Val[i].offset << ' ';
    }
    msg = message(calibrate_val.str().c_str());
    // store the calibrated value
    status = setCalibrateVal();
    if (status < 0)
    {
        return status;
    }
    return CALIBRATE_SUCCESS;
}

/**
 * @brief  Pressure-Device ID verify
 * @param  None
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::verify()
{
    Pressure_Packet packet;
    uint8_t timeout = TIMEOUT;
    while (timeout--)
    {
        sendDatas(1, DEVICE_ID_REG_ADDR);
        packet = receiveDatas(1);
        if (!packet.datas.empty() && packet.datas.at(0) == DEVICE_ID)
        {
            return VERIFY_SUCCESS;
        }
    }
    return VERIFY_FAILED;
}

/**
 * @brief  Pressure-Print code message
 * @param  status_code      PRESSURE STATUS CODE
 * @retval message
 */
String Pressure::message(const PRESSURE_STATUS_CODE &status_code)
{
    String device;
    String message;
    switch (status_code)
    {
    case GET_DATA_SUCCESS:
        message = "GET DATA SUCCESS";
        break;
    case VERIFY_SUCCESS:
        message = "VERIFY SUCCESS";
        break;
    case CALIBRATE_SUCCESS:
        message = "CALIBRATE SUCCESS";
        break;
    case CONFIG_STAGES_SUCCESS:
        message = "CONFIG STAGES SUCCESS";
        break;
    case POWEROFF_SUCCESS:
        message = "POWEROFF SUCCESS";
        break;
    case POWERON_SUCCESS:
        message = "POWERON SUCCESS";
        break;
    case PRESSURE_INIT_SUCCESS:
        message = "PRESSURE INIT SUCCESS";
        break;
    case SET_CALIBRATE_VALUE_SUCCESS:
        message = "SET CALIBRATE VALUE SUCCESS";
        break;
    case GET_CALIBRATE_VALUE_SUCCESS:
        message = "GET CALIBRATE VALUE SUCCESS";
        break;
    case VERIFY_FAILED:
        message = "VERIFY FAILED";
        break;
    case POWERON_FAILED:
        message = "POWERON FAILED";
        break;
    case POWEROFF_FAILED:
        message = "POWEROFF FAILED";
        break;
    case SLAVEADDR_ERR:
        message = "SLAVEADDR ERR";
        break;
    case NAMESPACE_FAILED:
        message = "NAMESPACE FAILED";
        break;
    case STORE_CALIBRATE_FAILED:
        message = "STORE CALIBRATE FAILED";
        break;
    case CONFIG_STAGES_FAILED:
        message = "CONFIGSTAGES FAILED";
        break;
    case POWERMODE_ERR:
        message = "POWERMODE ERR";
        break;
    case CALIBRATE_FAILED:
        message = "CALIBRATE FAILED";
        break;
    case STAGE_CONVERT_TIMEOUT:
        message = "STAGE CONVERT TIMEOUT";
        break;
    default:
        message = "UNKNOWN CODE";
        break;
    }
    switch (pressure_id)
    {
    case PRESSURE1:
        device = "[PRESSURE1]:";
        break;
    case PRESSURE2:
        device = "[PRESSURE2]:";
        break;
    case PRESSURE3:
        device = "[PRESSURE3]:";
        break;
    }
    return device + message;
}

/**
 * @brief  Pressure-Print code message
 * @param  msg              MESSAGE
 * @retval message
 */
String Pressure::message(const char *message)
{
    String device;
    switch (pressure_id)
    {
    case PRESSURE1:
        device = "[PRESSURE1]:";
        break;
    case PRESSURE2:
        device = "[PRESSURE2]:";
        break;
    case PRESSURE3:
        device = "[PRESSURE3]:";
        break;
    }
    return device + message;
}

/**
 * @brief  Pressure-Print code message
 * @param  packet           packet data
 * @retval message
 */
String Pressure::message(const Pressure_Packet &packet)
{
    const char *device;
    stringstream str;
    switch (pressure_id)
    {
    case PRESSURE1:
        device = "[PRESSURE1]:";
        break;
    case PRESSURE2:
        device = "[PRESSURE2]:";
        break;
    case PRESSURE3:
        device = "[PRESSURE3]:";
        break;
    }
    str << device;
    if (packet.datas.size() != CHANNEL_NUM)
    {
        str << "DATA ERR";
    }
    else
    {
        str << "DATAS|";
        for (uint8_t i = 0; i < CHANNEL_NUM; i++)
        {
            str << (highByte(packet.datas.at(i)) - highByte(Calibrate_Val[i].benchmark)) << ' ';
        }
    }
    return str.str().c_str();
}

/**
 * @brief  Pressure-Whether the conversion is complete
 * @param  None
 * @retval true or false
 */
bool Pressure::allStageconversionCheck()
{
    sendDatas(1, STAGE_COMPLETE_STATUS_REG_ADDR);
    Pressure_Packet packet = receiveDatas(1);
    return !(packet.datas.empty() || packet.datas.at(0) != (STAGE0_COMPLETE_MASK << (CHANNEL_NUM - 1)));
}

/**
 * @brief  Pressure-Get the datas of pressure sensor
 * @param  packet   store sample datas of 12 Channels
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::getData(Pressure_Packet &packet)
{
    PRESSURE_STATUS_CODE status;
    sendDatas(1, CONVERSION_RESULT_REG_ADDR);
    packet = receiveDatas(CHANNEL_NUM);
    status = powerOff();
    if (status < 0)
    {
        return status;
    }
    status = powerOn(FULL_POWER);
    if (status < 0)
    {
        return status;
    }
    return GET_DATA_SUCCESS;
}

/**
 * @brief  Pressure-Get the calibrate value of pressure sensor
 * @param  benchmark_Val   store calibrate base of 12 Channels
 * @retval PRESSURE STATUS CODE
 */
PRESSURE_STATUS_CODE Pressure::getCalibrateVal(uint16_t (&benchmark_Val)[CHANNEL_NUM])
{
    PRESSURE_STATUS_CODE status;
    status = getCalibrateVal();
    if (status < 0)
    {
        return status;
    }
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        benchmark_Val[i] = Calibrate_Val[i].benchmark;
    }
    return status;
}

#ifdef WIRE_USE_MUTEX
/**
 * @brief  Set the mutex of Wire
 * @param  None
 * @retval true on success,false on failed
 */
bool Wire_Mutex_Init()
{
    wire_lock = xSemaphoreCreateMutex();
    return wire_lock != NULL;
}
#endif

Pressure pressure1(PRESSURE1);
Pressure pressure2(PRESSURE2);
Pressure pressure3(PRESSURE3);
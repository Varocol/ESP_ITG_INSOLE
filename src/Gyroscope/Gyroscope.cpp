#include "Gyroscope.h"

Adafruit_ICM20948 icm20948;

/**
 * @brief  Gyroscope-Initialization
 * @param  None
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_init()
{
    if (icm20948.begin_I2C(GYROSCOPE_ADDR))
    {
        return GYROSCOPE_INIT_SUCCESS;
    }
    else
    {
        return GYROSCOPE_INIT_FAILED;
    }
}

/**
 * @brief  Gyroscope-Get Gyro Rate Divisor
 * @param  None
 * @retval Gyro Rate Divisor
 */
uint8_t gyroscope_getGyroRateDivisor()
{
    return icm20948.getGyroRateDivisor();
}

/**
 * @brief  Gyroscope-Set Gyro Rate Divisor
 * @param  new_gyro_divisor         Gyro Rate Divisor value
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_setGyroRateDivisor(const uint8_t &new_gyro_divisor)
{
    icm20948.setGyroRateDivisor(new_gyro_divisor);
    if (gyroscope_getGyroRateDivisor() != new_gyro_divisor)
    {
        return SET_GYRORATEDIVISOR_FAILED;
    }
    return SET_GYRORATEDIVISOR_SUCCESS;
}

/**
 * @brief  Gyroscope-Get Accel Rate Divisor
 * @param  None
 * @retval Accel Rate Divisor
 */
uint16_t gyroscope_getAccelRateDivisor()
{
    return icm20948.getAccelRateDivisor();
}

/**
 * @brief  Gyroscope-Set Accel Rate Divisor
 * @param  new_accel_divisor        Accel Rate Divisor value
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_setAccelRateDivisor(const uint16_t &new_accel_divisor)
{
    icm20948.setAccelRateDivisor(new_accel_divisor);
    if (gyroscope_getAccelRateDivisor() != new_accel_divisor)
    {
        return SET_ACCELRATEDIVISOR_FAILED;
    }
    return SET_ACCELRATEDIVISOR_SUCCESS;
}

/**
 * @brief  Gyroscope-Get Accel Range
 * @param  None
 * @retval Accel Range
 */
icm20948_accel_range_t gyroscope_getAccelRange()
{
    return icm20948.getAccelRange();
}

/**
 * @brief  Gyroscope-Set Accel Range
 * @param  new_accel_range      Accel Range value
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_setAccelRange(const icm20948_accel_range_t &new_accel_range)
{
    icm20948.setAccelRange(new_accel_range);
    if (gyroscope_getAccelRange() != new_accel_range)
    {
        return SET_ACCELRANGE_FAILED;
    }
    return SET_ACCELRANGE_SUCCESS;
}

/**
 * @brief  Gyroscope-Get Gyro Range
 * @param  None
 * @retval Gyro Range
 */
icm20948_gyro_range_t gyroscope_getGyroRange()
{
    return icm20948.getGyroRange();
}

/**
 * @brief  Gyroscope-Set Gyro Range
 * @param  new_gyro_range           Gyro Range
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_setGyroRange(const icm20948_gyro_range_t &new_gyro_range)
{
    icm20948.setGyroRange(new_gyro_range);
    if (gyroscope_getGyroRange() != new_gyro_range)
    {
        return SET_GYRORANGE_FAILED;
    }
    return SET_GYRORANGE_SUCCESS;
}

/**
 * @brief  Gyroscope-Get Mag Data Rate
 * @param  None
 * @retval Mag Data Rate
 */
ak09916_data_rate_t gyroscope_getMagDataRate()
{
    return icm20948.getMagDataRate();
}

/**
 * @brief  Gyroscope-Set Mag Data Rate
 * @param  rate             Mag Data Rate
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_setMagDataRate(const ak09916_data_rate_t &rate)
{
    icm20948.setMagDataRate(rate);
    if (gyroscope_getMagDataRate() != rate)
    {
        return SET_MAGDATARATE_FAILED;
    }
    return SET_MAGDATARATE_SUCCESS;
}

/**
 * @brief  Gyroscope-Enable Accel DLPF
 * @param  enable           enable or disable
 * @param  cutoff_freq      cutoff frequency
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_enableAccelDLPF(const bool &enable, const icm20x_accel_cutoff_t &cutoff_freq)
{
    if (icm20948.enableAccelDLPF(enable, cutoff_freq))
    {
        return ENABLEACCELDLPF_SUCCESS;
    }
    else
    {
        return ENABLEACCELDLPF_FAILED;
    }
}

/**
 * @brief  Gyroscope-Enable Gyrol DLPF
 * @param  enable           enable or disable
 * @param  cutoff_freq      cutoff frequency
 * @retval GYROSCOPE STATUS CODE
 */
GYROSCOPE_STATUS_CODE gyroscope_enableGyrolDLPF(const bool &enable, const icm20x_gyro_cutoff_t &cutoff_freq)
{
    if (icm20948.enableGyrolDLPF(enable, cutoff_freq))
    {
        return ENABLEGYROLDLPF_SUCCESS;
    }
    else
    {
        return ENABLEGYROLDLPF_FAILED;
    }
}

/**
 * @brief  Gyroscope-Get Data
 * @param  None
 * @retval Gyroscope Packet
 */
Gyroscope_Packet gyroscope_getData()
{
    Gyroscope_Packet packet;
    icm20948.getEvent(&packet.accel, &packet.gyro, &packet.temp, &packet.mag);
    return packet;
}

/**
 * @brief  Gyroscope-Print code message
 * @param  None
 * @retval message
 */
String gyroscope_message(const GYROSCOPE_STATUS_CODE &status_code)
{
    String device = "[GYROSCOPE]:";
    String message;
    switch (status_code)
    {
    case GYROSCOPE_INIT_FAILED:
        message = "GYROSCOPE INIT FAILED";
        break;
    case SET_GYRORATEDIVISOR_FAILED:
        message = "SET GYRORATEDIVISOR FAILED";
        break;
    case SET_ACCELRATEDIVISOR_FAILED:
        message = "SET ACCELRATEDIVISOR FAILED";
        break;
    case SET_ACCELRANGE_FAILED:
        message = "SET ACCELRANGE FAILED";
        break;
    case SET_GYRORANGE_FAILED:
        message = "SET GYRORANGE FAILED";
        break;
    case SET_MAGDATARATE_FAILED:
        message = "SET MAGDATARATE FAILED";
        break;
    case ENABLEACCELDLPF_FAILED:
        message = "ENABLEACCELDLPF FAILED";
        break;
    case ENABLEGYROLDLPF_FAILED:
        message = "ENABLEGYROLDLPF FAILED";
        break;
    case GYROSCOPE_INIT_SUCCESS:
        message = "GYROSCOPE INIT SUCCESS";
        break;
    case SET_GYRORATEDIVISOR_SUCCESS:
        message = "SET GYRORATEDIVISOR SUCCESS";
        break;
    case SET_ACCELRATEDIVISOR_SUCCESS:
        message = "SET ACCELRATEDIVISOR SUCCESS";
        break;
    case SET_ACCELRANGE_SUCCESS:
        message = "SET ACCELRANGE SUCCESS";
        break;
    case SET_GYRORANGE_SUCCESS:
        message = "SET GYRORANGE SUCCESS";
        break;
    case SET_MAGDATARATE_SUCCESS:
        message = "SET MAGDATARATE SUCCESS";
        break;
    case ENABLEACCELDLPF_SUCCESS:
        message = "ENABLEACCELDLP SUCCESS";
        break;
    case ENABLEGYROLDLPF_SUCCESS:
        message = "ENABLEGYROLDLPF SUCCESS";
        break;
    }
    return device + message;
}

/**
 * @brief  Gyroscope-Print code message
 * @param  None
 * @retval message
 */
String gyroscope_message(const char *msg)
{
    String device = "[GYROSCOPE]:";
    return device + msg;
}

/**
 * @brief  Gyroscope-Print code message
 * @param  None
 * @retval message
 */
String gyroscope_message(const Gyroscope_Packet &packet)
{
    String device = "[GYROSCOPE]:";
    stringstream str;
    str << "Accel"
        << "|x:" << packet.accel.acceleration.x << "|y:" << packet.accel.acceleration.y << "|z:" << packet.accel.acceleration.z << "m/s²|"
        << "Gyrol"
        << "|x:" << packet.gyro.gyro.x << "|y:" << packet.gyro.gyro.y << "|z:" << packet.gyro.gyro.z << "uT|"
        << "Mag"
        << "|x:" << packet.mag.magnetic.x << "|y:" << packet.mag.magnetic.y << "|z:" << packet.mag.magnetic.z << "radians/s|"
        << "|Temp" << packet.temp.temperature << "℃|";
    return device + str.str().c_str();
}
#ifndef __GYROSCOPE_H
#define __GYROSCOPE_H

#include "header.h"

#define GYROSCOPE_ADDR 0x68

using namespace std;
// GYROSCOPE STATUS CODE
typedef enum
{
    GYROSCOPE_INIT_FAILED = -8,
    SET_GYRORATEDIVISOR_FAILED,
    SET_ACCELRATEDIVISOR_FAILED,
    SET_ACCELRANGE_FAILED,
    SET_GYRORANGE_FAILED,
    SET_MAGDATARATE_FAILED,
    ENABLEACCELDLPF_FAILED,
    ENABLEGYROLDLPF_FAILED,
    GYROSCOPE_INIT_SUCCESS,
    SET_GYRORATEDIVISOR_SUCCESS,
    SET_ACCELRATEDIVISOR_SUCCESS,
    SET_ACCELRANGE_SUCCESS,
    SET_GYRORANGE_SUCCESS,
    SET_MAGDATARATE_SUCCESS,
    ENABLEACCELDLPF_SUCCESS,
    ENABLEGYROLDLPF_SUCCESS
} GYROSCOPE_STATUS_CODE;

// GYROSCOPE DATA PACKET
typedef struct
{
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    sensors_event_t mag;
} Gyroscope_Packet;

GYROSCOPE_STATUS_CODE gyroscope_init();
uint8_t gyroscope_getGyroRateDivisor();
GYROSCOPE_STATUS_CODE gyroscope_setGyroRateDivisor(const uint8_t &new_gyro_divisor);
uint16_t gyroscope_getAccelRateDivisor();
GYROSCOPE_STATUS_CODE gyroscope_setAccelRateDivisor(const uint16_t &new_accel_divisor);
icm20948_accel_range_t gyroscope_getAccelRange();
GYROSCOPE_STATUS_CODE gyroscope_setAccelRange(const icm20948_accel_range_t &new_accel_range);
icm20948_gyro_range_t gyroscope_getGyroRange();
GYROSCOPE_STATUS_CODE gyroscope_setGyroRange(const icm20948_gyro_range_t &new_gyro_range);
ak09916_data_rate_t gyroscope_getMagDataRate();
GYROSCOPE_STATUS_CODE gyroscope_setMagDataRate(const ak09916_data_rate_t &rate);
GYROSCOPE_STATUS_CODE gyroscope_enableAccelDLPF(const bool &enable, const icm20x_accel_cutoff_t &cutoff_freq);
GYROSCOPE_STATUS_CODE gyroscope_enableGyrolDLPF(const bool &enable, const icm20x_gyro_cutoff_t &cutoff_freq);
Gyroscope_Packet gyroscope_getData();
String gyroscope_message(const GYROSCOPE_STATUS_CODE &status_code);
String gyroscope_message(const char *msg);
String gyroscope_message(const Gyroscope_Packet &packet);

extern Adafruit_ICM20948 icm20948;
#endif // !__GYROSCOPE_H
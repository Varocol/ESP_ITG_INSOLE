#  Intelligent insole based on ESP32

###  Fusion of nine axis sensors with rich motion information

##  Features

- [Extremely advanced 9-axis sensor](#Extremely-advanced-9-axis-sensor)

- [3 capacity-to-digital converters with 12 channels and 16 bit accuracy](#3-capacity-to-digital-converters-with-12-channels-and-16-bit-accuracy)

- [Very safe multithreaded bus control system](#Very-safe-multithreaded-bus-control-system)

- [A relatively complete function menu that provides debugging functions](#A-relatively-complete-function-menu-that-provides-debugging-functions)

- [Supports Bluetooth Low Energy (BLE) for data transfer from all sensors](#Supports-Bluetooth-Low-Energy-BLE-for-data-transfer-from-all-sensors)

### Extremely advanced 9-axis sensor

The insole uses the ICM-20948 9-axis sensor, which is the world’s lowest power 9-axis  MotionTracking device that is ideally suited for Smartphones,  Tablets, Wearable Sensors, and IoT applications. 
- 3-axis gyroscope, 3-axis accelerometer, 3-axis  compass, and a Digital Motion Processor™ (DMPTM)  in a 3 mm x 3 mm x 1 mm (24-pin QFN) package
- DMP offloads computation of motion processing  algorithms from the host processor, improving  system power performance
- Software drivers are fully compliant with Google’s  latest Android release
- EIS FSYNC support

### 3 capacity-to-digital converters with 12 channels and 16 bit accuracy

AD7147 is a programmable capacitance-to-digital converter (CDC) ,with 12 channels and 16 bit accuracy.

##### Programmable capacitance-to-digital converter (CDC)
- Femtofarad resolution 
- 13 capacitance sensor inputs 
- 9 ms update rate, all 13 sensor inputs
- No external RC components required
- Automatic conversion sequencer

##### On-chip automatic calibration logic 
- Automatic compensation for environmental changes 
- Automatic adaptive threshold and sensitivity levels 

##### I2C-compatible serial interface

##### Low operating current 
- Full power mode: 1 mA 
- Low power mode: 21.5 μA

### Very safe multithreaded bus control system

Based on the time slice rotation system freeRTOS, and using the system's own resource mutex api to achieve time-sharing control of the bus, it can maximize the utilization of the I2C bus and have good data stability

### A relatively complete function menu that provides debugging functions

The function menu used to debug the sensor parameters in the insole, the following is an example of the function menu:

```
menu
 list all functions and descriptions

Pressure1_GetData
 return pressure1 sensors' datas

Pressure1_Calibrate
 calibrate pressure1 sensor

Pressure2_GetData
 return pressure2 sensors' datas

Pressure2_Calibrate
 calibrate pressure2 sensor

Pressure3_GetData
 return pressure3 sensors' datas

Pressure3_Calibrate
 calibrate pressure3 sensor

Gyroscope_GetGyroRateDivisor
 return value of gyro rate divisor

Gyroscope_SetGyroRateDivisor
   -gyroratedivisor (1~256)
 set the gyro rate divisor

Gyroscope_GetAccelRateDivisor
 return value of accel rate divisor

Gyroscope_SetAccelRateDivisor
   -accelratedivisor (1~65536)
 set the accel rate divisor

Gyroscope_GetAccelRange
 return the accel range

Gyroscope_SetAccelRange
   -accelrange 2|4|8|16
 return accel range

Gyroscope_GetGyroRange
 return the gyro range

Gyroscope_SetGyroRange
   -gyrorange 250|500|1000|2000
 set the gyro range

Gyroscope_GetMagDataRate
 return the mag data rate

Gyroscope_SetMagDataRate
   -magdatarate SHUTDOWN|SINGLE|10|20|50|100
 set mag data rate

Gyroscope_EnableAccelDLPF
   -enable ENABLE|DISABLE
   -cutoff_freq 437|246|111.4|50.4|23.9|11.5|5.7
 enable the accel DLPF(low pass filter)

Gyroscope_EnableGyrolDLPF
  -enable enable|disable
  -cutoff_freq 361.4|196.6|151.8|119.5|51.2|23.9|11.6|5.7
 enable the gyrol DLPF(low pass filter)

Gyroscope_GetData
 return the gyroscope datas

clear_nvs_namespace
 clear the nvs space

protocol_data
 return the packet consist of the data from all the sensors

show_task
 stow task that existed

stop_task
 -name (task name)
 stop task that existed
```

### Supports Bluetooth Low Energy (BLE) for data transfer from all sensors

Use ESP's low-power Bluetooth to transmit all sensor data packets, which are packaged in json format through the ArduinoJson api.

##  Features still to be implemented

- In order to use the system resource tracking function of freeRTOS, it is necessary to further improve the dynamic link library of freeRTOS

- Need to improve the encrypted part of the data packet transmitted through the Bluetooth low energy protocol
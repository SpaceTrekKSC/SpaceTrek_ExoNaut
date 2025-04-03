/*
 * ExoNaut_AccelGyro.h
 *
 * Author:    Andrew Gafford
 * Email:     agafford@spacetrek.com
 * Date:      February 26, 2025
 *
 * This library extends the ExoNaut library to provide accelerometer and gyroscope
 * functionality for the Space Trek ExoNaut Robot using an MPU6050 sensor.
 */

 #ifndef __EXONAUT_ACCELGYRO_H
 #define __EXONAUT_ACCELGYRO_H
 
 #include "ExoNaut.h"
 #include <Wire.h>
 
 // MPU6050 register addresses
 #define MPU6050_ADDR           0x68  // Address of Hiwonder MPU6050 (typically 0x68, but this one is at 0x78)
 #define MPU6050_REG_PWR_MGMT_1 0x6B
 #define MPU6050_REG_SMPLRT_DIV 0x19
 #define MPU6050_REG_CONFIG     0x1A
 #define MPU6050_REG_GYRO_CONFIG 0x1B
 #define MPU6050_REG_ACCEL_CONFIG 0x1C
 #define MPU6050_REG_ACCEL_XOUT_H 0x3B
 #define MPU6050_REG_TEMP_OUT_H 0x41
 #define MPU6050_REG_GYRO_XOUT_H 0x43
 #define MPU6050_REG_WHO_AM_I   0x75
 
 // IMU data structure
 typedef struct __imu_data_t {
     float accel_x;       // Acceleration in m/s² along X axis
     float accel_y;       // Acceleration in m/s² along Y axis
     float accel_z;       // Acceleration in m/s² along Z axis
     float gyro_x;        // Angular velocity in rad/s around X axis
     float gyro_y;        // Angular velocity in rad/s around Y axis
     float gyro_z;        // Angular velocity in rad/s around Z axis
     float temp;          // Temperature in degrees Celsius
     float pitch;         // Calculated pitch angle in degrees
     float roll;          // Calculated roll angle in degrees
     float yaw;           // Calculated yaw angle in degrees
     long last_update;    // Timestamp of last update
     bool calibrated;     // Whether the IMU has been calibrated
 } imu_data_t;
 
 class ExoNautAccelGyro : public exonaut {
 public:
     // Constructor
     ExoNautAccelGyro();
     
     // IMU functions
     bool beginIMU(void);
     bool updateIMU(void);
     void calibrateIMU(void);
     void getAcceleration(float *x, float *y, float *z);
     void getRotation(float *x, float *y, float *z);
     void getOrientation(float *pitch, float *roll, float *yaw);
     float getTemperature(void);
     
     // Motion detection
     bool detectShake(float threshold = 3.0);
     bool isFlat(float tolerance = 5.0);
     
 private:
     imu_data_t imu_data;                   // Current IMU data
     
     // Calibration variables
     float gyro_offset_x, gyro_offset_y, gyro_offset_z; // Gyroscope offsets
     unsigned long prev_imu_update_time;      // Time of last IMU update
     
     // Helper methods
     void calculateOrientation(void);         // Process raw values into orientation angles
     
     // MPU6050 specific methods
     void writeMPU6050Register(uint8_t reg, uint8_t value);
     uint8_t readMPU6050Register(uint8_t reg);
     void readMPU6050Data(int16_t* ax, int16_t* ay, int16_t* az, int16_t* temp, int16_t* gx, int16_t* gy, int16_t* gz);
 };
 
 extern imu_data_t imu_data;
 
 #endif // __EXONAUT_ACCELGYRO_H
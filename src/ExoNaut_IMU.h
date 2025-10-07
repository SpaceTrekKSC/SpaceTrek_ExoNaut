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

// ExoNaut_IMU.h
#ifndef EXONAUT_IMU_H
#define EXONAUT_IMU_H

#include <Wire.h>

class ExoNautIMU
{
public:
    bool start();
    void update();
    void calibrate();

    float getPitch();
    float getRoll();
    float getYaw();

    float getAccelX();
    float getAccelY();
    float getAccelZ();

    float getGyroX();
    float getGyroY();
    float getGyroZ();

    bool isShaking(float threshold = 1.5);

private:
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float pitch, roll, yaw;
    float gyroOffsetX, gyroOffsetY, gyroOffsetZ;
    unsigned long lastUpdate;

    void calculateOrientation();
    void readSensorData();
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
};

#endif // EXONAUT_IMU_H

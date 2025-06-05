/*
 * ExoNaut_AccelGyro.cpp
 *
 * Author:    Andrew Gafford
 * Email:     agafford@spacetrek.com
 * Date:      February 26, 2025
 *
 * Implementation for the ExoNaut_AccelGyro library with MPU6050 sensor.
 */

 #include "ExoNaut_IMU.h"
#include <math.h>

#define MPU6050_ADDR 0x68
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B

bool ExoNautIMU::start()
{
    Wire.begin();
    uint8_t whoAmI = readRegister(0x75);
    if (whoAmI != 0x68 && whoAmI != 0x78)
    {
        return false;
    }

    writeRegister(MPU6050_REG_PWR_MGMT_1, 0x00);
    gyroOffsetX = gyroOffsetY = gyroOffsetZ = 0;
    pitch = roll = yaw = 0;
    lastUpdate = millis();
    return true;
}

void ExoNautIMU::update()
{
    readSensorData();
    calculateOrientation();
}

void ExoNautIMU::calibrate()
{
    const int samples = 100;
    float sumX = 0, sumY = 0, sumZ = 0;

    for (int i = 0; i < samples; i++)
    {
        readSensorData();
        sumX += gyroX;
        sumY += gyroY;
        sumZ += gyroZ;
        delay(10);
    }

    gyroOffsetX = sumX / samples;
    gyroOffsetY = sumY / samples;
    gyroOffsetZ = sumZ / samples;
}

float ExoNautIMU::getPitch() { return pitch; }
float ExoNautIMU::getRoll() { return roll; }
float ExoNautIMU::getYaw() { return yaw; }

float ExoNautIMU::getAccelX() { return accelX; }
float ExoNautIMU::getAccelY() { return accelY; }
float ExoNautIMU::getAccelZ() { return accelZ; }

float ExoNautIMU::getGyroX() { return gyroX; }
float ExoNautIMU::getGyroY() { return gyroY; }
float ExoNautIMU::getGyroZ() { return gyroZ; }

bool ExoNautIMU::isShaking(float threshold)
{
    float mag = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);
    return (mag > threshold);
}

void ExoNautIMU::readSensorData()
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_REG_ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 14);

    int16_t ax = (Wire.read() << 8) | Wire.read();
    int16_t ay = (Wire.read() << 8) | Wire.read();
    int16_t az = (Wire.read() << 8) | Wire.read();
    Wire.read();
    Wire.read();
    int16_t gx = (Wire.read() << 8) | Wire.read();
    int16_t gy = (Wire.read() << 8) | Wire.read();
    int16_t gz = (Wire.read() << 8) | Wire.read();

    accelX = ax / 8192.0;
    accelY = ay / 8192.0;
    accelZ = az / 8192.0;

    gyroX = gx / 65.5 - gyroOffsetX;
    gyroY = gy / 65.5 - gyroOffsetY;
    gyroZ = gz / 65.5 - gyroOffsetZ;
}

void ExoNautIMU::calculateOrientation()
{
    unsigned long now = millis();
    float dt = (now - lastUpdate) / 1000.0f;
    if (dt > 0.2f)
        dt = 0.05f;

    float accPitch = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * 180.0 / M_PI;
    float accRoll = atan2(-accelX, accelZ) * 180.0 / M_PI;

    const float alpha = 0.96;
    pitch = alpha * (pitch + gyroX * dt) + (1 - alpha) * accPitch;
    roll = alpha * (roll + gyroY * dt) + (1 - alpha) * accRoll;
    yaw += gyroZ * dt;

    if (yaw < 0)
        yaw += 360;
    if (yaw >= 360)
        yaw -= 360;

    lastUpdate = now;
}

void ExoNautIMU::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t ExoNautIMU::readRegister(uint8_t reg)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 1);
    return Wire.read();
}

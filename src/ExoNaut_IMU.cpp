/*
 * ExoNaut_IMU.cpp
 *
 * Simple IMU library that automatically updates whenever functions are called.
 * Kids don't need to remember to call update()!
 */

#include "ExoNaut_IMU.h"
#include <math.h>

#define MPU6050_ADDR 0x68
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_WHO_AM_I 0x75
#define AUTO_UPDATE_INTERVAL 20 // Update every 20ms (50Hz)

bool ExoNautIMU::start()
{
    // Initialize I2C
    Wire.begin();
    Wire.setClock(100000);
    delay(100);

    // Check if MPU6050 is present
    uint8_t whoAmI = readRegister(MPU6050_REG_WHO_AM_I);
    if (whoAmI != 0x68 && whoAmI != 0x70)
    {
        Serial.print("IMU not found: 0x");
        Serial.println(whoAmI, HEX);
        return false;
    }

    // Wake up the MPU6050
    if (!writeRegister(MPU6050_REG_PWR_MGMT_1, 0x00))
    {
        Serial.println("Failed to wake up IMU");
        return false;
    }
    delay(100);

    // Configure sensor ranges for kid-friendly sensitivity
    writeRegister(0x1C, 0x08); // ±4g accelerometer
    writeRegister(0x1B, 0x08); // ±500°/s gyroscope

    // Initialize variables
    gyroOffsetX = gyroOffsetY = gyroOffsetZ = 0;
    pitch = roll = yaw = 0;
    lastUpdate = millis();
    lastAutoUpdate = 0;

    // Initialize orientation tracking
    currentPitch = PITCH_LEVEL;
    currentRoll = ROLL_FLAT;
    currentYaw = YAW_STRAIGHT;

    // Set kid-friendly thresholds (easier to detect)
    pitchThreshold = 25.0; // Easier to trigger "nose up/down"
    rollThreshold = 35.0;  // Easier to trigger "left/right side"
    yawThreshold = 20.0;   // Easier to trigger "turned left/right"

    // Initialize yaw reference
    initialYaw = 0;
    yawInitialized = false;

    Serial.println("IMU ready for kids!");
    return true;
}

void ExoNautIMU::calibrate()
{
    Serial.println("Calibrating IMU...");
    Serial.println("Keep robot still for 3 seconds!");

    // Countdown for kids
    for (int i = 3; i > 0; i--)
    {
        Serial.print(i);
        Serial.println("...");
        delay(1000);
    }

    Serial.println("Measuring...");

    const int samples = 100;
    float sumX = 0, sumY = 0, sumZ = 0;

    for (int i = 0; i < samples; i++)
    {
        readSensorData();
        sumX += gyroX;
        sumY += gyroY;
        sumZ += gyroZ;
        delay(10);

        if (i % 25 == 0)
            Serial.print(".");
    }
    Serial.println();

    gyroOffsetX = sumX / samples;
    gyroOffsetY = sumY / samples;
    gyroOffsetZ = sumZ / samples;

    // Set initial direction
    readSensorData();
    calculateOrientation();
    initialYaw = yaw;
    yawInitialized = true;

    Serial.println("Calibration complete!");
}

void ExoNautIMU::resetDirection()
{
    Serial.println("Setting new forward direction...");
    autoUpdate(); // Get current readings
    initialYaw = yaw;
    yawInitialized = true;
    Serial.println("New direction set!");
}

// ========================================
// AUTO-UPDATE SYSTEM
// ========================================

void ExoNautIMU::autoUpdate()
{
    if (needsUpdate())
    {
        readSensorData();
        calculateOrientation();

        // Update orientation states
        currentPitch = determinePitchState(pitch);
        currentRoll = determineRollState(roll);
        currentYaw = determineYawState(getRelativeYaw());

        lastAutoUpdate = millis();

        // Initialize yaw reference on first update
        if (!yawInitialized)
        {
            initialYaw = yaw;
            yawInitialized = true;
        }
    }
}

bool ExoNautIMU::needsUpdate()
{
    return (millis() - lastAutoUpdate) >= AUTO_UPDATE_INTERVAL;
}

// ========================================
// RAW VALUES - Exact Numbers
// ========================================

float ExoNautIMU::getPitchAngle()
{
    autoUpdate();
    return pitch;
}

float ExoNautIMU::getRollAngle()
{
    autoUpdate();
    return roll;
}

float ExoNautIMU::getYawAngle()
{
    autoUpdate();
    return yaw;
}

float ExoNautIMU::getTurnAngle()
{
    autoUpdate();
    return getRelativeYaw();
}

float ExoNautIMU::getPitchSpeed()
{
    autoUpdate();
    return gyroX;
}

float ExoNautIMU::getRollSpeed()
{
    autoUpdate();
    return gyroY;
}

float ExoNautIMU::getTurnSpeed()
{
    autoUpdate();
    return gyroZ;
}

float ExoNautIMU::getForceX()
{
    autoUpdate();
    return accelX;
}

float ExoNautIMU::getForceY()
{
    autoUpdate();
    return accelY;
}

float ExoNautIMU::getForceZ()
{
    autoUpdate();
    return accelZ;
}

// ========================================
// INTERPRETED VALUES - Easy to Understand
// ========================================

bool ExoNautIMU::isLevel()
{
    autoUpdate();
    return currentPitch == PITCH_LEVEL;
}

bool ExoNautIMU::isNoseUp()
{
    autoUpdate();
    return currentPitch == PITCH_NOSE_UP;
}

bool ExoNautIMU::isNoseDown()
{
    autoUpdate();
    return currentPitch == PITCH_NOSE_DOWN;
}

bool ExoNautIMU::isFlat()
{
    autoUpdate();
    return currentRoll == ROLL_FLAT;
}

bool ExoNautIMU::isOnLeftSide()
{
    autoUpdate();
    return currentRoll == ROLL_LEFT_SIDE;
}

bool ExoNautIMU::isOnRightSide()
{
    autoUpdate();
    return currentRoll == ROLL_RIGHT_SIDE;
}

bool ExoNautIMU::isFacingStraight()
{
    autoUpdate();
    return currentYaw == YAW_STRAIGHT;
}

bool ExoNautIMU::isTurnedLeft()
{
    autoUpdate();
    return currentYaw == YAW_TURNED_LEFT;
}

bool ExoNautIMU::isTurnedRight()
{
    autoUpdate();
    return currentYaw == YAW_TURNED_RIGHT;
}

String ExoNautIMU::getNoseDirection()
{
    autoUpdate();
    switch (currentPitch)
    {
    case PITCH_NOSE_UP:
        return "up";
    case PITCH_NOSE_DOWN:
        return "down";
    default:
        return "level";
    }
}

String ExoNautIMU::getSidePosition()
{
    autoUpdate();
    switch (currentRoll)
    {
    case ROLL_LEFT_SIDE:
        return "left side";
    case ROLL_RIGHT_SIDE:
        return "right side";
    default:
        return "flat";
    }
}

String ExoNautIMU::getTurnDirection()
{
    autoUpdate();
    switch (currentYaw)
    {
    case YAW_TURNED_LEFT:
        return "left";
    case YAW_TURNED_RIGHT:
        return "right";
    default:
        return "straight";
    }
}

bool ExoNautIMU::isShaking()
{
    autoUpdate();
    float totalForce = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);
    return totalForce > 1.3; // Kid-friendly threshold
}

bool ExoNautIMU::isMoving()
{
    autoUpdate();
    float totalRotation = abs(gyroX) + abs(gyroY) + abs(gyroZ);
    return totalRotation > 10; // Kid-friendly threshold
}

bool ExoNautIMU::isUpsideDown()
{
    autoUpdate();
    return accelZ < -0.7; // Z acceleration pointing up instead of down
}

// ========================================
// PRIVATE HELPER FUNCTIONS
// ========================================

void ExoNautIMU::readSensorData()
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_REG_ACCEL_XOUT_H);
    uint8_t error = Wire.endTransmission(false);

    if (error != 0)
    {
        return; // Silently fail for kid-friendly experience
    }

    uint8_t bytesReceived = Wire.requestFrom(MPU6050_ADDR, 14);
    if (bytesReceived != 14)
    {
        return; // Silently fail
    }

    // Read accelerometer data
    int16_t ax = (Wire.read() << 8) | Wire.read();
    int16_t ay = (Wire.read() << 8) | Wire.read();
    int16_t az = (Wire.read() << 8) | Wire.read();

    // Skip temperature data
    Wire.read();
    Wire.read();

    // Read gyroscope data
    int16_t gx = (Wire.read() << 8) | Wire.read();
    int16_t gy = (Wire.read() << 8) | Wire.read();
    int16_t gz = (Wire.read() << 8) | Wire.read();

    // Convert to meaningful units
    accelX = ax / 8192.0;
    accelY = ay / 8192.0;
    accelZ = az / 8192.0;

    gyroX = gx / 65.5 - gyroOffsetX;
    gyroY = gy / 65.5 - gyroOffsetY;
    gyroZ = -(gz / 65.5 - gyroOffsetZ); // Corrected yaw direction
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

    // Keep yaw in 0-360 range
    while (yaw < 0)
        yaw += 360;
    while (yaw >= 360)
        yaw -= 360;

    lastUpdate = now;
}

float ExoNautIMU::getRelativeYaw()
{
    if (!yawInitialized)
        return 0;

    float relativeYaw = yaw - initialYaw;
    while (relativeYaw > 180)
        relativeYaw -= 360;
    while (relativeYaw < -180)
        relativeYaw += 360;
    return relativeYaw;
}

// State determination functions
PitchState ExoNautIMU::determinePitchState(float pitch)
{
    if (pitch > pitchThreshold)
        return PITCH_NOSE_UP;
    else if (pitch < -pitchThreshold)
        return PITCH_NOSE_DOWN;
    else
        return PITCH_LEVEL;
}

RollState ExoNautIMU::determineRollState(float roll)
{
    if (roll > rollThreshold)
        return ROLL_RIGHT_SIDE;
    else if (roll < -rollThreshold)
        return ROLL_LEFT_SIDE;
    else
        return ROLL_FLAT;
}

YawState ExoNautIMU::determineYawState(float relativeYaw)
{
    if (relativeYaw > yawThreshold)
        return YAW_TURNED_RIGHT;
    else if (relativeYaw < -yawThreshold)
        return YAW_TURNED_LEFT;
    else
        return YAW_STRAIGHT;
}

// Hardware communication
bool ExoNautIMU::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(value);
    uint8_t error = Wire.endTransmission();
    return (error == 0);
}

uint8_t ExoNautIMU::readRegister(uint8_t reg)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    uint8_t error = Wire.endTransmission(false);

    if (error != 0)
        return 0xFF;

    uint8_t bytesReceived = Wire.requestFrom(MPU6050_ADDR, 1);
    if (bytesReceived != 1)
        return 0xFF;

    return Wire.read();
}

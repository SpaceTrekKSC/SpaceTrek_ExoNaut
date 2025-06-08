/*
 * ExoNaut_IMU.h - Kid-Friendly Version
 *
 * Author: Andrew Gafford  
 * Date: February 26, 2025
 *
 * Simple IMU library that automatically updates and provides easy functions
 * for kids to use in their robot programs.
 */

#ifndef EXONAUT_IMU_H
#define EXONAUT_IMU_H

#include <Arduino.h>
#include <Wire.h>

// Simple orientation states for kids
enum PitchState {
    PITCH_LEVEL,
    PITCH_NOSE_UP,
    PITCH_NOSE_DOWN
};

enum RollState {
    ROLL_FLAT,
    ROLL_LEFT_SIDE,
    ROLL_RIGHT_SIDE
};

enum YawState {
    YAW_STRAIGHT,
    YAW_TURNED_LEFT,
    YAW_TURNED_RIGHT
};

class ExoNautIMU
{
public:
    // Setup functions
    bool start();
    void calibrate();
    void resetDirection();  // Kid-friendly name for resetYawReference
    
    // ========================================
    // RAW VALUES - Exact Numbers
    // ========================================
    
    // Angles (in degrees)
    float getPitchAngle();     // How tilted forward/back (-180 to +180)
    float getRollAngle();      // How tilted left/right (-180 to +180)  
    float getYawAngle();       // Which direction facing (0 to 360)
    float getTurnAngle();      // How much turned from start (-180 to +180)
    
    // Motion (how fast rotating, in degrees per second)
    float getPitchSpeed();     // How fast tilting forward/back
    float getRollSpeed();      // How fast tilting left/right
    float getTurnSpeed();      // How fast turning left/right
    
    // Forces (in g-force, 1g = gravity)
    float getForceX();         // Side force (left/right)
    float getForceY();         // Forward force (forward/back)
    float getForceZ();         // Up force (up/down, usually gravity)
    
    // ========================================
    // INTERPRETED VALUES - Easy to Understand
    // ========================================
    
    // Simple true/false questions kids can ask
    bool isLevel();            // Is the robot flat/level?
    bool isNoseUp();           // Is the nose tilted up?
    bool isNoseDown();         // Is the nose tilted down?
    
    bool isFlat();             // Is the robot standing upright?
    bool isOnLeftSide();       // Is the robot on its left side?
    bool isOnRightSide();      // Is the robot on its right side?
    
    bool isFacingStraight();   // Is robot facing the starting direction?
    bool isTurnedLeft();       // Has robot turned left?
    bool isTurnedRight();      // Has robot turned right?
    
    // Simple descriptive words
    String getNoseDirection();    // Returns "up", "down", or "level"
    String getSidePosition();     // Returns "flat", "left side", or "right side"
    String getTurnDirection();    // Returns "straight", "left", or "right"
    
    // Utility functions
    bool isShaking();             // Is the robot shaking or vibrating?
    bool isMoving();              // Is the robot currently moving/rotating?
    bool isUpsideDown();          // Is the robot upside down?

private:
    // Internal sensor data
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float pitch, roll, yaw;
    float gyroOffsetX, gyroOffsetY, gyroOffsetZ;
    unsigned long lastUpdate;

    // Orientation tracking
    PitchState currentPitch;
    RollState currentRoll;
    YawState currentYaw;
    
    // Reference direction
    float initialYaw;
    bool yawInitialized;
    
    // Thresholds (kid-friendly defaults)
    float pitchThreshold;
    float rollThreshold;
    float yawThreshold;
    
    // Auto-update system
    void autoUpdate();         // Automatically called by all getter functions
    bool needsUpdate();        // Check if update is needed
    unsigned long lastAutoUpdate;
    
    // Core functions
    void readSensorData();
    void calculateOrientation();
    bool writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    
    // State determination
    PitchState determinePitchState(float pitch);
    RollState determineRollState(float roll);
    YawState determineYawState(float relativeYaw);
    
    // Helper functions
    float getRelativeYaw();
    float constrainAngle(float angle);
};

#endif // EXONAUT_IMU_H
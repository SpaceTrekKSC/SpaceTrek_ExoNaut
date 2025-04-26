/*
 * ExoNaut_AICamLF.h
 *
 * Author:Andrew Gafford
 * Date: March 12, 2025 (Updated April 2025 for simplified line following)
 *
 * This library extends the ExoNaut_AICam to provide simplified line following
 * functionality for the Space Trek ExoNaut Robot.
 */

 #ifndef EXONAUT_AICAMLF_H
 #define EXONAUT_AICAMLF_H
 
 #include <Arduino.h>
 #include "ExoNaut.h"
 #include "ExoNaut_AICam.h"
 
 // Line detection parameters
 #define MAX_LINE_IDS 4           // Maximum number of line IDs the camera can track
 #define LINE_FOLLOW_CENTER 160   // Center point of the camera view (horizontal)
 #define LINE_FOLLOW_THRESHOLD 20 // Threshold for line offset consideration
 
 // Line status constants
 #define LINE_STATUS_NONE 0     // No line detected
 #define LINE_STATUS_CENTERED 1 // Line is centered
 #define LINE_STATUS_LEFT 2     // Line is to the left
 #define LINE_STATUS_RIGHT 3    // Line is to the right
 #define LINE_STATUS_LOST 4     // Line was detected but now lost
 
 // Internal tuning constants
 #define SCAN_INTERVAL_MS 20        // Time between scans
 #define TURN_FACTOR 20.0f          // Turn strength
 #define ANGLE_WEIGHT 0.5f          // Angle influence weight
 #define OFFSET_WEIGHT 1.0f         // Start X influence weight
 #define SHARP_TURN_THRESHOLD 45    // Degrees beyond which slow down
 #define SLOW_SPEED 20.0f           // Slower speed for sharp turns
 #define PIVOT_SPEED 20.0f          // Speed for pivot recovery
 #define LOST_RECOVERY_TIMEOUT 1000 // Max ms to try pivoting
 
 class ExoNaut_AICamLF
 {
 public:
     ExoNaut_AICamLF();
 
     // Initialize the line follower
     bool begin(exonaut *robot, ExoNaut_AICam *camera);
 
     // Update line detection and get latest data
     bool update();
 
     // Get line status (none, centered, left, right, lost)
     uint8_t getLineStatus(uint8_t lineId = 1);
 
     // Get line angle (-90 to 90 degrees)
     int16_t getLineAngle(uint8_t lineId = 1);
 
     // Get line offset from center (-160 to 160)
     int16_t getLineOffset(uint8_t lineId = 1);
 
     // Get the full line data structure
     bool getLineData(uint8_t lineId, WonderCamLineResult *data);
 
     // Check if specific line ID is detected
     bool isLineDetected(uint8_t lineId = 1);
 
     // Get number of lines detected
     uint8_t getLineCount();
 
     // Advanced manual control helper
     void followLine(uint8_t lineId, float baseSpeed, float turnFactor);
 
     // --- Simplified functions for beginners ---
 
     // Set base driving speed
     void setBaseSpeed(float speed);
 
     // Simple line follower with auto turn, slow down, recovery
     void simpleFollowLine();
 
 private:
     exonaut *_robot;
     ExoNaut_AICam *_camera;
     bool _initialized;
     uint8_t _lastLineStatus[MAX_LINE_IDS + 1];
     float _baseSpeed;
 };
 
 #endif // EXONAUT_AICAMLF_H
 
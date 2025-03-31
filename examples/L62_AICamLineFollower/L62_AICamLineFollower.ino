/*
 * L62AICamLineFollower.ino
 *
 * This sketch demonstrates how to use the AI camera line following 
 * functionality on the Space Trek ExoNaut Robot.
 * 
 * The program detects lines with the camera and controls the robot's
 * movement to follow the line, including strategies for handling
 * lost lines and recovering the path.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 31, 2025
 *
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                 //This command creates an AI Camera object
 *                                       //Provides access to the camera's computer vision functions
 *
 * ExoNaut_AICamLF lineFollower;         //This command creates a specialized line follower object
 *                                       //Wraps camera line detection with advanced following algorithms
 *
 * robot.begin();                        //This command initializes the robot systems
 *
 * lineFollower.begin(&robot, &camera);  //This command initializes the line follower with robot and camera references
 *                                       //Returns true if initialization was successful
 *
 * lineFollower.update();                //This command updates the line follower data
 *                                       //Should be called regularly in the loop
 *
 * lineFollower.getLineStatus(lineId);   //This command gets the current status of a specific line
 *                                       //Returns LINE_STATUS_CENTERED, LINE_STATUS_LEFT, etc.
 *
 * lineFollower.isLineDetected(lineId);  //This command checks if a specific line ID is detected
 *                                       //Returns true if the line is visible, false otherwise
 *
 * lineFollower.getLineData(lineId, &data); //This command gets detailed data about a detected line
 *                                          //Fills the provided WonderCamLineResult structure
 *
 * lineFollower.getLineOffset(lineId);   //This command gets the offset of a line from center
 *                                       //Negative values mean left, positive values mean right
 *
 * robot.set_motor_speed(left, right);   //This command sets the motor speeds
 *                                       //Parameters are for left and right motors (-100 to 100)
 */
#include "ExoNaut.h"
#include "ExoNaut_AICam.h"
#include "ExoNaut_AICamLF.h"

// Create instances of the required classes
exonaut robot;
ExoNaut_AICam camera;
ExoNaut_AICamLF lineFollower;

// Line following parameters
#define LINE_ID 1             // ID of the line we want to follow (typically 1)
#define BASE_SPEED 20.0f      // Base motor speed (0-100)
#define TURN_FACTOR 10.0f     // How strongly to respond to line deviations
#define SCAN_INTERVAL 50      // Milliseconds between line position updates
#define LOST_SPEED 20.0f      // Slower speed when trying to find a lost line

// Status tracking
unsigned long lastScanTime = 0;
uint8_t lastLineStatus = LINE_STATUS_NONE;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ExoNaut Line Following Demo");
  
  // Initialize the robot
  robot.begin();
  delay(1000);
  
  // Initialize the line follower
  if (!lineFollower.begin(&robot, &camera)) {
    Serial.println("Failed to initialize line follower. Check connections!");
    while(1); // Stop if initialization fails
  }
  
  Serial.println("Line follower initialized successfully!");
  Serial.println("Waiting for a line to be detected...");
}

void loop() {
  // Check if it's time to update the line position
  if (millis() - lastScanTime >= SCAN_INTERVAL) {
    lastScanTime = millis();
    
    // Update line follower data
    lineFollower.update();
    
    // Get current line status
    uint8_t lineStatus = lineFollower.getLineStatus(LINE_ID);
    
    // Print line information if status changed
    if (lineStatus != lastLineStatus) {
      printLineStatus(lineStatus);
      lastLineStatus = lineStatus;
    }
    
    // Control robot based on line position
    controlRobot(lineStatus);
  }
}

void controlRobot(uint8_t lineStatus) {
  switch (lineStatus) {
    case LINE_STATUS_CENTERED:
      // Line is centered - move forward at base speed
      robot.set_motor_speed(BASE_SPEED, BASE_SPEED);
      break;
      
    case LINE_STATUS_LEFT:
      // Line is to the left - turn left
      robot.set_motor_speed(BASE_SPEED - TURN_FACTOR, BASE_SPEED + TURN_FACTOR);
      break;
      
    case LINE_STATUS_RIGHT:
      // Line is to the right - turn right
      robot.set_motor_speed(BASE_SPEED + TURN_FACTOR, BASE_SPEED - TURN_FACTOR);
      break;
      
    case LINE_STATUS_LOST:
      // Line was lost - slow down and seek
      searchForLine();
      break;
      
    default:
      // No line or unknown status - stop and scan
      if (!scanForLine()) {
        // If scanning doesn't find a line, stop
        robot.set_motor_speed(0, 0);
      }
      break;
  }
}

// Function to search for a lost line
void searchForLine() {
  // Get last detected offset to determine which way to turn
  int16_t lastOffset = lineFollower.getLineOffset(LINE_ID);
  
  if (lastOffset < 0) {
    // Line was last seen on the left, so turn left to find it
    robot.set_motor_speed(0, LOST_SPEED);
  } else {
    // Line was last seen on the right, so turn right to find it
    robot.set_motor_speed(LOST_SPEED, 0);
  }
}

// Function to scan for a line by rotating in place
bool scanForLine() {
  static int scanDirection = 1;  // 1 for right, -1 for left
  static unsigned long scanStartTime = 0;
  
  // If we just started scanning
  if (scanStartTime == 0) {
    scanStartTime = millis();
  }
  
  // Check if we've been scanning for too long (3 seconds)
  if (millis() - scanStartTime > 3000) {
    // Switch direction
    scanDirection *= -1;
    scanStartTime = millis();
  }
  
  // Rotate in place slowly
  robot.set_motor_speed(LOST_SPEED * scanDirection, -LOST_SPEED * scanDirection);
  
  // Check if we found a line during scanning
  if (lineFollower.isLineDetected(LINE_ID)) {
    // Reset scan timer
    scanStartTime = 0;
    return true;
  }
  
  return false;
}

// Function to print line status information
void printLineStatus(uint8_t status) {
  Serial.print("Line status: ");
  
  switch (status) {
    case LINE_STATUS_NONE:
      Serial.println("NONE - No line detected");
      break;
    case LINE_STATUS_CENTERED:
      Serial.println("CENTERED - Line is in the center");
      break;
    case LINE_STATUS_LEFT:
      Serial.println("LEFT - Line is to the left");
      break;
    case LINE_STATUS_RIGHT:
      Serial.println("RIGHT - Line is to the right");
      break;
    case LINE_STATUS_LOST:
      Serial.println("LOST - Line was detected but now lost");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }
  
  // If a line is detected, print additional info
  if (lineFollower.isLineDetected(LINE_ID)) {
    WonderCamLineResult lineData;
    if (lineFollower.getLineData(LINE_ID, &lineData)) {
      Serial.print("  Angle: ");
      Serial.print(lineData.angle);
      Serial.print(" degrees, Offset: ");
      Serial.println(lineData.offset);
    }
  }
}

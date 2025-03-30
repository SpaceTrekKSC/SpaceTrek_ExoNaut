
/*
 * L20_LineFollower.ino
 *
 * Example sketch demonstrating line following functionality
 * using the ExoNaut robot with AI Camera
 * 
 * This example uses the ExoNaut_AICamLF class to simplify line following
 * tasks with the Space Trek ExoNaut Robot.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com 
 * Date: March 30, 2025
 * 
 * Hardware Requirements:
 * - Space Trek ExoNaut Robot
 * - AI Camera mounted on the robot
 * - Black line on white background (or white line on black background)
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
 * camera.begin();                       //This command initializes the camera module
 *
 * lineFollower.begin(&robot, &camera);  //This command initializes the line follower with robot and camera references
 *                                       //Returns true if initialization was successful
 *
 * lineFollower.update();                //This command updates the camera data
 *                                       //Should be called regularly before using line detection functions
 *
 * lineFollower.isLineDetected(lineId);  //This command checks if a specific line ID is currently detected
 *                                       //Returns true if the line is visible, false otherwise
 *
 * lineFollower.getLineStatus(lineId);   //This command returns the status of a specific line
 *                                       //Returns LINE_STATUS_CENTERED, LINE_STATUS_LEFT, etc.
 *
 * lineFollower.followLine(lineId, speed, factor); //This command follows a line using built-in algorithms
 *                                                 //Parameters: line ID, base speed, turning factor
 *
 * lineFollower.printLineData(lineId);   //This command prints line data to the serial monitor
 *                                       //Useful for debugging line following behavior
 *
 * robot.set_motor_speed(left, right);   //This command sets the motor speeds
 *                                       //Parameters are for left and right motors (-100 to 100)
 *
 * robot.setColorAll(r, g, b);           //This command sets all LEDs to the same color
 *                                       //Parameters: red, green, blue values (0-255)
 *
 * robot.clear();                        //This command turns off all LEDs
 */

#include "ExoNaut.h"
#include "ExoNaut_AICam.h"
#include "ExoNaut_AICamLF.h"

// Create instances of the required classes
exonaut robot;
ExoNaut_AICam camera;
ExoNaut_AICamLF lineFollower;

// Line following parameters
#define LINE_ID 1               // ID of the line to follow (configure in AI Camera)
#define BASE_SPEED 45           // Base motor speed (0-100)
#define TURN_FACTOR 30          // How aggressively to turn (higher = more aggressive)
#define UPDATE_INTERVAL 50      // Update interval in milliseconds

// LED indicator colors
#define LED_LINE_DETECTED 0, 255, 0    // Green when line is detected
#define LED_LINE_LOST 255, 0, 0        // Red when line is lost
#define LED_CENTERED 0, 0, 255         // Blue when line is centered

// Timing variables
unsigned long lastUpdateTime = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ExoNaut Line Follower Example");
  
  // Initialize the robot
  robot.begin();
  
  // Initialize the AI Camera
  camera.begin();
  
  // Initialize line follower
  if (!lineFollower.begin(&robot, &camera)) {
    Serial.println("Failed to initialize line follower!");
    
    // Show error with red LEDs
    robot.setColorAll(255, 0, 0);
    
    // Stop execution
    while (1) {
      delay(100);
    }
  }
  
  Serial.println("Line follower initialized");
  
  // Show startup pattern on LEDs
  startupLedSequence();
  
  // Wait a moment for everything to stabilize
  delay(1000);
}

void loop() {
  // Update at regular intervals
  if (millis() - lastUpdateTime >= UPDATE_INTERVAL) {
    lastUpdateTime = millis();
    
    // Update camera data
    lineFollower.update();
    
    // Check if line is detected
    if (lineFollower.isLineDetected(LINE_ID)) {
      // Get line status
      uint8_t lineStatus = lineFollower.getLineStatus(LINE_ID);
      
      // Update LED based on line status
      updateLedIndicator(lineStatus);
      
      // Print line data every 20 updates (about once per second)
      static int updateCount = 0;
      if (updateCount++ % 20 == 0) {
        lineFollower.printLineData(LINE_ID);
      }
      
      // Follow the line using the library's built-in line following function
      lineFollower.followLine(LINE_ID, BASE_SPEED, TURN_FACTOR);
    } 
    else {
      // Line not detected
      Serial.println("Line not detected");
      
      // Set indicator LEDs to red
      robot.setColorAll(LED_LINE_LOST);
      
      // Stop the robot
      robot.set_motor_speed(0, 0);
      
      // Alternative: You could implement a search pattern here
      // searchForLine();
    }
  }
  
  // Check if button A is pressed
  if (digitalRead(BUTTON_A_PIN) == LOW) {
    // Button A pressed - stop the robot
    Serial.println("Button A pressed - stopping");
    robot.set_motor_speed(0, 0);
    
    // Flash LEDs
    for (int i = 0; i < 3; i++) {
      robot.setColorAll(255, 255, 255);
      delay(100);
      robot.setColorAll(0, 0, 0);
      delay(100);
    }
    
    // Wait for button release
    while (digitalRead(BUTTON_A_PIN) == LOW) {
      delay(10);
    }
  }
  
  // Check if button B is pressed
  if (digitalRead(BUTTON_B_PIN) == LOW) {
    // Button B pressed - print all detected lines
    Serial.println("Button B pressed - displaying all lines");
    lineFollower.printAllLines();
    
    // Wait for button release
    while (digitalRead(BUTTON_B_PIN) == LOW) {
      delay(10);
    }
  }
}

// Update the LED indicators based on line status
void updateLedIndicator(uint8_t lineStatus) {
  switch (lineStatus) {
    case LINE_STATUS_CENTERED:
      // Blue when centered
      robot.setColorAll(LED_CENTERED);
      break;
    
    case LINE_STATUS_LEFT:
      // Amber when line is to the left
      robot.setColorAll(255, 128, 0);
      break;
    
    case LINE_STATUS_RIGHT:
      // Purple when line is to the right
      robot.setColorAll(255, 0, 255);
      break;
    
    case LINE_STATUS_LOST:
      // Red when line is lost
      robot.setColorAll(LED_LINE_LOST);
      break;
    
    default:
      // White for other states
      robot.setColorAll(100, 100, 100);
      break;
  }
}

// LED startup sequence
void startupLedSequence() {
  // Clear all LEDs
  robot.clear();
  
  // Sequentially light up each LED
  for (int i = 0; i < NUM_PIXELS; i++) {
    robot.setColor(i, 0, 255, 0);  // Green
    robot.show();
    delay(100);
  }
  
  // Flash all LEDs
  for (int i = 0; i < 3; i++) {
    robot.setColorAll(0, 0, 0);  // Off
    robot.show();
    delay(100);
    robot.setColorAll(0, 0, 255);  // Blue
    robot.show();
    delay(100);
  }
  
  // Set all LEDs to green
  robot.setColorAll(0, 255, 0);
}

// Advanced: Search pattern for finding the line again
void searchForLine() {
  static int searchState = 0;
  static unsigned long searchStartTime = 0;
  
  // Start a new search if not already searching
  if (searchState == 0) {
    searchStartTime = millis();
    searchState = 1;
  }
  
  // Time-based searching pattern
  unsigned long searchTime = millis() - searchStartTime;
  
  if (searchTime < 1000) {
    // First, rotate slowly clockwise
    robot.set_motor_speed(-20, 20);
  } 
  else if (searchTime < 3000) {
    // Then, rotate counter-clockwise for longer
    robot.set_motor_speed(20, -20);
  } 
  else if (searchTime < 4000) {
    // Return to clockwise rotation
    robot.set_motor_speed(-20, 20);
  } 
  else {
    // Reset search if line not found after timeout
    searchState = 0;
    robot.set_motor_speed(0, 0);
  }
  
  // Check if line was found during search
  if (lineFollower.isLineDetected(LINE_ID)) {
    searchState = 0;  // Reset search state
  }
}

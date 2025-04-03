/*
 * L15_Color_Detection_Control.ino
 *
 * This example demonstrates using the ExoNaut AI Camera for color
 * detection to control robot movement. The program detects specific
 * colors and responds with appropriate movement commands.
 *
 * Color ID 1: Robot moves forward
 * Color ID 2: Robot stops
 * No colors detected: Robot stops
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * Commands:
 * exonaut robot;                       //This command creates the main robot instance
 *                                      //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                //This command creates an AI Camera object called 'camera'
 *                                      //This is the object that handles vision detection functions
 *
 * camera.begin();                      //This command initializes the camera module and sets up I2C communication
 *
 * camera.changeFunc(mode);             //This command changes the camera's operating mode
 *                                      //We use APPLICATION_COLORDETECT for color detection
 *
 * camera.setLed(state);                //This command turns the camera's LED on or off
 *                                      //WONDERCAM_LED_ON turns the LED on for better detection
 *
 * camera.updateResult();               //This command updates all recognition results from the camera
 *                                      //Should be called regularly in the loop
 *
 * camera.anyColorDetected();           //This command checks if any color has been detected
 *
 * camera.numOfColorDetected();         //This command returns the number of colors detected
 *
 * camera.colorIdDetected(id);          //This command checks if a specific color ID has been detected
 *                                      //Color IDs range from 1-7 for different colors
 *
 * robot.set_motor_speed(left, right);  //This command sets both motor speeds
 *                                      //Parameters: left motor speed, right motor speed
 */

#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

// Create instances of ExoNaut and ExoNaut_AICam
exonaut robot;
ExoNaut_AICam camera;

// Movement parameters
const float FORWARD_SPEED = 30.0;  // Speed when moving forward
const float STOP_SPEED = 0.0;      // Speed when stopping

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize the robot
  robot.begin();
  
  // Initialize the camera
  camera.begin();
  
  // Change camera function to color detection
  camera.changeFunc(APPLICATION_COLORDETECT);
  
  // Turn on camera LED
  camera.setLed(WONDERCAM_LED_ON);
  
  // Brief delay to allow camera to initialize
  delay(1000);
}

void loop() {
  // Update camera results
  camera.updateResult();
  
  // Check if any color is detected
  if (camera.anyColorDetected()) {
    // Get number of detected colors
    int colorCount = camera.numOfColorDetected();
    Serial.print("Colors detected: ");
    Serial.println(colorCount);
    
    // Check for specific color IDs
    if (camera.colorIdDetected(1)) {
      // Color ID 1 detected - move forward
      Serial.println("Color ID 1 detected - Moving Forward");
      robot.set_motor_speed(FORWARD_SPEED, FORWARD_SPEED);
    }
    else if (camera.colorIdDetected(2)) {
      // Color ID 2 detected - stop
      Serial.println("Color ID 2 detected - Stopping");
      robot.set_motor_speed(STOP_SPEED, STOP_SPEED);
    }
  }
  else {
    // No colors detected - optional default behavior
    Serial.println("No colors detected");
    robot.set_motor_speed(STOP_SPEED, STOP_SPEED);
  }
  
  // Small delay to prevent overwhelming the system
  delay(100);
}

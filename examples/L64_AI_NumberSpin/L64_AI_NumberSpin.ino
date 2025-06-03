/**************************************************
 * 64_AI_NumberSpin.ino
 * A simplified program to detect landmarks and numbers, then spin for a specified duration
 *
 * The camera module is an i2c device. It must be plugged
 * into port 3, 4, 5 or 9. It will not work in any other ports.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: May 29, 2025
 * 
 * Program Flow:
 * Station 1: Landmark recognition (ID 1 = clockwise spin, ID 2 = counter-clockwise spin)
 * Station 2: Number recognition (1-5, determines spin duration in seconds)
 * Station 3: Spin in the direction determined by landmark for duration determined by number
 * 
 * Commands:
 * exonaut robot;                        //Creates the main robot instance
 *                                       //Handles motors and core functions
 *
 * ExoNaut_AICam camera;                 //Creates an AI Camera object
 *                                       //Handles vision-based detection and recognition
 *
 * robot.begin();                        //Initializes the robot systems
 *
 * camera.begin();                       //Initializes the camera module
 *
 * robot.set_motor_speed(left, right);   //Sets the motor speeds
 *                                       //Parameters are for left and right motors (-100 to 100)
 *
 * camera.changeFunc(mode);              //Changes the camera's operating mode
 *                                       //Modes: APPLICATION_LANDMARK, APPLICATION_NUMBER_REC
 *
 * camera.updateResult();                //Updates recognition results from the camera
 *
 * camera.landmarkIdWithMaxProb();       //Returns the landmark ID with highest probability
 *
 * camera.landmarkMaxProb();             //Returns the confidence level of the detected landmark
 *
 * camera.numberWithMaxProb();           //Returns the recognized number with highest probability
 *
 * camera.numberMaxProb();               //Returns the confidence level of the recognized number
 *
 * robot.setColorAll(r, g, b);           //Sets the color of all onboard NeoPixel LEDs
 *                                       //Parameters are RGB values (0-255)
 **************************************************/

#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

// Create robot and camera objects
exonaut robot;
ExoNaut_AICam camera;

// Simple state tracking
int step = 1;  // 1=landmarks, 2=numbers, 3=spinning
float spinDirection = 0;  // Will be 30 or -30
unsigned long spinEndTime = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize everything
  robot.begin();
  camera.begin();
  camera.setLed(true);
  
  // Start looking for landmarks
  camera.changeFunc(APPLICATION_LANDMARK);
  robot.setColorAll(0, 0, 255);  // Blue = looking for landmarks
  robot.show();
  
  Serial.println("Looking for landmarks...");
}

void loop() {
  camera.updateResult();
  
  if (step == 1) {
    // Step 1: Look for landmarks
    int landmark = camera.landmarkIdWithMaxProb();
    float confidence = camera.landmarkMaxProb();
    
    if (confidence > 0.5) {  // 50% confidence
      if (landmark == 1) {
        spinDirection = 30;   // Clockwise
        Serial.println("Landmark 1: Will spin clockwise");
      } else if (landmark == 2) {
        spinDirection = -30;  // Counter-clockwise  
        Serial.println("Landmark 2: Will spin counter-clockwise");
      }
      
      if (spinDirection != 0) {
        // Switch to number recognition
        camera.changeFunc(APPLICATION_NUMBER_REC);
        robot.setColorAll(0, 255, 0);  // Green = looking for numbers
        robot.show();
        step = 2;
        Serial.println("Now looking for numbers...");
        delay(500);
      }
    }
    
  } else if (step == 2) {
    // Step 2: Look for numbers
    int number = camera.numberWithMaxProb();
    float confidence = camera.numberMaxProb();
    
    if (confidence > 0.7 && number >= 1 && number <= 5) {  // 70% confidence, valid range
      // Start spinning
      robot.set_motor_speed(spinDirection, -spinDirection);
      robot.setColorAll(255, 0, 0);  // Red = spinning
      robot.show();
      
      spinEndTime = millis() + (number * 1000);  // Spin for 'number' seconds
      step = 3;
      
      Serial.print("Spinning for ");
      Serial.print(number);
      Serial.println(" seconds");
    }
    
  } else if (step == 3) {
    // Step 3: Spinning
    if (millis() >= spinEndTime) {
      // Stop and reset
      robot.set_motor_speed(0, 0);
      robot.setColorAll(0, 0, 255);  // Back to blue
      robot.show();
      
      // Reset for next cycle
      camera.changeFunc(APPLICATION_LANDMARK);
      step = 1;
      spinDirection = 0;
      
      Serial.println("Done spinning. Looking for landmarks again...");
      delay(500);
    }
  }
  
  delay(100);  // Small delay
}
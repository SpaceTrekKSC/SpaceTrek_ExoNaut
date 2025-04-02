/**************************************************
 * ExoNaut_AI_Demo.ino
 * A program to detect landmarks and numbers, then spin for a specified duration
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2, 2025
 * 
 * Program Flow:
 * Station 1: Landmark recognition (ID 1 = clockwise spin, ID 2 = counter-clockwise spin)
 * Station 2: Number recognition (1-5, determines spin duration in seconds)
 * Station 3: Spin in the direction determined by landmark for duration determined by number
 * 
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                 //This command creates an AI Camera object
 *                                       //Handles vision-based detection and recognition
 *
 * robot.begin();                        //This command initializes the robot systems
 *
 * camera.begin();                       //This command initializes the camera module
 *
 * robot.stop_motor(motorID);            //This command stops the specified motors
 *                                       //0 = both motors, 1 = left motor, 2 = right motor
 *
 * camera.changeFunc(mode);              //This command changes the camera's operating mode
 *                                       //Different modes include: APPLICATION_LANDMARK, 
 *                                       //APPLICATION_NUMBER_REC
 *
 * camera.updateResult();                //This command updates recognition results from the camera
 *
 * camera.landmarkIdDetected(id);        //This command checks if a specific landmark ID is detected
 *
 * camera.numberWithMaxProb();           //This command returns the recognized number with highest probability
 *
 * camera.numberMaxProb();               //This command returns the confidence level of the recognized number
 *
 * robot.set_motor_speed(left, right);   //This command sets the motor speeds
 *                                       //Parameters are for left and right motors (-100 to 100)
 *
 * robot.setColorAll(r, g, b);           //This command sets the color of all onboard NeoPixel LEDs
 *                                       //Parameters are RGB values (0-255)
 **************************************************/

#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

// Create instances of needed classes
exonaut robot;
ExoNaut_AICam camera;

// Constants for motion control
const int SECOND_DURATION = 1000;    // Duration of one second in milliseconds
const float SPIN_SPEED = 30.0;       // Motor speed for spinning
const int MAX_NUMBER = 5;            // Maximum number to recognize

// LED indicator constants
const int NUM_LEDS = 6;              // Number of onboard NeoPixels
// Colors for different modes
const uint8_t LANDMARK_COLOR_R = 0;    // Blue for landmark recognition
const uint8_t LANDMARK_COLOR_G = 0;
const uint8_t LANDMARK_COLOR_B = 255;

const uint8_t NUMBER_COLOR_R = 0;      // Green for number recognition
const uint8_t NUMBER_COLOR_G = 255;
const uint8_t NUMBER_COLOR_B = 0;

const uint8_t SPINNING_COLOR_R = 255;  // Red for spinning
const uint8_t SPINNING_COLOR_G = 0;
const uint8_t SPINNING_COLOR_B = 0;

// States for the state machine
enum State {
  LANDMARK_RECOGNITION,
  NUMBER_RECOGNITION,
  SPINNING
};

// Current state
State currentState = LANDMARK_RECOGNITION;

// Variables to track timing and motion
unsigned long spinStartTime = 0;
unsigned long spinDuration = 0;
float leftMotorSpeed = 0;
float rightMotorSpeed = 0;
bool directionDetermined = false;

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  Serial.println("ExoNaut AI Camera Demo");
  
  // Initialize the robot
  robot.begin();
  
  // Initialize the camera
  camera.begin();
  
  // Start with landmark recognition mode
  camera.changeFunc(APPLICATION_LANDMARK);
  
  // Turn on the camera LED
  camera.setLed(WONDERCAM_LED_ON);
  
  // Set initial LED state - blue for landmark recognition
  updateLEDs();
  
  // Wait for initialization
  delay(1000);
  
  Serial.println("System initialized. Starting landmark recognition mode.");
}

void loop() {
  // Update the camera data
  camera.updateResult();
  
  // State machine
  switch (currentState) {
    case LANDMARK_RECOGNITION:
      handleLandmarkRecognition();
      break;
      
    case NUMBER_RECOGNITION:
      handleNumberRecognition();
      break;
      
    case SPINNING:
      handleSpinning();
      break;
  }
  
  // Brief delay to prevent overwhelming the system
  delay(50);
}

void updateLEDs() {
  // Set all LEDs to the color corresponding to the current state
  switch (currentState) {
    case LANDMARK_RECOGNITION:
      // Blue for landmark recognition
      robot.setColorAll(LANDMARK_COLOR_R, LANDMARK_COLOR_G, LANDMARK_COLOR_B);
      break;
      
    case NUMBER_RECOGNITION:
      // Green for number recognition
      robot.setColorAll(NUMBER_COLOR_R, NUMBER_COLOR_G, NUMBER_COLOR_B);
      break;
      
    case SPINNING:
      // Red for spinning
      robot.setColorAll(SPINNING_COLOR_R, SPINNING_COLOR_G, SPINNING_COLOR_B);
      break;
  }
}

void handleLandmarkRecognition() {
  // Check if a landmark is detected
  if (camera.anyLandmarkDetected()) {
    // Check which landmark was detected
    if (camera.landmarkIdDetected(1)) {
      Serial.println("Landmark ID 1 detected. Will spin clockwise.");
      leftMotorSpeed = SPIN_SPEED;
      rightMotorSpeed = -SPIN_SPEED;  // Clockwise spin
      directionDetermined = true;
    } 
    else if (camera.landmarkIdDetected(2)) {
      Serial.println("Landmark ID 2 detected. Will spin counter-clockwise.");
      leftMotorSpeed = -SPIN_SPEED;
      rightMotorSpeed = SPIN_SPEED;  // Counter-clockwise spin
      directionDetermined = true;
    }
    
    // If direction was determined, switch to number recognition
    if (directionDetermined) {
      Serial.println("Direction determined. Switching to number recognition mode.");
      camera.changeFunc(APPLICATION_NUMBER_REC);
      currentState = NUMBER_RECOGNITION;
      updateLEDs();  // Update LEDs to show new state
      delay(500);    // Give time for the camera to switch modes
    }
  }
}

void handleNumberRecognition() {
  // Check if a number is detected with high confidence
  int number = camera.numberWithMaxProb();
  float confidence = camera.numberMaxProb();
  
  if (confidence > 0.7) {  // Only proceed if confidence is high enough
    // Make sure number is between 1 and MAX_NUMBER
    if (number >= 1 && number <= MAX_NUMBER) {
      Serial.print("Number detected: ");
      Serial.print(number);
      Serial.print(" with confidence: ");
      Serial.println(confidence);
      
      // Set spin duration directly to the number of seconds
      spinDuration = number * SECOND_DURATION;
      
      // Start spinning in the direction determined by landmark
      Serial.print("Spinning for ");
      Serial.print(number);
      Serial.println(" seconds");
      
      robot.set_motor_speed(leftMotorSpeed, rightMotorSpeed);  // Start spinning in the determined direction
      spinStartTime = millis();  // Record the start time
      
      // Transition to the spinning state
      currentState = SPINNING;
      updateLEDs();  // Update LEDs to show new state
    } else {
      Serial.print("Detected number ");
      Serial.print(number);
      Serial.println(" is out of range (1-5). Ignoring.");
    }
  }
}

void handleSpinning() {
  // Check if spinning duration has elapsed
  if (millis() - spinStartTime >= spinDuration) {
    // Stop spinning
    robot.set_motor_speed(0, 0);
    Serial.println("Spin complete. Resetting and switching back to landmark recognition.");
    
    // Reset direction determination
    directionDetermined = false;
    
    // Switch back to landmark recognition mode
    camera.changeFunc(APPLICATION_LANDMARK);
    currentState = LANDMARK_RECOGNITION;
    updateLEDs();  // Update LEDs to show new state
    delay(500);    // Give time for the camera to switch modes
  }
}

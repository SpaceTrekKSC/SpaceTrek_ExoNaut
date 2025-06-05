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
 * * Program Flow:
 * Station 1: Landmark recognition (ID 1 = clockwise spin, ID 2 = counter-clockwise spin)
 * Station 2: Number recognition (1-5, determines spin duration in seconds)
 * Station 3: Spin in the direction determined by landmark for duration determined by number
 * * Commands:
 * exonaut robot;                       //Creates the main robot instance
 * //Handles motors and core functions
 *
 * ExoNaut_AICam camera;                //Creates an AI Camera object
 * //Handles vision-based detection and recognition
 *
 * robot.begin();                       //Initializes the robot systems
 *
 * camera.begin();                      //Initializes the camera module
 *
 * robot.set_motor_speed(left, right);  //Sets the motor speeds
 * //Parameters are for left and right motors (-100 to 100)
 *
 * camera.changeFunc(mode);             //Changes the camera's operating mode
 * //Modes: APPLICATION_LANDMARK, APPLICATION_NUMBER_REC
 *
 * camera.updateResult();               //Updates recognition results from the camera
 *
 * camera.landmarkIdWithMaxProb();      //Returns the landmark ID with highest probability
 *
 * camera.landmarkMaxProb();            //Returns the confidence level of the detected landmark
 *
 * camera.numberWithMaxProb();          //Returns the recognized number with highest probability
 *
 * camera.numberMaxProb();              //Returns the confidence level of the recognized number
 *
 * robot.setColorAll(r, g, b);          //Sets the color of all onboard NeoPixel LEDs
 * //Parameters are RGB values (0-255)
 **************************************************/

// These lines add the special instructions (libraries) our robot needs.
#include "ExoNaut.h"        // For basic robot controls like moving and lights.
#include "ExoNaut_AICam.h"    // For the AI Camera's smart vision.

// Let's name our robot and camera so we can command them.
exonaut robot;              // This is our main ExoNaut robot.
ExoNaut_AICam camera;       // This is the robot's AI Camera.

// These variables help the robot remember what it's doing.
int step = 1;               // Keeps track of which step the robot is on (1, 2, or 3).
float spinDirection = 0;    // Remembers if the robot should spin fast (30) or slow (-30). 0 means not set yet.
unsigned long spinEndTime = 0; // Stores the future time when the robot should stop spinning.

// The setup() runs once when the robot turns on.
void setup() {
  Serial.begin(115200);       // Lets the robot send messages to the computer.
  
  // Wake up all the robot parts!
  robot.begin();              // Wakes up the main robot.
  camera.begin();             // Wakes up the AI Camera.
  camera.setLed(true);        // Turn on the camera's LED light.
  
  // Start by looking for landmarks.
  camera.changeFunc(APPLICATION_LANDMARK); 
  robot.setColorAll(0, 0, 255); // Make the robot's lights blue while looking for landmarks.
  robot.show();                 // Update the lights to show the new color.
  
  Serial.println("Looking for landmarks..."); // Send a message to the computer.
}

// The loop() runs over and over, forever!
void loop() {
  camera.updateResult(); // Ask the camera what it currently sees.
  
  if (step == 1) {
    // Step 1: Look for landmarks (like special pictures).
    int landmark = camera.landmarkIdWithMaxProb(); // Ask the camera which landmark it sees best.
    float confidence = camera.landmarkMaxProb();   // Ask how sure the camera is (0.0 to 1.0).
    
    // Only act if the camera is pretty sure (more than 50% confident).
    if (confidence > 0.5) { 
      if (landmark == 1) {
        spinDirection = 30;   // If it sees landmark 1, set spin to clockwise (positive speed).
        Serial.println("Landmark 1: Will spin clockwise");
      } else if (landmark == 2) {
        spinDirection = -30;  // If it sees landmark 2, set spin to counter-clockwise (negative speed).
        Serial.println("Landmark 2: Will spin counter-clockwise");
      }
      
      // If a spin direction was chosen...
      if (spinDirection != 0) {
        // ...move to the next step: looking for numbers.
        camera.changeFunc(APPLICATION_NUMBER_REC);
        robot.setColorAll(0, 255, 0); // Make the lights green while looking for numbers.
        robot.show();
        step = 2; // Go to step 2.
        Serial.println("Now looking for numbers...");
        delay(500); // Wait a little bit for the camera to switch modes.
      }
    }
    
  } else if (step == 2) {
    // Step 2: Look for numbers.
    int number = camera.numberWithMaxProb();      // Ask the camera what number it sees best.
    float confidence = camera.numberMaxProb();    // Ask how sure it is.
    
    // Only act if the camera is very sure (70%) AND the number is between 1 and 5.
    if (confidence > 0.7 && number >= 1 && number <= 5) { 
      // Start spinning!
      robot.set_motor_speed(spinDirection, -spinDirection); // Tell motors to spin (one forward, one backward).
      robot.setColorAll(255, 0, 0);   // Make the lights red while spinning.
      robot.show();
      
      // Calculate when to stop spinning. 'millis()' is the current time in milliseconds.
      spinEndTime = millis() + (number * 1000);   // 'number * 1000' is the spin duration in milliseconds.
      step = 3; // Go to step 3.
      
      Serial.print("Spinning for ");
      Serial.print(number);
      Serial.println(" seconds");
    }
    
  } else if (step == 3) {
    // Step 3: Robot is currently spinning.
    // Check if it's time to stop.
    if (millis() >= spinEndTime) {
      // Stop spinning.
      robot.set_motor_speed(0, 0);    // Stop both motors.
      robot.setColorAll(0, 0, 255);   // Change lights back to blue.
      robot.show();
      
      // Get ready to start over from the beginning.
      camera.changeFunc(APPLICATION_LANDMARK); // Tell camera to look for landmarks again.
      step = 1;                   // Go back to step 1.
      spinDirection = 0;          // Reset the spin direction.
      
      Serial.println("Done spinning. Looking for landmarks again...");
      delay(500); // Wait a bit before starting over.
    }
  }
  
  delay(100);   // A small pause in each loop to let things process.
}

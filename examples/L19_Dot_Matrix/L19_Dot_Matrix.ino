/*
* L19_DotMatrix.ino
*
* 
*
* Author: For ExoNaut Robot
* Date: March 5th, 2025
*/

#include <Arduino.h>
#include "ExoNaut.h"
#include "ExoNaut_DotMatrix.h"

// Create instances
exonaut robot;
ExoNaut_DotMatrix dotMatrix;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ExoNaut Dot Matrix Large Number Display Example");
  
  // Initialize the ExoNaut robot
  robot.begin();
  
  // Initialize the TM1640 dot matrix
  dotMatrix.begin();
  
  // Set robot's onboard LEDs
  robot.setColorAll(0, 255, 0);  // Green
  robot.show();
  
  // Quick test of all LEDs
  dotMatrix.setAllOn();
  delay(500);
  dotMatrix.clear();
  delay(500);
}

void loop() {
  // Example 1: Simple counting with regular display
  Serial.println("Example 1: Counting 0-99");
  for (int i = 0; i <= 99; i++) {
    dotMatrix.displayNumber(i);
    delay(300);  // Slow enough to see each number
  }
  delay(1000);
  
  // Example 2: Countdown from 10 with large numbers
  Serial.println("Example 2: Countdown");
  for (int i = 10; i >= 0; i--) {
    dotMatrix.displayNumber(i);
    delay(1000);  // One second for countdown effect
  }
  
  // Flash 00 a few times
  for (int i = 0; i < 3; i++) {
    dotMatrix.setAllOff();
    delay(250);
    dotMatrix.displayNumber(0);
    delay(250);
  }
  
  delay(2000);  // Pause before repeating
}

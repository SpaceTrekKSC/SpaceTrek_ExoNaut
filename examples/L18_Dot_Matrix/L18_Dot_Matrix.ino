/*
 * ExoNaut_DotMatrix_Demo.ino
 *
 * Author: For ExoNaut Robot
 * Date: March 28th, 2025
 *
 * This sketch demonstrates the features of the ExoNaut_DotMatrix library,
 * including displaying numbers and scrolling text.
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
  Serial.println("ExoNaut Dot Matrix Demo");
  
  // Initialize the ExoNaut robot
  robot.begin();
  
  // Initialize the TM1640 dot matrix
  dotMatrix.begin();
  dotMatrix.setBrightness(TM1640_BRIGHTNESS_MAX);
  
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
  // PART 1: Number Display Demo
  Serial.println("Number Display Demo");
  
  // Count up from 0 to 20
  for (int i = 0; i <= 20; i++) {
    dotMatrix.displayNumber(i);
    delay(300);
  }
  delay(1000);
  
  // Countdown with effect
  Serial.println("Countdown Demo");
  for (int i = 10; i >= 0; i--) {
    dotMatrix.displayNumberWithEffect(i);
    delay(1000);
  }
  
  // Flash 00 a few times
  for (int i = 0; i < 3; i++) {
    dotMatrix.setAllOff();
    delay(250);
    dotMatrix.displayNumber(0);
    delay(250);
  }
  
  // PART 2: Text Scrolling Demo
  Serial.println("Text Scrolling Demo");
  
  // Start scrolling text (3 complete scrolls)
  dotMatrix.scrollText("HELLO EXONAUT WORLD!", 3);
  
  // Keep updating the scroll until it's complete
  while (dotMatrix.isScrolling()) {
    dotMatrix.updateScroll();
    delay(10);  // Small delay to prevent excessive CPU usage
  }
  
  // Pause before next demo
  delay(1000);
  
  // Scroll text with faster speed (1 scroll)
  dotMatrix.scrollText("SPACE TREK", 1, 50);  // 50ms update speed (faster)
  
  // Keep updating the scroll until it's complete
  while (dotMatrix.isScrolling()) {
    dotMatrix.updateScroll();
    delay(10);
  }
  
  // Pause before next demo
  delay(1000);
  
  // Countdown timer (minutes:seconds)
  int seconds = 20;  // 1:30
  
  Serial.println("Countdown Timer Demo");
  while (seconds > 0) {
    int mins = seconds / 60;
    int secs = seconds % 60;
    
    // Display as MM:SS
    String timeStr = "";
    
    // Add leading zero for minutes if needed
    if (mins < 10) {
      timeStr += "0";
    }
    timeStr += String(mins);
    timeStr += ":";
    
    // Add leading zero for seconds if needed
    if (secs < 10) {
      timeStr += "0";
    }
    timeStr += String(secs);
    
    // Scroll once
    dotMatrix.scrollText(timeStr.c_str(), 1, 60);
    
    // Keep updating until complete
    while (dotMatrix.isScrolling()) {
      dotMatrix.updateScroll();
      delay(10);
    }
    
    // Decrement the seconds
    seconds--;
    
    // Flash robot LEDs as timer approaches 0
    if (seconds <= 10) {
      robot.setColorAll(255, 0, 0);  // Red for final countdown
    } else if (seconds <= 30) {
      robot.setColorAll(255, 255, 0);  // Yellow for warning
    }
    robot.show();
  }
  
  // Timer complete
  dotMatrix.scrollText("TIME'S UP!", 2, 70);
  
  // Flash robot LEDs rapidly
  for (int i = 0; i < 10; i++) {
    robot.setColorAll(255, 0, 0);  // Red
    robot.show();
    delay(200);
    robot.setColorAll(0, 0, 0);    // Off
    robot.show();
    delay(200);
  }
  
  // Keep updating the scroll until it's complete
  while (dotMatrix.isScrolling()) {
    dotMatrix.updateScroll();
    delay(10);
  }
  
  // Reset LEDs to green
  robot.setColorAll(0, 255, 0);  // Green
  robot.show();
  
  // Pause before restarting the demo
  delay(2000);
}

/*
 * ExoNaut_DotMatrix.ino
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: March 28th, 2025
 *
 * This sketch demonstrates the features of the ExoNaut_DotMatrix library,
 * including displaying numbers and scrolling text.
 *
 * The previous Dot
 * 
 * The demo now supports the port selection system, allowing the dot matrix
 * to be connected to either port 6 or port 8 of the ExoNaut robot.
 */

#include <Arduino.h>
#include "ExoNaut.h"
#include "ExoNaut_DotMatrix.h"

// Create instances
exonaut robot;

// Create dot matrix on port 6 (can be changed to port 8)
uint8_t dotMatrixPort = 6; // Change to 8 if using port 8
ExoNaut_DotMatrix dotMatrix(dotMatrixPort);

void setup() {
  Serial.begin(115200);                               // Initialize serial communication
  Serial.println("ExoNaut Dot Matrix Demo");
  Serial.print("Using port: ");
  Serial.println(dotMatrixPort);
  
  robot.begin();                                      // Initialize the ExoNaut robot object
  
  
  dotMatrix.begin();                                  // Initialize the TM1640 dot matrix
  dotMatrix.setBrightness(0);                         //use values from 0 to 7, with 0 being the minimum and 7 being the maximum
  
  // Set robot's onboard LEDs
  robot.setColorAll(0, 255, 0);  // Green
  robot.show();
  
  // Quick test of all LEDs
  dotMatrix.setAllOn();
  delay(500);
  dotMatrix.clear();
  delay(500);
  
  // Display welcome message showing which port is being used
  String welcomeMsg = "DOT MATRIX ON PORT " + String(dotMatrixPort);
  dotMatrix.scrollText(welcomeMsg.c_str(), 1, 70);
  
  // Keep updating the scroll until it's complete
  while (dotMatrix.isScrolling()) {
    dotMatrix.updateScroll();
    delay(10);
  }
  
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
  String portMsg = "PORT " + String(dotMatrixPort) + " ACTIVE";
  dotMatrix.scrollText(portMsg.c_str(), 1, 50);  // 50ms update speed (faster)
  
  // Keep updating the scroll until it's complete
  while (dotMatrix.isScrolling()) {
    dotMatrix.updateScroll();
    delay(10);
  }
  
  // Pause before next demo
  delay(1000);
  
  // Countdown timer (minutes:seconds)
  int seconds = 20;  // 20 seconds countdown
  
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
  
  // Display which port is being used
  String portReminder = "USING PORT " + String(dotMatrixPort);
  dotMatrix.scrollText(portReminder.c_str(), 1, 70);
  
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

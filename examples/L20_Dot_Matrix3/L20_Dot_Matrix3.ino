/*
 * L20_DotMatrix.ino
 *
 * This example demonstrates how to use the ExoNaut Dot Matrix
 * to display numbers
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * The demo now supports the port selection system, allowing the dot matrix
 * to be connected to either port 6 or port 8 of the ExoNaut robot.

Commands:
ExoNaut_DotMatrix dotMatrix;               //This command creates a DotMatrix object called 'dotMatrix'
                                           //This is the object that handles all dot matrix display functions

dotMatrix.begin();                          //This command initializes the display hardware and prepares it for use
                                           //It is used once at the beginning of the program

dotMatrix.setBrightness(brightness);        //This command adjusts the brightness level of the display
                                           //Values range from 0 (darkest) to 7 (brightest)

dotMatrix.clear();                          //This command turns off all LEDs and clears the display buffer

dotMatrix.setAllOn();                       //This command turns on all LEDs in the display
dotMatrix.setAllOff();                      //This command turns off all LEDs (same as clear)

dotMatrix.displayNumber(number);            //This command shows a number (0-99) on the display

dotMatrix.scrollText(text, numScrolls, speed); //This command scrolls text across the display
                                              //Parameters: text to display, number of times to scroll, speed
*/

#include <Arduino.h>
#include "ExoNaut.h"
#include "ExoNaut_DotMatrix.h"

// Create instances
exonaut robot;
ExoNaut_DotMatrix dotMatrix(8);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ExoNaut Dot Matrix Large Number Display Example");
  
  // Initialize the ExoNaut robot
  robot.begin();
  
  // Initialize the TM1640 dot matrix
  dotMatrix.begin();
  dotMatrix.setBrightness(7);                         //use values from 0 to 7, with 0 being the minimum and 7 being the maximum
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

/***************************************************
 * L16_7SegmentDisplay.ino
 * A fun example to show how to use the ExoNaut 7-segment display!
 *
 * Author:    Ryan Bori
 * Email:     ryan.bori@spacetrek.com
 * Date:      May 22nd, 2025
 * Updated:   Self-contained library version (no external dependencies!)
 *
 * Plug the display into Port 6 or Port 8 on your robot.
 * Use 6 if you're not sure. Just change the number in the code if needed!
 *
 *
 * Commands List:
 * ExoNaut_7Segment display(port);                    // This sets up the display. Use port 6 or 8.
 *
 * display.begin();                                   // Start the display so it works!
 *
 * display.showText("TEXT");                          // Show 4 letters or numbers on the screen
 *
 * display.clear();                                   // Clear the whole screen
 *
 * display.showNumber(number);                        // Show any number up to 9999
 *
 * display.showDigit(number, position);               // Show 1 number at a spot (0 = left, 3 = right)
 *
 * display.countUp(start, end, delay);                // Count up from one number to another
 *
 * display.countDown(start, end, delay);              // Count down from one number to another
 *
 * display.blink(number, times, delay);               // Blink a number on and off
 *
 * display.scroll("HELLO WORLD", speed);              // Scroll a long message across the screen
 *
 * display.showDecimal(number, place);                // Show a number with a decimal point (like 31.42)
 *
 ***************************************************/

#include <ExoNaut.h>              // Include the main ExoNaut library
#include "ExoNaut_7Segment.h"     // Include our simple 7-segment library

// Create the robot and display objects
exonaut robot;                    // Main robot object (needed for I2C initialization)
ExoNaut_7Segment display(6);      // Create a 7-segment display object on port 6
                                  // Change the number to 8 if you plugged it into port 8

void setup() {
  // Initialize the robot first (this sets up I2C communication)
  robot.begin();
  delay(500);                     // Give the robot time to start up
  
  // Start the display
  display.begin();
  
  // Show a welcome message
  display.showText("HELO");       // "HELLO" but only 4 characters fit
  delay(2000);                    // Wait 2 seconds
  
  // Clear the display
  display.clear();
  delay(500);
  
  // Show some numbers
  display.showNumber(1234);       // Show the number 1234
  delay(2000);
  
  display.showNumber(42);         // Show the number 42
  delay(2000);
  
  // Count up from 0 to 10 (fun animation!)
  display.countUp(0, 10, 300);    // Count from 0 to 10, wait 300ms between numbers
  delay(1000);
  
  // Count down from 10 to 0
  display.countDown(10, 0, 200);  // Count from 10 to 0, wait 200ms between numbers
  delay(1000);
  
  // Blink the number 88
  display.blink(88, 5, 400);      // Blink 88 five times, wait 400ms between blinks
  delay(1000);
  
  // Show text with scrolling
  display.scroll("HELLO WORLD", 250);  // Scroll "HELLO WORLD" across display
  delay(2000);
  
  // Show a number with a decimal point
  display.showDecimal(3142, 2);   // Show 31.42 (decimal at position 2)
  delay(3000);
  
  // One more fun thing - show individual digits
  display.clear();
  display.showDigit(1, 0);        // Show "1" at position 0 (leftmost)
  delay(500);
  display.showDigit(2, 1);        // Show "2" at position 1
  delay(500);
  display.showDigit(3, 2);        // Show "3" at position 2
  delay(500);
  display.showDigit(4, 3);        // Show "4" at position 3 (rightmost)
  delay(2000);
  
  // Show "COOL" to finish
  display.showText("COOL");
}

void loop() {
  // Let's make a simple clock-like counter
  static int counter = 0;
  
  display.showNumber(counter);    // Show the current counter value
  counter++;                      // Add 1 to the counter
  
  if (counter > 9999) {           // If we go past 9999, start over at 0
    counter = 0;
  }
  
  delay(1000);                    // Wait 1 second before counting again
}
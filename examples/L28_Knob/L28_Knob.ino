/*
 * L10_Knob_Control.ino
 *
 * Example sketch for the ExoNaut knob module.
 * This sketch demonstrates reading a potentiometer
 * and printing its position as a percentage.
 * 
 * The knob is connected to a specified port, and the
 * output is scaled from 0 to 100% based on calibrated ADC range.
 * 
 * The knob must be connected to either port 1, 2, 6, or 8.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 19, 2025
 *
 * Commands:
 * ExoNaut_Knob knob;              //Creates a knob object for reading input
 * knob.begin(PORT);               //Initializes the knob on the specified port (1, 2, 6, 8)
 * knob.getPercent();              //Returns the current knob position as a percentage (0–100)
 */

#include <ExoNaut_Knob.h> // Include the knob library

ExoNaut_Knob knob; // Create a knob object to read the knob value

void setup() {
  Serial.begin(115200); // Start the Serial Monitor
  knob.begin(8);        // Start the knob on port 8
}

void loop() {
  int percent = knob.getPercent(); // Read knob position as a percentage
  Serial.print("Knob: ");          // Print label
  Serial.print(percent);           // Print the percent value
  Serial.println("%");             // Print the percent sign and move to next line
  delay(20);                       // Wait a short time before reading again
}

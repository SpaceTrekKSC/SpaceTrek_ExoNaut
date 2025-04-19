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


 #include <ExoNaut_Knob.h>

 ExoNaut_Knob knob;
 
 void setup() {
   Serial.begin(115200);
   knob.begin(8); // Port 1 = GPIO 36
 }
 
 void loop() {
   int percent = knob.getPercent();
   Serial.print("Knob: ");
   Serial.print(percent);
   Serial.println("%");
   delay(20);
 }
 
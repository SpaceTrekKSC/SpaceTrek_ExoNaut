/**************************************************
 * L14_RGB_LED_Module.ino
 * An example program to demonstrate how to use the RGB LED module with the ExoNaut.
 * 
 * The RGB LEDs are Neo Pixels. The module has two LEDs and the port to pin mapping is:
 *	___________________________
 *	 PORT Number |	Pin Number
 *        2      |      32
 *        6      |      33
 *        8      |      26
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 *
 * Commands:
 * RGB rgb;                                         // Create the RGB object
 * rgb.begin(port);                                 // Initialize the RGB module on specified port (2, 6, or 8)
 * rgb.setColor(pixel-num, red, green, blue);       // Set the color of one LED (call rgb.show() to display)
 * rgb.setColorAll(red, green, blue);               // Set all LEDs to same color and display immediately
 * rgb.show();                                      // Update LEDs with colors in memory
 * rgb.clear();                                     // Turn off all LEDs
 *
**************************************************/

#include <ExoNaut.h>
#include <ExoNaut_RGB_LED.h>

exonaut robot;             // Create the main robot object
RGB rgb;                   // Create the RGB LED object

void setup() {
  Serial.begin(115200);    // Initialize serial communication
  Serial.println("RGB LED Test Starting");
  
  robot.begin();           // Initialize the robot
  Serial.println("Robot initialized");
  
  rgb.begin(8);            // Initialize the RGB module on port 8 (pin 26)
  Serial.println("RGB LED initialized on port 8 (pin 26)");
  
  delay(1500);             // Wait 1.5 seconds
}

void loop() {
  // Example 1: Set all LEDs to different colors
  rgb.setColorAll(150, 0, 0);    // Red
  delay(1000);
  rgb.setColorAll(0, 150, 0);    // Green
  delay(1000);
  rgb.setColorAll(0, 0, 150);    // Blue
  delay(1000);
  rgb.clear();                   // Turn off
  delay(1000);

  // Example 2: Set each LED individually, one at a time
  for(int i = 0; i < 2; i++) {
    rgb.setColor(i, 0, 0, 200);  // Blue
    rgb.show();                  // Update display
    delay(1000);
  }
  delay(1000);
  rgb.clear();
  delay(1000);

  // Example 3: Set different colors for each LED
  rgb.setColor(0, 200, 0, 0);    // Set first LED to red
  rgb.setColor(1, 0, 200, 0);    // Set second LED to green
  rgb.show();                    // Update both at once
  delay(2000);
  rgb.clear();
  delay(1000);

  // Example 4: Set all LEDs to white
  for(int i = 0; i < 2; i++) {
    rgb.setColor(i, 200, 200, 200);  // White
  }
  rgb.show();
  delay(2000);

  rgb.clear();
  delay(1000);
}

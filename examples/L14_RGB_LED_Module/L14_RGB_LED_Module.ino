/***************************************************
 * L14_RGB_LED_Module.ino
 * A simple example to test the RGB LED module using the
 * ExoNaut RGB class. This program turns on individual LEDs,
 * changes brightness, and shows basic animations.
 *
 * === Common Commands ===
 * RGB pixels(port);   // Give Port being used
 * pixels.begin();                   // Start the LED strip
 * pixels.setBrightness(0–255);      // Set brightness level
 * pixels.setColor(index, r, g, b);  // Set a specific LED to a color
 * pixels.clear();                   // Turn off all LEDs
 * pixels.colorWipe(r, g, b, delay); // Light up one by one
 *
 * Author:   Ryan Bori
 * Email:    ryan.bori@spacetrek.com
 * Date:     July 1st, 2025
 ***************************************************/

#include <ExoNaut_RGB_LED.h>  // Include the ExoNaut RGB LED library

// Create an instance of the RGB class for Port 8 (pin 26), defaulting to 8 LEDs
RGB pixels(8); 

void setup() {
  Serial.begin(115200);  // Start Serial Monitor for debugging
  Serial.println("Simplified External RGB Test");

  pixels.begin();          // Start the RGB LED module
  pixels.setBrightness(50); // Set brightness level to 50 (out of 255)

  pixels.clear();           // Turn off all LEDs at the start
  
  Serial.println("RGB Module Initialized! Starting test...");
}

void loop() {
  // Set pixel 0 to RED
  Serial.println("Setting pixel 0 to RED");
  pixels.setColor(0, 255, 0, 0);  // (index, red, green, blue)
  delay(1000); // Wait 1 second

  pixels.setBrightness(10); // Dim the LEDs
  delay(1000); // Wait 1 second

  // Set pixel 0 to RED again (to show dimmer brightness)
  Serial.println("Setting pixel 0 to RED");
  pixels.setColor(0, 255, 0, 0);
  delay(1000);

  pixels.setBrightness(50); // Restore brightness
  delay(1000);

  // Set pixel 0 to RED again
  Serial.println("Setting pixel 0 to RED");
  pixels.setColor(0, 255, 0, 0);
  delay(1000);

  // Set pixel 1 to BLUE
  Serial.println("Setting pixel 1 to BLUE");
  pixels.setColor(1, 0, 0, 255);
  delay(1000);

  // Set pixel 0 to GREEN
  Serial.println("Setting pixel 0 to GREEN");
  pixels.setColor(0, 0, 255, 0);
  delay(1000);

  // Turn off all LEDs
  Serial.println("Clearing all pixels");
  pixels.clear();
  delay(1000);

  pixels.setBrightness(10); // Lower brightness for animation

  // Wipe GREEN across all pixels, 1 second per LED
  pixels.colorWipe(0, 255, 0, 1000);  
  pixels.clear(); // Turn off all LEDs again
  delay(1000);
}

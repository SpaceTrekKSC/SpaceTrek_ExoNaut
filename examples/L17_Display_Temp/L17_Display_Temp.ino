/**********************************************************
 * L17_Display_Temp_Recreated.ino
 * Displays temperature on a 4-digit 7-segment display.
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 *
 * Plug Instructions:
 * - Temp & Humidity Sensor: Port 3, 4, 5, or 9
 * - 7-Segment Display: Port 6 or 8
 *
 * Commands:
 * exonaut robot;                      // Main robot system
 * TempHumid th;                      // Temp & Humidity sensor
 * ExoNaut_7Segment display(port);    // 7-segment display
 *
 * robot.begin();                     // Start robot system
 * th.begin();                        // Start sensor
 * display.begin();                   // Start display
 * th.readSensor();                   // Get new sensor reading
 * display.showDecimal(num, pos);     // Show number with decimal
 **********************************************************/

#include <ExoNaut.h>
#include <ExoNaut_TempHumid.h>
#include <ExoNaut_7Segment.h>

exonaut robot;
TempHumid th;
ExoNaut_7Segment display(6);

int displayTemp = 0;

void setup() {
  robot.begin();
  th.begin();
  display.begin();

  // display.setBrightness(7); // Optional: max brightness
}

void loop() {
  th.readSensor();

  displayTemp = th.temperature * 10;

  display.clear();
  display.showDecimal(displayTemp, 2);

  delay(1000);
}

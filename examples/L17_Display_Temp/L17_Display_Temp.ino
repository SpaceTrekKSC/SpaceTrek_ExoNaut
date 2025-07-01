/**********************************************************
 * L17_Display_Temp_Recreated.ino
 * Displays temperature on a 4-digit 7-segment display
 * and prints temperature in F, C, and K to the Serial Monitor.
 *
 * Author: Andrew Gafford
 * Email:  agafford@spacetrek.com
 * Date:   July 1, 2025
 *
 * Plug Instructions:
 * - Temp & Humidity Sensor: Port 3, 4, 5, or 9
 * - 7-Segment Display: Port 6 or 8
 *
 * Commands:
 * exonaut robot;                  // Main robot system
 * TempHumid th;                   // Temp & Humidity sensor
 * ExoNaut_7Segment display(port);   // 7-segment display
 *
 * robot.begin();                  // Start robot system
 * th.begin();                     // Start sensor
 * display.begin();                // Start display
 * th.readSensor();                // Get new sensor reading
 * th.setUnit('F');                // Set temperature unit ('F', 'C', or 'K')
 * th.getTemperature();            // Get temp in the currently set unit
 * display.showNumber(num);        // Show a number on the display
 **********************************************************/

#include <ExoNaut.h>
#include <ExoNaut_TempHumid.h>   // Library for Temp & Humid sensor
#include <ExoNaut_7Segment.h>    // Library for 7-segment display

exonaut robot;                  // Initialize robot controller
TempHumid th;                   // Create temperature/humidity sensor object
ExoNaut_7Segment display(8);    // 7-segment display on Port 8

void setup() {
  robot.begin();                // Initialize robot hardware
  Serial.begin(115200);         // Open Serial Monitor at 115200 baud
  th.begin();                   // Start Temp/Humid sensor
  display.begin();              // Start 7-segment display
}

void loop() {
  // === Read the sensor ===
  th.readSensor();              // Update temp and humidity values

  // === Temperature in Fahrenheit ===
  th.setUnit('F');              // Set unit to Fahrenheit
  int tempF = th.getTemperature();  // Get temperature value
  Serial.print("Temperature (Fahrenheit): ");
  Serial.println(tempF);

  // === Temperature in Celsius ===
  th.setUnit('C');              // Set unit to Celsius
  int tempC = th.getTemperature();
  Serial.print("Temperature (Celsius):    ");
  Serial.println(tempC);

  // === Temperature in Kelvin ===
  th.setUnit('K');              // Set unit to Kelvin
  int tempK = th.getTemperature();
  Serial.print("Temperature (Kelvin):     ");
  Serial.println(tempK);

  // === Humidity ===
  int hum = th.humidity;        // Read humidity
  Serial.print("Humidity:                 ");
  Serial.println(hum);
  Serial.println("---------------------------------");

  // === Show Fahrenheit temp on display ===
  display.clear();              // Clear any previous number
  display.showNumber(tempF);    // Show Fahrenheit temp on 7-segment

  delay(2000);                  // Wait 2 seconds before next reading
}
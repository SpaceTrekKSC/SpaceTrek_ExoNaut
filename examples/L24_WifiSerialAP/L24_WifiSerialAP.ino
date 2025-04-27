/*
 * L24_WebSerial_AP.ino
 *
 * Example sketch for the WebSerialAccessPoint library.
 * This sketch demonstrates creating a WiFi Access Point
 * and sending serial messages to a web page hosted by the ESP32.
 * 
 * The ESP32 creates its own wireless network that devices can join.
 * Serial output is mirrored both to the traditional Serial Monitor
 * and to a browser page via WebSerial.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 27, 2025
 *
 * Commands:
 * WebSerial.begin("SSID", "PASSWORD");    // Starts the Access Point and WebSerial server
 * WebSerial.println(message);             // Sends text to the WebSerial browser page
 */

#include "WebSerialAccessPoint.h"

void setup() {
  Serial.begin(115200);
  WebSerial.begin("Robot_AP", "12345678");  // Create WiFi Access Point

  Serial.println("Access Point Started!");
  WebSerial.println("Hello from ESP32!");
}

void loop() {
  static int counter = 0;
  String message = "Loop count: " + String(counter++);
  
  Serial.println(message);                 // Still prints to Serial Monitor
  WebSerial.println(message);   // Also sends to Web Page

  delay(1000);
}

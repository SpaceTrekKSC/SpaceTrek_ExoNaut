/*
 * L35_MP3_Playback.ino
 *
 * Demonstrates basic MP3 playback functionality using the ExoNaut_MP3 library.
 * Intended for use with the Space Trek ExoNaut robot and MP3 module via I2C.
 *
 * Controls:
 * - Serial commands to control playback:
 *     p = play
 *     a = pause
 *     n = next track
 *     b = previous track
 *     u = volume up
 *     d = volume down
 *     x = max volume
 *     m = mute
 *     0–9 = set volume percentage (0% to 90%)
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 3, 2025
 *
 * Notes:
 * - MP3 module must be plugged into ports 9, 5, 4, or 3.
 * - Requires ExoNaut_MP3 library.
 */

#include <Wire.h>         // Include I2C communication library
#include "ExoNaut_MP3.h"  // Include MP3 control library

ExoNaut_MP3 mp3; // Create an MP3 player object

void setup() {
  Serial.begin(115200); // Start the Serial Monitor
  mp3.begin();          // Initialize the MP3 module
  Serial.println("MP3 Ready! Commands:"); // Show that we're ready
  Serial.println("p = play, a = pause, n = next, b = previous"); // List commands
  Serial.println("u = volume up, d = volume down");
  Serial.println("x = max volume, m = mute");
  Serial.println("0–9 = set volume percent (0% to 90%)");
}

void loop() {
  if (Serial.available()) {        // Check if a key was pressed
    char c = Serial.read();        // Read the pressed key
    switch (c) {
      case 'p': mp3.play(); break;                   // p = play
      case 'a': mp3.pause(); break;                  // a = pause
      case 'n': mp3.next(); break;                   // n = next track
      case 'b': mp3.previous(); break;               // b = previous track
      case 'u': mp3.volumeUp(); break;               // u = volume up
      case 'd': mp3.volumeDown(); break;             // d = volume down
      case 'x': mp3.maxVolume(); break;              // x = set max volume
      case 'm': mp3.mute(); break;                   // m = mute audio
      case '0'...'9': mp3.setVolumePercent((c - '0') * 10); break; // 0-9 = set volume %
    }
  }
}

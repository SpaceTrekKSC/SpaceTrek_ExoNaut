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

 #include <Wire.h>
 #include "ExoNaut_MP3.h"
 
 ExoNaut_MP3 mp3;
 
 void setup() {
     Serial.begin(115200);
     mp3.begin();
     Serial.println("MP3 Ready! Commands:");
     Serial.println("p = play, a = pause, n = next, b = previous");
     Serial.println("u = volume up, d = volume down");
     Serial.println("x = max volume, m = mute");
     Serial.println("0–9 = set volume percent (0% to 90%)");
 }
 
 void loop() {
     if (Serial.available()) {
         char c = Serial.read();
         switch (c) {
             case 'p': mp3.play(); break;
             case 'a': mp3.pause(); break;
             case 'n': mp3.next(); break;
             case 'b': mp3.previous(); break;
             case 'u': mp3.volumeUp(); break;
             case 'd': mp3.volumeDown(); break;
             case 'x': mp3.maxVolume(); break;
             case 'm': mp3.mute(); break;
             case '0'...'9': mp3.setVolumePercent((c - '0') * 10); break;
         }
     }
 }
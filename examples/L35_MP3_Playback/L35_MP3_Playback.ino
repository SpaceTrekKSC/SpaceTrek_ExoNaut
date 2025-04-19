/*
 * L35_MP3_Playback.ino
 *
 * WARNING - Work in progress, not fully functional yet
 *
 * Example sketch for demonstrating the ExoNaut_MP3 library
 * with the Space Trek ExoNaut robot.
 * 
 * This sketch provides basic MP3 playback functionality using:
 * - Button A (PIN 0): Next Track
 * - Button B (PIN 2): Play/Pause
 * - LED indicators for playback status
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 3, 2025
 *
 * For MP3 Module (I2C communication)
 * Must be plugged into port 9, 5, 4, 3
 *
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * ExoNaut_MP3 mp3;                      //This command creates an MP3 player object called 'mp3'
 *                                       //This is the object that handles audio playback control
 *
 * mp3.begin();                          //This command initializes the MP3 module and sets up I2C communication
 *                                       //Also detects the total number of tracks available
 *
 * mp3.setVolume(volume);                //This command sets the playback volume (0-30)
 *                                       //0 = mute, 30 = maximum volume
 *
 * mp3.getTotalTracks();                 //This command returns the total number of tracks detected on the module
 *
 * mp3.playTrack(trackNumber);           //This command plays a specific track by number
 *                                       //Tracks are numbered starting from 1
 *
 * mp3.play();                           //This command starts or resumes playback of the current track
 *
 * mp3.pause();                          //This command pauses the current playback
 *
 * mp3.next();                           //This command skips to the next track
 *
 * mp3.previous();                       //This command returns to the previous track
 *
 * mp3.getVolume();                      //This command returns the current volume setting
 *
 * mp3.volumeUp();                       //This command increases the volume by one step
 *
 * mp3.volumeDown();                     //This command decreases the volume by one step
 *
 * mp3.getCurrentTrack();                //This command returns the current track number
 *
 * robot.setColor(index, r, g, b);       //This command sets the color of an individual LED
 *                                       //Parameters: LED index, red, green, blue values (0-255)
 *
 * robot.setColorAll(r, g, b);           //This command sets all LEDs to the same color
 *                                       //Parameters: red, green, blue values (0-255)
 *
 * robot.clear();                        //This command turns off all LEDs
 *
 * robot.show();                         //This command updates the physical LED display
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
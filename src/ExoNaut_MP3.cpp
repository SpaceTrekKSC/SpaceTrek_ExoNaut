/*
 *ExoNaut_MP3.cpp
 *
 *Author:     Ryan Bori
 *Email:		ryan.bori@spacetrek.com
 *Date:		March 3rd, 2025
 *
 *This library is for controlling the MP3 module for the Space Trek ExoNaut Robot.
 *It provides functions for playing, pausing, and controlling MP3 files via I2C.
 *
 */

 #include "ExoNaut_MP3.h"

 void ExoNaut_MP3::begin()
 {
     Wire.begin();
     delay(200);           // Wait for MP3 module to boot
     setVolumePercent(66); // Set to ~20 out of 30
     currentTrack = 1;     // Assume we start at track 1
 }
 
 bool ExoNaut_MP3::sendCommand(uint8_t cmd)
 {
     Wire.beginTransmission(MP3_I2C_ADDR);
     Wire.write(cmd);
     return Wire.endTransmission() == 0;
 }
 
 bool ExoNaut_MP3::sendCommandWithParam(uint8_t cmd, uint8_t param)
 {
     Wire.beginTransmission(MP3_I2C_ADDR);
     Wire.write(cmd);
     Wire.write(param);
     return Wire.endTransmission() == 0;
 }
 
 void ExoNaut_MP3::play()
 {
     sendCommand(MP3_PLAY_ADDR);
 }
 
 void ExoNaut_MP3::pause()
 {
     sendCommand(MP3_PAUSE_ADDR);
 }
 
 void ExoNaut_MP3::next()
 {
     sendCommand(MP3_NEXT_ADDR);
     currentTrack++; // Update our internal tracker
 }
 
 void ExoNaut_MP3::previous()
 {
     sendCommand(MP3_PREV_ADDR);
     if (currentTrack > 1) currentTrack--; // Update our internal tracker
 }

 void ExoNaut_MP3::goToTrack(uint8_t targetTrack)
 {
    if (targetTrack == 0) return; // Ignore invalid track numbers

    int steps = targetTrack - currentTrack;

    if (steps > 0) {
        // If the target is ahead, press 'next'
        for (int i = 0; i < steps; i++) {
            next();
            delay(50); // Small delay between commands helps the module keep up
        }
    } else if (steps < 0) {
        // If the target is behind, press 'previous'
        for (int i = 0; i < -steps; i++) {
            previous();
            delay(50);
        }
    }
    // If steps is 0, we're already on the right track, so we do nothing.
 }
 
 void ExoNaut_MP3::volumeUp()
 {
     if (volume < 30)
         setVolumePercent((++volume) * 100 / 30);
 }
 
 void ExoNaut_MP3::volumeDown()
 {
     if (volume > 0)
         setVolumePercent((--volume) * 100 / 30);
 }
 
 void ExoNaut_MP3::maxVolume()
 {
     setVolumePercent(100);
 }
 
 void ExoNaut_MP3::mute()
 {
     setVolumePercent(0);
 }
 
 void ExoNaut_MP3::setVolumePercent(uint8_t percent)
 {
     if (percent > 100)
         percent = 100;
     volume = map(percent, 0, 100, 0, 30);
     sendCommandWithParam(MP3_VOL_VALUE_ADDR, volume);
 }

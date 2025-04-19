/*
 *ExoNaut_MP3.h
 *
 *Author:	Ryan Bori
 *Email:		ryan.bori@spacetrek.com
 *Date:		March 3rd, 2025
 *
 *This library is for controlling the Hiwonder MP3 module for the Space Trek ExoNaut Robot.
 *It provides functions for playing, pausing, and controlling MP3 files via I2C.
 *
 */

 #ifndef __EXONAUT_MP3_H
 #define __EXONAUT_MP3_H
 
 #include <Arduino.h>
 #include <Wire.h>
 
 #define MP3_I2C_ADDR 0x7B
 
 // Command addresses
 #define MP3_PLAY_NUM_ADDR 1
 #define MP3_PLAY_ADDR 5
 #define MP3_PAUSE_ADDR 6
 #define MP3_PREV_ADDR 8
 #define MP3_NEXT_ADDR 9
 #define MP3_VOL_VALUE_ADDR 12
 
 class ExoNaut_MP3
 {
 public:
     void begin(void);
     void play(void);
     void pause(void);
     void next(void);
     void previous(void);
     void volumeUp(void);
     void volumeDown(void);
     void maxVolume(void);
     void mute(void);
     void setVolumePercent(uint8_t percent); // 0 to 100
 
 private:
     bool sendCommand(uint8_t cmd);
     bool sendCommandWithParam(uint8_t cmd, uint8_t param);
     uint8_t volume = 20; // Start with medium volume
 };
 
 #endif // __EXONAUT_MP3_H
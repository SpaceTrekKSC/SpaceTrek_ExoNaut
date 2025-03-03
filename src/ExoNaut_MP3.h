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
#include "ExoNaut.h"

// I2C address for the Hiwonder MP3 module
#define MP3_I2C_ADDR 0x7B

// MP3 module command addresses
#define MP3_PLAY_NUM_ADDR 1          // Play specified track, 0~3000, low byte first, high byte second
#define MP3_PLAY_ADDR 5              // Play
#define MP3_PAUSE_ADDR 6             // Pause
#define MP3_PREV_ADDR 8              // Previous track
#define MP3_NEXT_ADDR 9              // Next track
#define MP3_VOL_VALUE_ADDR 12        // Set volume (0-30)
#define MP3_SINGLE_LOOP_ON_ADDR 13   // Turn on single loop
#define MP3_SINGLE_LOOP_OFF_ADDR 14  // Turn off single loop
#define MP3_GET_STATUS_ADDR 15       // Get status information
#define MP3_GET_TOTAL_TRACKS_ADDR 16 // Get total number of tracks

// Structure to track MP3 module state
typedef struct __mp3_module_state
{
    bool isPlaying;
    uint8_t currentVolume;
    uint16_t currentTrack;
    uint16_t totalTracks;
    bool loopMode;
} mp3_module_state_t;

class ExoNaut_MP3
{
public:
    // Initialization
    void begin(void);

    // Basic playback control
    void play(void);
    void pause(void);
    void next(void);
    void previous(void);

    // Volume control
    void setVolume(uint8_t volume); // 0-30 scale
    uint8_t getVolume(void);
    void volumeUp(void);
    void volumeDown(void);

    // Track selection
    void playTrack(uint16_t trackNumber);
    uint16_t getCurrentTrack(void);

    // Playback modes
    void enableLoop(bool enable);
    bool isLoopEnabled(void);

    // Status checks
    bool isPlaying(void);

    // Track count
    uint16_t getTotalTracks(void);
    void detectTotalTracks(void);

private:
    mp3_module_state_t state;
    bool sendCommand(uint8_t cmd);
    bool sendCommandWithParam(uint8_t cmd, uint8_t param);
    bool sendCommandWithParam16(uint8_t cmd, uint16_t param);
    int readResponseData(uint8_t cmd, uint8_t *buffer, uint8_t length);
};

extern mp3_module_state_t mp3_state;

#endif // __EXONAUT_MP3_H
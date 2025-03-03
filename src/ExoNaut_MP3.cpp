/*
 *ExoNaut_MP3.cpp
 *
 *Author:     Ryan Bori
 *Email:		ryan.bori@spacetrek.com
 *Date:		March 3rd, 2025
 *
 *This library is for controlling the Hiwonder MP3 module for the Space Trek ExoNaut Robot.
 *It provides functions for playing, pausing, and controlling MP3 files via I2C.
 *
 */

#include <Arduino.h>
#include "ExoNaut_MP3.h"

// Create global instance of MP3 module state
mp3_module_state_t mp3_state = {
    .isPlaying = false,
    .currentVolume = 20, // Default volume
    .currentTrack = 1,   // Default track
    .totalTracks = 0,    // Will be detected during initialization
    .loopMode = false    // Loop mode off by default
};

void ExoNaut_MP3::begin(void)
{
    // Wire.begin() should be called in the main sketch, as it's shared with other devices

    // Give module time to boot
    delay(200);

    // Set initial volume
    setVolume(mp3_state.currentVolume);

    // Detect total number of tracks
    detectTotalTracks();

    // Set initial state
    mp3_state.isPlaying = false;

    Serial.print("MP3 Module initialized. Found ");
    Serial.print(mp3_state.totalTracks);
    Serial.println(" tracks.");
}

// Helper function to send a command with no parameters
bool ExoNaut_MP3::sendCommand(uint8_t cmd)
{
    Wire.beginTransmission(MP3_I2C_ADDR);
    Wire.write(cmd);
    if (Wire.endTransmission() != 0)
    {
        Serial.print("Failed to send command: 0x");
        Serial.println(cmd, HEX);
        return false;
    }
    return true;
}

// Helper function to read response data
int ExoNaut_MP3::readResponseData(uint8_t cmd, uint8_t *buffer, uint8_t length)
{
    // First send the command we want to read from
    Wire.beginTransmission(MP3_I2C_ADDR);
    Wire.write(cmd);
    if (Wire.endTransmission() != 0)
    {
        Serial.print("Failed to send read command: 0x");
        Serial.println(cmd, HEX);
        return -1;
    }

    // Now request the data
    uint8_t bytesRead = 0;
    Wire.requestFrom(MP3_I2C_ADDR, length);

    while (Wire.available() && bytesRead < length)
    {
        buffer[bytesRead] = Wire.read();
        bytesRead++;
    }

    return bytesRead;
}

// Helper function to send a command with one byte parameter
bool ExoNaut_MP3::sendCommandWithParam(uint8_t cmd, uint8_t param)
{
    Wire.beginTransmission(MP3_I2C_ADDR);
    Wire.write(cmd);
    Wire.write(param);
    if (Wire.endTransmission() != 0)
    {
        return false;
    }
    return true;
}

// Helper function to send a command with a 16-bit parameter (low byte first)
bool ExoNaut_MP3::sendCommandWithParam16(uint8_t cmd, uint16_t param)
{
    Wire.beginTransmission(MP3_I2C_ADDR);
    Wire.write(cmd);
    Wire.write(param & 0xFF);        // Low byte
    Wire.write((param >> 8) & 0xFF); // High byte
    if (Wire.endTransmission() != 0)
    {
        return false;
    }
    return true;
}

// Basic playback control
void ExoNaut_MP3::play(void)
{
    if (sendCommand(MP3_PLAY_ADDR))
    {
        mp3_state.isPlaying = true;
    }
    delay(15); // Small delay after command
}

void ExoNaut_MP3::pause(void)
{
    if (sendCommand(MP3_PAUSE_ADDR))
    {
        mp3_state.isPlaying = false;
    }
    delay(15); // Small delay after command
}

void ExoNaut_MP3::next(void)
{
    if (sendCommand(MP3_NEXT_ADDR))
    {
        mp3_state.currentTrack++;
        mp3_state.isPlaying = true;
    }
    delay(15); // Small delay after command
}

void ExoNaut_MP3::previous(void)
{
    if (sendCommand(MP3_PREV_ADDR))
    {
        if (mp3_state.currentTrack > 1)
        {
            mp3_state.currentTrack--;
        }
        mp3_state.isPlaying = true;
    }
    delay(15); // Small delay after command
}

// Volume control
void ExoNaut_MP3::setVolume(uint8_t volume)
{
    // Ensure volume is within valid range (0-30)
    volume = volume > 30 ? 30 : volume;

    if (sendCommandWithParam(MP3_VOL_VALUE_ADDR, volume))
    {
        mp3_state.currentVolume = volume;
    }
    delay(15); // Small delay after command
}

uint8_t ExoNaut_MP3::getVolume(void)
{
    return mp3_state.currentVolume;
}

void ExoNaut_MP3::volumeUp(void)
{
    if (mp3_state.currentVolume < 30)
    {
        uint8_t newVolume = mp3_state.currentVolume + 1;
        Serial.print("Setting volume to: ");
        Serial.println(newVolume);
        setVolume(newVolume);
    }
    else
    {
        Serial.println("Volume already at maximum (30)");
    }
}

void ExoNaut_MP3::volumeDown(void)
{
    if (mp3_state.currentVolume > 0)
    {
        uint8_t newVolume = mp3_state.currentVolume - 1;
        Serial.print("Setting volume to: ");
        Serial.println(newVolume);
        setVolume(newVolume);
    }
    else
    {
        Serial.println("Volume already at minimum (0)");
    }
}

// Track selection
void ExoNaut_MP3::playTrack(uint16_t trackNumber)
{
    if (sendCommandWithParam16(MP3_PLAY_NUM_ADDR, trackNumber))
    {
        mp3_state.currentTrack = trackNumber;
        mp3_state.isPlaying = true;
    }
    delay(15); // Small delay after command
}

uint16_t ExoNaut_MP3::getCurrentTrack(void)
{
    return mp3_state.currentTrack;
}

// Playback modes
void ExoNaut_MP3::enableLoop(bool enable)
{
    if (enable)
    {
        if (sendCommand(MP3_SINGLE_LOOP_ON_ADDR))
        {
            mp3_state.loopMode = true;
        }
    }
    else
    {
        if (sendCommand(MP3_SINGLE_LOOP_OFF_ADDR))
        {
            mp3_state.loopMode = false;
        }
    }
    delay(15); // Small delay after command
}

bool ExoNaut_MP3::isLoopEnabled(void)
{
    return mp3_state.loopMode;
}

// Status checks
bool ExoNaut_MP3::isPlaying(void)
{
    return mp3_state.isPlaying;
}

// Track count functions
uint16_t ExoNaut_MP3::getTotalTracks(void)
{
    return mp3_state.totalTracks;
}

void ExoNaut_MP3::detectTotalTracks(void)
{
    // Try to get total track count from the module
    uint8_t responseBuffer[2];

    // We'll make multiple attempts because sometimes I2C can be unreliable
    for (int attempt = 0; attempt < 3; attempt++)
    {
        int bytesRead = readResponseData(MP3_GET_TOTAL_TRACKS_ADDR, responseBuffer, 2);

        if (bytesRead == 2)
        {
            // Construct the 16-bit value (low byte first)
            uint16_t trackCount = responseBuffer[0] | (responseBuffer[1] << 8);

            // Validate the track count (sanity check)
            if (trackCount > 0 && trackCount < 3000)
            {
                mp3_state.totalTracks = trackCount;
                return;
            }
        }

        delay(50); // Wait a bit before retrying
    }

    // If we couldn't read the track count or got invalid values, use a manual count
    // For this specific SD card, we know there are 6 tracks
    mp3_state.totalTracks = 6;

    Serial.println("Using manual track count: 6 tracks");
}
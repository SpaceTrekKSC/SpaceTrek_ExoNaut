/*
 * ExoNaut_DotMatrix.h
 *
 * Author: Based on ExoNaut code by Andrew Gafford
 * Date: March 5th, 2025
 *
 * This library provides control for TM1640-based LED dot matrix displays
 * for the Space Trek ExoNaut Robot.
 */

#ifndef __EXONAUT_DOTMATRIX_h
#define __EXONAUT_DOTMATRIX_h

#include <Arduino.h>

// Define TM1640 pins - you should adjust these to match your wiring
#define TM1640_CLK_PIN 25 // Clock pin
#define TM1640_DIN_PIN 33 // Data pin

// Dot matrix configuration
#define TM1640_GRID_WIDTH 16 // 16 columns (2 x 8x8 matrices side by side)
#define TM1640_GRID_HEIGHT 8 // 8 rows

// TM1640 commands
#define TM1640_CMD_DISPLAY_MODE 0x00 // + 0x08 (display on), + 0x00 (display off)
#define TM1640_CMD_DATA_SET 0x40     // + 0x00 (normal), + 0x01 (test mode)
#define TM1640_CMD_ADDRESS 0xC0      // + Address (0x00 to 0x0F)
#define TM1640_CMD_BRIGHTNESS 0x88   // + 0x00 to 0x07 for brightness level

// Display brightness levels
#define TM1640_BRIGHTNESS_MIN 0
#define TM1640_BRIGHTNESS_MAX 7

class ExoNaut_DotMatrix
{
public:
    ExoNaut_DotMatrix(uint8_t clkPin = TM1640_CLK_PIN, uint8_t dinPin = TM1640_DIN_PIN);

    void begin();
    void clear();
    void setBrightness(uint8_t brightness);
    void setAllOn();  // Turn all LEDs on
    void setAllOff(); // Turn all LEDs off
    void setLED(uint8_t row, uint8_t col, bool state);
    void setRow(uint8_t row, uint8_t data);
    void setMatrix(const uint8_t data[]);

    // Number display functions
    void displayNumber(uint8_t number);           // Display a number from 0-99
    void displayNumberWithEffect(uint8_t number); // Display with scroll effect

private:
    uint8_t _clkPin;
    uint8_t _dinPin;
    uint8_t _displayBuffer[16]; // Buffer for display data

    // Digit patterns for 5x7 font (0-9)
    static const uint8_t DIGITS[10][5];

    void sendCommand(uint8_t cmd);
    void startTransmission();
    void endTransmission();
    void sendData(uint8_t address, uint8_t data);
    void sendData(uint8_t data);

    // Helper functions for number display
    void clearBuffer();
    void setPixel(int x, int y, bool state);
    void drawDigit(int digit, int xOffset, int yOffset);
};

#endif // __EXONAUT_DOTMATRIX_h

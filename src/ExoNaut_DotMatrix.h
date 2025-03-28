/*
 * ExoNaut_DotMatrix.h
 *
 * Author: Based on ExoNaut code by Andrew Gafford
 * Date: March 28th, 2025
 *
 * This library provides control for TM1640-based LED dot matrix displays
 * for the Space Trek ExoNaut Robot.
 *
 * The Dot Matrix module will work on ports 6 and 8 of the ExoNout robot.
 * The following table gives the port to pin mapping:
 *  __________________________
 *  PORT Number |   Pin Mapping
 *      6       |   CLK: 33, DIN: 25
 *      8       |   CLK: 26, DIN: 27
 */

#ifndef EXONAUT_DOTMATRIX_H
#define EXONAUT_DOTMATRIX_H

#include <Arduino.h>
#include "ExoNaut.h"

// Port specific pin mappings
#define PORT_6_CLK_PIN 25
#define PORT_6_DIN_PIN 33
#define PORT_8_CLK_PIN 27
#define PORT_8_DIN_PIN 26

// Default ports
#define DEFAULT_PORT 6
#define DEFAULT_CLK_PIN PORT_6_CLK_PIN
#define DEFAULT_DIN_PIN PORT_6_DIN_PIN

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

// Scrolling text constants
#define SCROLL_SPEED_DEFAULT 80 // Milliseconds between scroll updates (lower = faster)

class ExoNaut_DotMatrix
{
public:
    // Constructor with port number
    ExoNaut_DotMatrix(uint8_t port = DEFAULT_PORT);

    // Alternatively initialize with specific pins
    void setPins(uint8_t clkPin, uint8_t dinPin);

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

    // Text and scrolling functions
    void drawChar(char character, int xOffset, int yOffset);
    void scrollText(const char *text, uint8_t numScrolls = 0, uint8_t scrollSpeed = SCROLL_SPEED_DEFAULT);
    void stopScroll();
    bool isScrolling();
    void updateScroll(); // Call this from loop() to update scrolling text

private:
    uint8_t _clkPin;
    uint8_t _dinPin;
    uint8_t _port;
    uint8_t _displayBuffer[16]; // Buffer for display data

    // Scrolling text variables
    bool _scrollActive;
    unsigned long _lastScrollTime;
    int _scrollPosition;
    String _scrollText;
    int _totalScrollWidth;
    uint8_t _scrollSpeed;
    uint8_t _scrollsRemaining;
    uint8_t _scrollCount;

    // Digit patterns for 5x7 font (0-9)
    static const uint8_t DIGITS[10][5];
    // Character patterns for ASCII 32-127
    static const uint8_t CHARS[96][5];

    void sendCommand(uint8_t cmd);
    void startTransmission();
    void endTransmission();
    void sendData(uint8_t address, uint8_t data);
    void sendData(uint8_t data);

    // Helper functions
    void clearBuffer();
    void setPixel(int x, int y, bool state);
    void drawDigit(int digit, int xOffset, int yOffset);
    int getCharWidth(char character);
    int calculateTextPixelWidth(const char *text);
};

#endif // EXONAUT_DOTMATRIX_H

/*
 * ExoNaut_7Segment.h
 *
 * A simple, kid-friendly library for the 7-segment display on ExoNaut
 *
 * Author: Based on ExoNaut code by Andrew Gafford
 * Date: May 22, 2025
 *
 * This library makes it super easy for kids to use the 4-digit 7-segment display!
 * Just plug it into port 6 or 8 and start displaying numbers and simple text.
 */

#ifndef EXONAUT_7SEGMENT_H
#define EXONAUT_7SEGMENT_H

#include <Arduino.h>
#include <TM1640.h>

// Port pin mappings (same as dot matrix)
#define PORT_6_CLK_PIN 25
#define PORT_6_DATA_PIN 33
#define PORT_8_CLK_PIN 27
#define PORT_8_DATA_PIN 26

class ExoNaut_7Segment
{
public:
    // Constructor - just tell it which port (6 or 8)
    ExoNaut_7Segment(uint8_t port = 6);

    // Start the display
    void begin();

    // Simple functions for kids
    void showNumber(int number); // Show any number 0-9999
    void showText(String text);  // Show simple text like "HELP" or "COOL"
    void clear();                // Turn off all segments

    // Fun effects
    void countUp(int startNum, int endNum, int delayMs = 500);   // Count up animation
    void countDown(int startNum, int endNum, int delayMs = 500); // Count down animation
    void blink(int number, int times = 3, int delayMs = 500);    // Blink a number
    void scroll(String text, int delayMs = 300);                 // Scroll text across display

    // Individual digit control (for advanced kids)
    void showDigit(int digit, int position); // Show single digit (0-9) at position (0-3)
    void clearDigit(int position);           // Clear single digit at position (0-3)

    // Decimal point control
    void showDecimal(int number, int decimalPlace); // Show number with decimal point

private:
    TM1640 *_display;
    uint8_t _port;
    uint8_t _dataPin;
    uint8_t _clkPin;

    // Helper functions
    bool isValidPort(uint8_t port);
    void setPins(uint8_t port);
};

#endif // EXONAUT_7SEGMENT_H
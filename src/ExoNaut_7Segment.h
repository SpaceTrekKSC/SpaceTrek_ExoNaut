/*
 * ExoNaut_7Segment.h
 *
 * A simple, kid-friendly library for the 7-segment display on ExoNaut
 * Self-contained implementation without external TM1640 library dependency
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

 // Port pin mappings (same as dot matrix)
 #define PORT_6_CLK_PIN 25
 #define PORT_6_DATA_PIN 33
 #define PORT_8_CLK_PIN 27
 #define PORT_8_DATA_PIN 26

 // TM1640 commands for 7-segment display (matching TM16xx library)
 #define TM1640_CMD_DATA_SET 0x40      // Auto increment addressing
 #define TM1640_CMD_DATA_FIXED 0x44    // Fixed addressing
 #define TM1640_CMD_DISPLAY_CTRL 0x88  // Display control (on/off + brightness)
 #define TM1640_CMD_ADDR_SET 0xC0      // Address setting

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
     void showDecimal(float number);                 // ✅ NEW: Show a float number like 3.14

     // Brightness control
     void setBrightness(uint8_t brightness); // 0-7

 private:
     uint8_t _port;
     uint8_t _dataPin;
     uint8_t _clkPin;
     uint8_t _displayBuffer[4]; // Buffer for 4 digits

     // Helper functions
     bool isValidPort(uint8_t port);
     void setPins(uint8_t port);

     // Low-level TM1640 communication
     void startTransmission();
     void endTransmission();
     void sendByte(uint8_t data);
     void sendCommand(uint8_t cmd);
     void sendData(uint8_t addr, uint8_t data);
     void updateDisplay();
     void bitDelay();  // Add timing delay for fast MCUs

     // Character encoding
     uint8_t encodeDigit(uint8_t digit);
     uint8_t encodeChar(char c);

     // 7-segment patterns for digits 0-9
     static const uint8_t DIGIT_PATTERNS[10];
     // 7-segment patterns for letters A-Z
     static const uint8_t LETTER_PATTERNS[26];
 };

 #endif // EXONAUT_7SEGMENT_H
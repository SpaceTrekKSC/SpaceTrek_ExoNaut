/*
 * ExoNaut_7Segment.cpp
 *
 * Self-contained implementation without external TM1640 library dependency
 */

 #include "ExoNaut_7Segment.h"

 // 7-segment patterns for digits 0-9
 // Bit pattern: DP G F E D C B A
 const uint8_t ExoNaut_7Segment::DIGIT_PATTERNS[10] = {
     0b00111111, // 0
     0b00000110, // 1
     0b01011011, // 2
     0b01001111, // 3
     0b01100110, // 4
     0b01101101, // 5
     0b01111101, // 6
     0b00000111, // 7
     0b01111111, // 8
     0b01101111  // 9
 };
 
 // 7-segment patterns for letters A-Z (where possible)
 const uint8_t ExoNaut_7Segment::LETTER_PATTERNS[26] = {
     0b01110111, // A
     0b01111100, // b
     0b00111001, // C
     0b01011110, // d
     0b01111001, // E
     0b01110001, // F
     0b00111101, // G
     0b01110110, // H
     0b00000110, // I
     0b00001110, // J
     0b01110101, // K (approximation)
     0b00111000, // L
     0b00110111, // M (approximation)
     0b01010100, // n
     0b00111111, // O
     0b01110011, // P
     0b01100111, // q
     0b01010000, // r
     0b01101101, // S
     0b01111000, // t
     0b00111110, // U
     0b00011100, // v (approximation)
     0b00101010, // W (approximation)
     0b01110110, // X (same as H)
     0b01101110, // y
     0b01011011  // Z (same as 2)
 };
 
 ExoNaut_7Segment::ExoNaut_7Segment(uint8_t port)
 {
     _port = isValidPort(port) ? port : 6; // Default to port 6 if invalid
     setPins(_port);
     
     // Initialize display buffer
     for (int i = 0; i < 4; i++) {
         _displayBuffer[i] = 0;
     }
 }
 
 void ExoNaut_7Segment::begin()
 {
     // Set pin modes
     pinMode(_dataPin, OUTPUT);
     pinMode(_clkPin, OUTPUT);
     
     // Initialize pins to high (TM1640 default state)
     digitalWrite(_clkPin, HIGH);
     digitalWrite(_dataPin, HIGH);
     
     // Initialize display using TM16xx commands
     sendCommand(TM1640_CMD_DATA_SET);           // Data command setting (auto increment)
     sendCommand(TM1640_CMD_DISPLAY_CTRL | 0x07); // Display on, max brightness
     
     // Clear the display
     clear();
     delay(100);
 }
 
 void ExoNaut_7Segment::showNumber(int number)
 {
     // Limit to 4 digits (0-9999)
     number = (number < 0) ? 0 : ((number > 9999) ? 9999 : number);
     
     // Clear buffer
     for (int i = 0; i < 4; i++) {
         _displayBuffer[i] = 0;
     }
     
     // Handle special case of 0
     if (number == 0) {
         _displayBuffer[3] = DIGIT_PATTERNS[0];
     } else {
         // Extract digits and fill buffer from right to left
         int pos = 3;
         while (number > 0 && pos >= 0) {
             int digit = number % 10;
             _displayBuffer[pos] = DIGIT_PATTERNS[digit];
             number /= 10;
             pos--;
         }
     }
     
     updateDisplay();
 }
 
 void ExoNaut_7Segment::showText(String text)
 {
     // Clear buffer
     for (int i = 0; i < 4; i++) {
         _displayBuffer[i] = 0;
     }
     
     // Convert text to uppercase and limit to 4 characters
     text.toUpperCase();
     int len = (text.length() > 4) ? 4 : text.length(); // Fix type mismatch
     
     // Encode characters
     for (int i = 0; i < len; i++) {
         _displayBuffer[i] = encodeChar(text[i]);
     }
     
     updateDisplay();
 }
 
 void ExoNaut_7Segment::clear()
 {
     for (int i = 0; i < 4; i++) {
         _displayBuffer[i] = 0;
     }
     updateDisplay();
 }
 
 void ExoNaut_7Segment::countUp(int startNum, int endNum, int delayMs)
 {
     startNum = (startNum < 0) ? 0 : ((startNum > 9999) ? 9999 : startNum);
     endNum = (endNum < 0) ? 0 : ((endNum > 9999) ? 9999 : endNum);
 
     if (startNum <= endNum) {
         for (int i = startNum; i <= endNum; i++) {
             showNumber(i);
             delay(delayMs);
         }
     }
 }
 
 void ExoNaut_7Segment::countDown(int startNum, int endNum, int delayMs)
 {
     startNum = (startNum < 0) ? 0 : ((startNum > 9999) ? 9999 : startNum);
     endNum = (endNum < 0) ? 0 : ((endNum > 9999) ? 9999 : endNum);
 
     if (startNum >= endNum) {
         for (int i = startNum; i >= endNum; i--) {
             showNumber(i);
             delay(delayMs);
         }
     }
 }
 
 void ExoNaut_7Segment::blink(int number, int times, int delayMs)
 {
     for (int i = 0; i < times; i++) {
         showNumber(number);
         delay(delayMs);
         clear();
         delay(delayMs);
     }
     // Show the number one final time
     showNumber(number);
 }
 
 void ExoNaut_7Segment::scroll(String text, int delayMs)
 {
     if (text.length() <= 4) {
         // If text fits, just show it
         showText(text);
         return;
     }
 
     // Add spaces to make scrolling smoother
     text = "    " + text + "    ";
 
     // Scroll through the text
     for (int i = 0; i <= text.length() - 4; i++) {
         String segment = text.substring(i, i + 4);
         showText(segment);
         delay(delayMs);
     }
 }
 
 void ExoNaut_7Segment::showDigit(int digit, int position)
 {
     digit = (digit < 0) ? 0 : ((digit > 9) ? 9 : digit);
     position = (position < 0) ? 0 : ((position > 3) ? 3 : position);
 
     _displayBuffer[position] = DIGIT_PATTERNS[digit];
     updateDisplay();
 }
 
 void ExoNaut_7Segment::clearDigit(int position)
 {
     position = (position < 0) ? 0 : ((position > 3) ? 3 : position);
     _displayBuffer[position] = 0;
     updateDisplay();
 }
 
 void ExoNaut_7Segment::showDecimal(int number, int decimalPlace)
 {
     number = (number < 0) ? 0 : ((number > 9999) ? 9999 : number);
     decimalPlace = (decimalPlace < 0) ? 0 : ((decimalPlace > 3) ? 3 : decimalPlace);
 
     showNumber(number);
     
     // Add decimal point to specified position
     if (decimalPlace < 4) {
         _displayBuffer[decimalPlace] |= 0x80; // Set DP bit
         updateDisplay();
     }
 }
 
 bool ExoNaut_7Segment::isValidPort(uint8_t port)
 {
     return (port == 6 || port == 8);
 }
 
 void ExoNaut_7Segment::setPins(uint8_t port)
 {
     switch (port) {
     case 6:
         _dataPin = PORT_6_DATA_PIN;
         _clkPin = PORT_6_CLK_PIN;
         break;
     case 8:
         _dataPin = PORT_8_DATA_PIN;
         _clkPin = PORT_8_CLK_PIN;
         break;
     default:
         // Default to port 6
         _dataPin = PORT_6_DATA_PIN;
         _clkPin = PORT_6_CLK_PIN;
         break;
     }
 }
 
 void ExoNaut_7Segment::startTransmission()
 {
     // TM1640 start condition: when CLK is high, DIN becomes low from high
     digitalWrite(_dataPin, LOW);
     digitalWrite(_clkPin, LOW);
     bitDelay();
 }
 
 void ExoNaut_7Segment::endTransmission()
 {
     // TM1640 stop condition: when CLK is high, DIN becomes high from low
     digitalWrite(_clkPin, HIGH);
     digitalWrite(_dataPin, HIGH);
     bitDelay();
 }
 
 void ExoNaut_7Segment::sendByte(uint8_t data)
 {
     // Send 8 bits LSB first (matching TM16xx::send method)
     for (int i = 0; i < 8; i++) {
         digitalWrite(_clkPin, LOW);
         bitDelay();
         digitalWrite(_dataPin, data & 1 ? HIGH : LOW);
         bitDelay();
         data >>= 1;
         digitalWrite(_clkPin, HIGH);
         bitDelay();
     }
     bitDelay(); // TM1638 specifies a Twait between bytes of minimal 1us
     
     // TM1640 specific: wants data and clock to be low after sending data
     digitalWrite(_clkPin, LOW);    // first clock low
     digitalWrite(_dataPin, LOW);   // then data low
     bitDelay();
 }
 
 void ExoNaut_7Segment::sendCommand(uint8_t cmd)
 {
     startTransmission();
     sendByte(cmd);
     endTransmission();
 }
 
 void ExoNaut_7Segment::sendData(uint8_t addr, uint8_t data)
 {
     sendCommand(TM1640_CMD_DATA_FIXED); // Fixed addressing mode
     
     startTransmission();
     sendByte(TM1640_CMD_ADDR_SET | addr);
     sendByte(data);
     endTransmission();
 }
 
 void ExoNaut_7Segment::updateDisplay()
 {
     // Use auto increment addressing to send all 4 digits at once
     sendCommand(TM1640_CMD_DATA_SET); // Auto increment mode
     
     startTransmission();
     sendByte(TM1640_CMD_ADDR_SET | 0); // Start at address 0
     for (int i = 0; i < 4; i++) {
         sendByte(_displayBuffer[i]);
     }
     endTransmission();
 }
 
 void ExoNaut_7Segment::setBrightness(uint8_t brightness)
 {
     brightness = (brightness > 7) ? 7 : brightness;
     sendCommand(TM1640_CMD_DISPLAY_CTRL | brightness);
 }
 
 // Add bit delay function matching TM16xx implementation
 void ExoNaut_7Segment::bitDelay()
 {
     // Add delay for fast MCUs (like ESP32)
     #if F_CPU > 100000000
         delayMicroseconds(1);
     #endif
 }
 
 uint8_t ExoNaut_7Segment::encodeDigit(uint8_t digit)
 {
     if (digit <= 9) {
         return DIGIT_PATTERNS[digit];
     }
     return 0; // Blank for invalid digit
 }
 
 uint8_t ExoNaut_7Segment::encodeChar(char c)
 {
     if (c >= '0' && c <= '9') {
         return DIGIT_PATTERNS[c - '0'];
     } else if (c >= 'A' && c <= 'Z') {
         return LETTER_PATTERNS[c - 'A'];
     } else if (c >= 'a' && c <= 'z') {
         return LETTER_PATTERNS[c - 'a'];
     } else if (c == ' ') {
         return 0; // Blank for space
     } else if (c == '-') {
         return 0b01000000; // Just the middle segment
     }
     return 0; // Blank for unsupported characters
 }
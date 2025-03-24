/*
 * ExoNaut_DotMatrix.cpp
 *
 * Author: Based on ExoNaut code by Andrew Gafford
 * Date: March 5th, 2025
 *
 * Implementation of TM1640 LED dot matrix display control
 * for the Space Trek ExoNaut Robot, including scrolling text functionality.
 */

#include "ExoNaut_DotMatrix.h"

// Define 5x7 font for digits 0-9 (larger and more visible)
const uint8_t ExoNaut_DotMatrix::DIGITS[10][5] = {
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 0/*
 * ExoNaut_DotMatrix.cpp
 *
 * Author: Based on ExoNaut code by Andrew Gafford
 * Date: March 5th, 2025
 *
 * Implementation of TM1640 LED dot matrix display control
 * for the Space Trek ExoNaut Robot.
 */

#include "ExoNaut_DotMatrix.h"

// Define 5x7 font for digits 0-9 (larger and more visible)
const uint8_t ExoNaut_DotMatrix::DIGITS[10][5] = {
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x62, 0x51, 0x49, 0x49, 0x46}, // 2
    {0x22, 0x41, 0x49, 0x49, 0x36}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}  // 9
};

// Constructor
ExoNaut_DotMatrix::ExoNaut_DotMatrix(uint8_t clkPin, uint8_t dinPin)
{
    _clkPin = clkPin;
    _dinPin = dinPin;

    // Initialize buffer
    clearBuffer();
}

// Initialize the display
void ExoNaut_DotMatrix::begin()
{
    // Set pin modes
    pinMode(_clkPin, OUTPUT);
    pinMode(_dinPin, OUTPUT);

    // Set pins to initial state
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dinPin, HIGH);
    delay(100); // Give hardware time to stabilize

    // Initialize the display with proper control sequence for TM1640
    // 1. Set data command (auto increment address)
    sendCommand(TM1640_CMD_DATA_SET | 0x00);

    // 2. Set display control (turn it on with max brightness)
    sendCommand(TM1640_CMD_DISPLAY_MODE | 0x08);

    // 3. Clear the display (all LEDs off)
    clear();

    // 4. Small delay to ensure initialization completes
    delay(100);
}

// Clear the display (all LEDs off)
void ExoNaut_DotMatrix::clear()
{
    // Start with address 0
    startTransmission();
    sendData(TM1640_CMD_ADDRESS | 0x00);

    // Send 16 bytes of zeros to clear all positions
    for (uint8_t i = 0; i < 16; i++)
    {
        sendData(0x00);
    }

    endTransmission();

    // Also clear the buffer
    clearBuffer();
}

// Set display brightness
void ExoNaut_DotMatrix::setBrightness(uint8_t brightness)
{
    // Ensure brightness is in valid range
    brightness = constrain(brightness, TM1640_BRIGHTNESS_MIN, TM1640_BRIGHTNESS_MAX);
    sendCommand(TM1640_CMD_BRIGHTNESS | brightness);
}

// Turn all LEDs on
void ExoNaut_DotMatrix::setAllOn()
{
    // Start with address 0
    startTransmission();
    sendData(TM1640_CMD_ADDRESS | 0x00);

    // Send 16 bytes of 0xFF to turn on all LEDs
    for (uint8_t i = 0; i < 16; i++)
    {
        sendData(0xFF);
    }

    endTransmission();
}

// Turn all LEDs off (same as clear)
void ExoNaut_DotMatrix::setAllOff()
{
    clear();
}

// Set an individual LED's state
void ExoNaut_DotMatrix::setLED(uint8_t row, uint8_t col, bool state)
{
    if (row >= 8 || col >= 16)
        return; // Bounds check

    // In TM1640 matrices, columns are often grouped by 8 with each byte
    // controlling a set of 8 LEDs in a column
    uint8_t grid = col / 8;      // Which 8x8 grid (0 or 1)
    uint8_t colInGrid = col % 8; // Column within the grid

    // Calculate the address
    uint8_t addr = grid * 8 + colInGrid;

    // Read current data (not actually possible with TM1640, we need to track state)
    // For now, we'll just set the whole column
    uint8_t data = 0;
    if (state)
    {
        data = (1 << row);
    }

    // Write to display
    sendData(TM1640_CMD_ADDRESS | addr, data);
}

// Set an entire row at once with a byte pattern
void ExoNaut_DotMatrix::setRow(uint8_t row, uint8_t pattern)
{
    if (row >= 8)
        return; // Bounds check

    // For a row, we need to set the appropriate bit in each column
    for (uint8_t col = 0; col < 16; col++)
    {
        setLED(row, col, (pattern & (1 << col)) != 0);
    }
}

// Set the entire matrix at once with an array of patterns
void ExoNaut_DotMatrix::setMatrix(const uint8_t data[])
{
    // Start with address 0
    startTransmission();
    sendData(TM1640_CMD_ADDRESS | 0x00);

    // Send all 16 bytes
    for (uint8_t i = 0; i < 16; i++)
    {
        sendData(data[i]);
    }

    endTransmission();
}

// Clear the internal display buffer
void ExoNaut_DotMatrix::clearBuffer()
{
    for (int i = 0; i < 16; i++)
    {
        _displayBuffer[i] = 0;
    }
}

// Set a pixel in the buffer
void ExoNaut_DotMatrix::setPixel(int x, int y, bool state)
{
    if (x >= 0 && x < 16 && y >= 0 && y < 8)
    {
        if (state)
        {
            _displayBuffer[x] |= (1 << y);
        }
        else
        {
            _displayBuffer[x] &= ~(1 << y);
        }
    }
}

// Draw a digit at a specific position
void ExoNaut_DotMatrix::drawDigit(int digit, int xOffset, int yOffset)
{
    if (digit < 0 || digit > 9)
        return;

    for (int col = 0; col < 5; col++)
    {
        uint8_t pattern = DIGITS[digit][col];

        for (int row = 0; row < 7; row++)
        {
            bool isOn = (pattern & (1 << row)) != 0;
            setPixel(xOffset + col, yOffset + row, isOn);
        }
    }
}

// Display a number (0-99) on the dot matrix
void ExoNaut_DotMatrix::displayNumber(uint8_t number)
{
    // Ensure number is within range
    number = constrain(number, 0, 99);

    // Clear the display buffer
    clearBuffer();

    // Extract the two digits
    int tens = number / 10;
    int ones = number % 10;

    // Draw the tens digit (positioned on left side)
    drawDigit(tens, 1, 0);

    // Draw the ones digit (positioned on right side)
    drawDigit(ones, 9, 0);

    // Update the display
    setMatrix(_displayBuffer);
}

// Display a number with scroll effect
void ExoNaut_DotMatrix::displayNumberWithEffect(uint8_t number)
{
    // Ensure number is within range
    number = constrain(number, 0, 99);

    // Extract the two digits
    int tens = number / 10;
    int ones = number % 10;

    // Scroll effect - slide in from the bottom
    for (int y = 8; y >= 0; y--)
    {
        clearBuffer();
        drawDigit(tens, 1, y);
        drawDigit(ones, 9, y);
        setMatrix(_displayBuffer);
        delay(30);
    }
}

// Send a command to the TM1640
void ExoNaut_DotMatrix::sendCommand(uint8_t cmd)
{
    startTransmission();
    sendData(cmd);
    endTransmission();
}

// Start a transmission sequence
void ExoNaut_DotMatrix::startTransmission()
{
    digitalWrite(_dinPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dinPin, LOW);
    delayMicroseconds(2);
}

// End a transmission sequence
void ExoNaut_DotMatrix::endTransmission()
{
    digitalWrite(_clkPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dinPin, HIGH);
    delayMicroseconds(2);
}

// Send data with an address
void ExoNaut_DotMatrix::sendData(uint8_t address, uint8_t data)
{
    startTransmission();
    sendData(address);
    sendData(data);
    endTransmission();
}

// Send a byte of data (used by other methods)
void ExoNaut_DotMatrix::sendData(uint8_t data)
{
    // Send 8 bits LSB first
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(_clkPin, LOW);
        delayMicroseconds(2);

        // Set data bit
        if (data & 0x01)
        {
            digitalWrite(_dinPin, HIGH);
        }
        else
        {
            digitalWrite(_dinPin, LOW);
        }

        data >>= 1; // Shift to next bit

        delayMicroseconds(2);
        digitalWrite(_clkPin, HIGH);
        delayMicroseconds(2);
    }
}
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x62, 0x51, 0x49, 0x49, 0x46}, // 2
    {0x22, 0x41, 0x49, 0x49, 0x36}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}  // 9
};

// Define character set for ASCII chars 32-127 (space through DEL)
// This is a 5x7 font, with each column represented as a byte
// Only showing the first few characters here for brevity
const uint8_t ExoNaut_DotMatrix::CHARS[96][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space (32)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x56, 0x20, 0x50}, // &
    {0x00, 0x08, 0x07, 0x03, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x2A, 0x1C, 0x7F, 0x1C, 0x2A}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x80, 0x70, 0x30, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x00, 0x60, 0x60, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    // Digits 0-9 (ASCII 48-57) are referenced through DIGITS array
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 0 (48)
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x62, 0x51, 0x49, 0x49, 0x46}, // 2
    {0x22, 0x41, 0x49, 0x49, 0x36}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x3E, 0x41, 0x5D, 0x59, 0x4E}, // @
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, // A (65)
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // \
     {0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a (97)
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    {0x18, 0xA4, 0xA4, 0xA4, 0x7C}, // g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    {0x40, 0x80, 0x84, 0x7D, 0x00}, // j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0xFC, 0x24, 0x24, 0x24, 0x18}, // p
    {0x18, 0x24, 0x24, 0x18, 0xFC}, // q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x
    {0x1C, 0xA0, 0xA0, 0xA0, 0x7C}, // y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // {
    {0x00, 0x00, 0x77, 0x00, 0x00}, // |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // }
    {0x02, 0x01, 0x02, 0x04, 0x02}, // ~
    {0x3C, 0x26, 0x23, 0x26, 0x3C}  // DEL
};

// Constructor
ExoNaut_DotMatrix::ExoNaut_DotMatrix(uint8_t clkPin, uint8_t dinPin)
{
    _clkPin = clkPin;
    _dinPin = dinPin;

    // Initialize scrolling variables
    _scrollActive = false;
    _lastScrollTime = 0;
    _scrollPosition = 0;
    _scrollText = "";
    _totalScrollWidth = 0;
    _scrollSpeed = SCROLL_SPEED_DEFAULT;
    _scrollsRemaining = 0;
    _scrollCount = 0;

    // Initialize buffer
    clearBuffer();
}

// Initialize the display
void ExoNaut_DotMatrix::begin()
{
    // Set pin modes
    pinMode(_clkPin, OUTPUT);
    pinMode(_dinPin, OUTPUT);

    // Set pins to initial state
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dinPin, HIGH);
    delay(100); // Give hardware time to stabilize

    // Initialize the display with proper control sequence for TM1640
    // 1. Set data command (auto increment address)
    sendCommand(TM1640_CMD_DATA_SET | 0x00);

    // 2. Set display control (turn it on with max brightness)
    sendCommand(TM1640_CMD_DISPLAY_MODE | 0x08);

    // 3. Clear the display (all LEDs off)
    clear();

    // 4. Small delay to ensure initialization completes
    delay(100);
}

// Clear the display (all LEDs off)
void ExoNaut_DotMatrix::clear()
{
    // Start with address 0
    startTransmission();
    sendData(TM1640_CMD_ADDRESS | 0x00);

    // Send 16 bytes of zeros to clear all positions
    for (uint8_t i = 0; i < 16; i++)
    {
        sendData(0x00);
    }

    endTransmission();

    // Also clear the buffer
    clearBuffer();
}

// Set display brightness
void ExoNaut_DotMatrix::setBrightness(uint8_t brightness)
{
    // Ensure brightness is in valid range
    brightness = constrain(brightness, TM1640_BRIGHTNESS_MIN, TM1640_BRIGHTNESS_MAX);
    sendCommand(TM1640_CMD_BRIGHTNESS | brightness);
}

// Turn all LEDs on
void ExoNaut_DotMatrix::setAllOn()
{
    // Start with address 0
    startTransmission();
    sendData(TM1640_CMD_ADDRESS | 0x00);

    // Send 16 bytes of 0xFF to turn on all LEDs
    for (uint8_t i = 0; i < 16; i++)
    {
        sendData(0xFF);
    }

    endTransmission();
}

// Turn all LEDs off (same as clear)
void ExoNaut_DotMatrix::setAllOff()
{
    clear();
}

// Set an individual LED's state
void ExoNaut_DotMatrix::setLED(uint8_t row, uint8_t col, bool state)
{
    if (row >= 8 || col >= 16)
        return; // Bounds check

    // In TM1640 matrices, columns are often grouped by 8 with each byte
    // controlling a set of 8 LEDs in a column
    uint8_t grid = col / 8;      // Which 8x8 grid (0 or 1)
    uint8_t colInGrid = col % 8; // Column within the grid

    // Calculate the address
    uint8_t addr = grid * 8 + colInGrid;

    // Read current data (not actually possible with TM1640, we need to track state)
    // For now, we'll just set the whole column
    uint8_t data = 0;
    if (state)
    {
        data = (1 << row);
    }

    // Write to display
    sendData(TM1640_CMD_ADDRESS | addr, data);
}

// Set an entire row at once with a byte pattern
void ExoNaut_DotMatrix::setRow(uint8_t row, uint8_t pattern)
{
    if (row >= 8)
        return; // Bounds check

    // For a row, we need to set the appropriate bit in each column
    for (uint8_t col = 0; col < 16; col++)
    {
        setLED(row, col, (pattern & (1 << col)) != 0);
    }
}

// Set the entire matrix at once with an array of patterns
void ExoNaut_DotMatrix::setMatrix(const uint8_t data[])
{
    // Start with address 0
    startTransmission();
    sendData(TM1640_CMD_ADDRESS | 0x00);

    // Send all 16 bytes
    for (uint8_t i = 0; i < 16; i++)
    {
        sendData(data[i]);
    }

    endTransmission();
}

// Clear the internal display buffer
void ExoNaut_DotMatrix::clearBuffer()
{
    for (int i = 0; i < 16; i++)
    {
        _displayBuffer[i] = 0;
    }
}

// Set a pixel in the buffer
void ExoNaut_DotMatrix::setPixel(int x, int y, bool state)
{
    if (x >= 0 && x < 16 && y >= 0 && y < 8)
    {
        if (state)
        {
            _displayBuffer[x] |= (1 << y);
        }
        else
        {
            _displayBuffer[x] &= ~(1 << y);
        }
    }
}

// Draw a digit at a specific position
void ExoNaut_DotMatrix::drawDigit(int digit, int xOffset, int yOffset)
{
    if (digit < 0 || digit > 9)
        return;

    for (int col = 0; col < 5; col++)
    {
        uint8_t pattern = DIGITS[digit][col];

        for (int row = 0; row < 7; row++)
        {
            bool isOn = (pattern & (1 << row)) != 0;
            setPixel(xOffset + col, yOffset + row, isOn);
        }
    }
}

// Draw a character at a specific position
void ExoNaut_DotMatrix::drawChar(char character, int xOffset, int yOffset)
{
    // Convert character to index in CHARS array
    uint8_t charIndex = character - 32; // ASCII 32 (space) is the first character in our array

    // Bounds check
    if (charIndex >= 96)
        return;

    // Draw the character
    for (int col = 0; col < 5; col++)
    {
        uint8_t pattern;

        // If the character is a digit (0-9), use DIGITS array instead
        if (character >= '0' && character <= '9')
        {
            pattern = DIGITS[character - '0'][col];
        }
        else
        {
            pattern = CHARS[charIndex][col];
        }

        for (int row = 0; row < 7; row++)
        {
            bool isOn = (pattern & (1 << row)) != 0;
            setPixel(xOffset + col, yOffset + row, isOn);
        }
    }
}

// Display a number (0-99) on the dot matrix
void ExoNaut_DotMatrix::displayNumber(uint8_t number)
{
    // Ensure number is within range
    number = constrain(number, 0, 99);

    // Clear the display buffer
    clearBuffer();

    // Extract the two digits
    int tens = number / 10;
    int ones = number % 10;

    // Draw the tens digit (positioned on left side)
    drawDigit(tens, 1, 0);

    // Draw the ones digit (positioned on right side)
    drawDigit(ones, 9, 0);

    // Update the display
    setMatrix(_displayBuffer);
}

// Display a number with scroll effect
void ExoNaut_DotMatrix::displayNumberWithEffect(uint8_t number)
{
    // Ensure number is within range
    number = constrain(number, 0, 99);

    // Extract the two digits
    int tens = number / 10;
    int ones = number % 10;

    // Scroll effect - slide in from the bottom
    for (int y = 8; y >= 0; y--)
    {
        clearBuffer();
        drawDigit(tens, 1, y);
        drawDigit(ones, 9, y);
        setMatrix(_displayBuffer);
        delay(30);
    }
}

// Get the width of a character in pixels
int ExoNaut_DotMatrix::getCharWidth(char character)
{
    // Most characters are 5 pixels wide
    // Space can be narrower (3 pixels)
    return (character == ' ') ? 3 : 5;
}

// Calculate the total pixel width of a text string
int ExoNaut_DotMatrix::calculateTextPixelWidth(const char *text)
{
    int width = 0;
    for (size_t i = 0; i < strlen(text); i++)
    {
        width += getCharWidth(text[i]);
        // Add spacing between characters (except after the last character)
        if (i < strlen(text) - 1)
        {
            width += CHAR_SPACING;
        }
    }
    return width;
}

// Start scrolling text across the display
void ExoNaut_DotMatrix::scrollText(const char *text, uint8_t numScrolls, uint8_t scrollSpeed)
{
    _scrollText = text;
    _scrollSpeed = scrollSpeed;
    _scrollsRemaining = numScrolls;
    _scrollCount = 0;
    _scrollPosition = TM1640_GRID_WIDTH; // Start just off the right edge
    _totalScrollWidth = calculateTextPixelWidth(text);
    _scrollActive = true;
    _lastScrollTime = millis();

    // Immediately draw the first frame
    updateScroll();
}

// Stop scrolling
void ExoNaut_DotMatrix::stopScroll()
{
    _scrollActive = false;
}

// Check if scrolling is active
bool ExoNaut_DotMatrix::isScrolling()
{
    return _scrollActive;
}

// Update the scrolling display - call this from loop()
void ExoNaut_DotMatrix::updateScroll()
{
    // If not scrolling, just return
    if (!_scrollActive)
    {
        return;
    }

    // Check if it's time to update the scroll position
    unsigned long currentTime = millis();
    if (currentTime - _lastScrollTime < _scrollSpeed)
    {
        return;
    }

    // Time to update the scroll
    _lastScrollTime = currentTime;

    // Clear the buffer for the next frame
    clearBuffer();

    // Draw characters at their current positions
    int xPos = _scrollPosition;
    for (size_t i = 0; i < _scrollText.length(); i++)
    {
        // Only draw characters that might be visible
        if (xPos > -5 && xPos < TM1640_GRID_WIDTH)
        {
            drawChar(_scrollText[i], xPos, 0);
        }

        // Move to the next character position
        xPos += getCharWidth(_scrollText[i]) + CHAR_SPACING;
    }

    // Update the display
    setMatrix(_displayBuffer);

    // Move the scroll position for the next update
    _scrollPosition--;

    // Check if we've scrolled the entire text off the left side
    if (_scrollPosition < -(_totalScrollWidth))
    {
        // If unlimited scrolling, or we haven't reached our scroll count limit
        if (_scrollsRemaining == 0 || _scrollCount < _scrollsRemaining)
        {
            // Reset position to start scrolling again
            _scrollPosition = TM1640_GRID_WIDTH;
            _scrollCount++;
        }
        else
        {
            // We're done scrolling
            _scrollActive = false;
            clear(); // Clear the display
        }
    }
}

// Send a command to the TM1640
void ExoNaut_DotMatrix::sendCommand(uint8_t cmd)
{
    startTransmission();
    sendData(cmd);
    endTransmission();
}

// Start a transmission sequence
void ExoNaut_DotMatrix::startTransmission()
{
    digitalWrite(_dinPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dinPin, LOW);
    delayMicroseconds(2);
}

// End a transmission sequence
void ExoNaut_DotMatrix::endTransmission()
{
    digitalWrite(_clkPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dinPin, HIGH);
    delayMicroseconds(2);
}

// Send data with an address
void ExoNaut_DotMatrix::sendData(uint8_t address, uint8_t data)
{
    startTransmission();
    sendData(address);
    sendData(data);
    endTransmission();
}

// Send a byte of data (used by other methods)
void ExoNaut_DotMatrix::sendData(uint8_t data)
{
    // Send 8 bits LSB first
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(_clkPin, LOW);
        delayMicroseconds(2);

        // Set data bit
        if (data & 0x01)
        {
            digitalWrite(_dinPin, HIGH);
        }
        else
        {
            digitalWrite(_dinPin, LOW);
        }

        data >>= 1; // Shift to next bit

        delayMicroseconds(2);
        digitalWrite(_clkPin, HIGH);
        delayMicroseconds(2);
    }
}

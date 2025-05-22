/*
 * ExoNaut_7Segment.cpp
 *
 * A simple, kid-friendly implementation for the 7-segment display
 */

#include "ExoNaut_7Segment.h"

ExoNaut_7Segment::ExoNaut_7Segment(uint8_t port)
{
    _port = isValidPort(port) ? port : 6; // Default to port 6 if invalid
    setPins(_port);
    _display = nullptr;
}

void ExoNaut_7Segment::begin()
{
    // Create the display object with the correct pins
    _display = new TM1640(_dataPin, _clkPin, 4);

    // Clear the display to start
    clear();
    delay(100);
}

void ExoNaut_7Segment::showNumber(int number)
{
    if (_display == nullptr)
        return;

    // Limit to 4 digits (0-9999)
    number = constrain(number, 0, 9999);

    // Use the TM1640 function to display the number
    _display->setDisplayToDecNumber(number, 0); // 0 means no decimal point
}

void ExoNaut_7Segment::showText(String text)
{
    if (_display == nullptr)
        return;

    // Convert String to char array for TM1640
    char textArray[5];  // 4 characters + null terminator
    text.toUpperCase(); // Make it uppercase for better display
    text.substring(0, 4).toCharArray(textArray, 5);

    _display->setDisplayToString(textArray);
}

void ExoNaut_7Segment::clear()
{
    if (_display == nullptr)
        return;
    _display->clearDisplay();
}

void ExoNaut_7Segment::countUp(int startNum, int endNum, int delayMs)
{
    startNum = constrain(startNum, 0, 9999);
    endNum = constrain(endNum, 0, 9999);

    if (startNum <= endNum)
    {
        for (int i = startNum; i <= endNum; i++)
        {
            showNumber(i);
            delay(delayMs);
        }
    }
}

void ExoNaut_7Segment::countDown(int startNum, int endNum, int delayMs)
{
    startNum = constrain(startNum, 0, 9999);
    endNum = constrain(endNum, 0, 9999);

    if (startNum >= endNum)
    {
        for (int i = startNum; i >= endNum; i--)
        {
            showNumber(i);
            delay(delayMs);
        }
    }
}

void ExoNaut_7Segment::blink(int number, int times, int delayMs)
{
    for (int i = 0; i < times; i++)
    {
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
    if (text.length() <= 4)
    {
        // If text fits, just show it
        showText(text);
        return;
    }

    // Add spaces to make scrolling smoother
    text = "    " + text + "    ";

    // Scroll through the text
    for (int i = 0; i <= text.length() - 4; i++)
    {
        String segment = text.substring(i, i + 4);
        showText(segment);
        delay(delayMs);
    }
}

void ExoNaut_7Segment::showDigit(int digit, int position)
{
    if (_display == nullptr)
        return;

    digit = constrain(digit, 0, 9);
    position = constrain(position, 0, 3);

    _display->setDisplayDigit(digit, position);
}

void ExoNaut_7Segment::clearDigit(int position)
{
    if (_display == nullptr)
        return;

    position = constrain(position, 0, 3);
    _display->clearDisplayDigit(position);
}

void ExoNaut_7Segment::showDecimal(int number, int decimalPlace)
{
    if (_display == nullptr)
        return;

    number = constrain(number, 0, 9999);
    decimalPlace = constrain(decimalPlace, 1, 4);

    // TM1640 uses _BV() for decimal places, numbered 1-4 from right to left
    _display->setDisplayToDecNumber(number, _BV(decimalPlace));
}

bool ExoNaut_7Segment::isValidPort(uint8_t port)
{
    return (port == 6 || port == 8);
}

void ExoNaut_7Segment::setPins(uint8_t port)
{
    switch (port)
    {
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
/*
 * ExoNaut_Knob.h
 *
 * A library for interfacing with analog knob modules on ESP32 via ADC.
 * Optimized for use with the ExoNaut robot platform.
 *
 * Created: March 3, 2025
 */

#ifndef EXONAUT_KNOB_H
#define EXONAUT_KNOB_H

#include <Arduino.h>
#include "ExoNaut.h"

// Configuration values
#define KNOB_MIN_VALUE 0
#define KNOB_MAX_VALUE 4095 // 12-bit ADC resolution

class ExoNaut_Knob
{
public:
  // Constructor - uses port instead of pin
  ExoNaut_Knob();

  // Initialize and configure the knob module with a specific port
  bool begin(uint8_t port);

  // Read raw ADC value (0 to 4095 for 12-bit resolution)
  int readRaw(void);

  // Read mapped value (between minValue and maxValue)
  int readMapped(int minVal = 0, int maxVal = 100);

  // Set the range for mapped values (updates stored min/max)
  void setRange(int minVal, int maxVal);

  // Get a percentage (0-100%)
  int readPercent(void);

  // Check if knob position has changed since last read
  bool hasChanged(int threshold = 10);

  // Smoothing functionality
  void setSmoothing(bool enable, int samples = 10);

  // Update internal state (call this in loop)
  void update(void);

private:
  // Pin assignment
  uint8_t _knobPin; // Analog input pin for knob

  // Value tracking
  int _lastRawValue;    // Last raw reading
  int _currentRawValue; // Current raw reading
  int _minValue;        // Minimum mapped value
  int _maxValue;        // Maximum mapped value

  // Smoothing
  bool _smoothingEnabled;    // Whether smoothing is enabled
  int _smoothingSamples;     // Number of samples for smoothing
  int *_smoothingBuffer;     // Buffer for smoothing samples
  int _smoothingIndex;       // Current index in smoothing buffer
  bool _smoothingBufferFull; // Whether buffer has been filled once

  // Private methods
  int calculateSmoothedValue(void); // Calculate smoothed value from buffer
};

#endif // EXONAUT_KNOB_H

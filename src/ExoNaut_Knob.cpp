/*
 * ExoNaut_Knob.cpp
 *
 * Implementation for the ExoNaut_Knob library.
 * Provides functionality for analog knob (potentiometer) sensing.
 *
 * Created: March 3, 2025
 */

#include "ExoNaut_Knob.h"

ExoNaut_Knob::ExoNaut_Knob()
{
  // Initialize values
  _knobPin = 0; // Will be set in begin() based on port
  _lastRawValue = 0;
  _currentRawValue = 0;
  _minValue = 0;
  _maxValue = 100;

  // Smoothing (initialize without allocation)
  _smoothingEnabled = false;
  _smoothingSamples = 10;
  _smoothingBuffer = NULL;
  _smoothingIndex = 0;
  _smoothingBufferFull = false;
}

bool ExoNaut_Knob::begin(uint8_t port)
{
  // Determine pin based on port number
  switch (port)
  {
  case 1:
    _knobPin = 36; // Port 1 is PIN 36
    break;
  case 2:
    _knobPin = 32; // Port 2 is PIN 32
    break;
  case 6:
    _knobPin = 33; // Port 6 is PIN 33
    break;
  case 8:
    _knobPin = 26; // Port 8 is PIN 26
    break;
  default:
    // Invalid port, use a default (Port 1)
    _knobPin = 36;
    break;
  }

  // Configure analog input for knob
  pinMode(_knobPin, INPUT);

  // Set ADC resolution to 12 bits for ESP32
  analogReadResolution(12);

  // Take an initial reading
  _currentRawValue = analogRead(_knobPin);
  _lastRawValue = _currentRawValue;

  // Allocate smoothing buffer if enabled
  if (_smoothingEnabled && _smoothingBuffer == NULL)
  {
    _smoothingBuffer = new int[_smoothingSamples];
    if (_smoothingBuffer == NULL)
    {
      // Allocation failed
      _smoothingEnabled = false;
      return false;
    }

    // Initialize buffer with current value
    for (int i = 0; i < _smoothingSamples; i++)
    {
      _smoothingBuffer[i] = _currentRawValue;
    }
  }

  return true;
}

int ExoNaut_Knob::readRaw(void)
{
  if (_smoothingEnabled && _smoothingBuffer != NULL)
  {
    return calculateSmoothedValue();
  }
  else
  {
    _lastRawValue = _currentRawValue;
    _currentRawValue = analogRead(_knobPin);
    return _currentRawValue;
  }
}

int ExoNaut_Knob::readMapped(int minVal, int maxVal)
{
  int rawValue = readRaw();

  // Use provided range or stored range
  int min = (minVal == 0 && maxVal == 100) ? _minValue : minVal;
  int max = (minVal == 0 && maxVal == 100) ? _maxValue : maxVal;

  // Map raw value to the desired range
  return map(rawValue, KNOB_MIN_VALUE, KNOB_MAX_VALUE, min, max);
}

void ExoNaut_Knob::setRange(int minVal, int maxVal)
{
  _minValue = minVal;
  _maxValue = maxVal;
}

int ExoNaut_Knob::readPercent(void)
{
  int rawValue = readRaw();

  // Calculate percentage (0-100)
  return map(rawValue, KNOB_MIN_VALUE, KNOB_MAX_VALUE, 0, 100);
}

bool ExoNaut_Knob::hasChanged(int threshold)
{
  int rawValue = readRaw();
  int diff = abs(rawValue - _lastRawValue);

  return (diff > threshold);
}

void ExoNaut_Knob::setSmoothing(bool enable, int samples)
{
  // Avoid unnecessary changes
  if (_smoothingEnabled == enable && _smoothingSamples == samples)
  {
    return;
  }

  // Clean up old buffer if it exists
  if (_smoothingBuffer != NULL)
  {
    delete[] _smoothingBuffer;
    _smoothingBuffer = NULL;
  }

  _smoothingEnabled = enable;

  if (samples > 0)
  {
    _smoothingSamples = samples;
  }

  if (_smoothingEnabled)
  {
    // Allocate new buffer
    _smoothingBuffer = new int[_smoothingSamples];

    if (_smoothingBuffer == NULL)
    {
      // Allocation failed
      _smoothingEnabled = false;
      return;
    }

    // Initialize buffer
    _smoothingIndex = 0;
    _smoothingBufferFull = false;

    // Fill buffer with current value
    int currentReading = analogRead(_knobPin);
    for (int i = 0; i < _smoothingSamples; i++)
    {
      _smoothingBuffer[i] = currentReading;
    }
  }
}

void ExoNaut_Knob::update(void)
{
  // Read the current knob value
  _currentRawValue = analogRead(_knobPin);

  // Update the smoothing buffer if enabled
  if (_smoothingEnabled && _smoothingBuffer != NULL)
  {
    _smoothingBuffer[_smoothingIndex] = _currentRawValue;
    _smoothingIndex = (_smoothingIndex + 1) % _smoothingSamples;

    if (!_smoothingBufferFull && _smoothingIndex == 0)
    {
      _smoothingBufferFull = true;
    }
  }
}

int ExoNaut_Knob::calculateSmoothedValue(void)
{
  // If smoothing is disabled or buffer is not allocated, return current raw value
  if (!_smoothingEnabled || _smoothingBuffer == NULL)
  {
    return _currentRawValue;
  }

  // Calculate average of values in buffer
  long sum = 0;
  int count = _smoothingBufferFull ? _smoothingSamples : _smoothingIndex;

  for (int i = 0; i < count; i++)
  {
    sum += _smoothingBuffer[i];
  }

  return (count > 0) ? (sum / count) : _currentRawValue;
}

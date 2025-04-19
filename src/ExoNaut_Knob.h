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
 
 class ExoNaut_Knob
 {
 public:
   // Constructor
   ExoNaut_Knob();
 
   // Initialize the knob on a specific port (1, 2, 6, 8)
   void begin(uint8_t port);
 
   // Get knob position as a percentage (0–100)
   int getPercent();
 
   // Manually calibrate raw min and max ADC values
   void setCalibration(int rawMin, int rawMax);
 
 private:
   uint8_t _knobPin;
   int _rawMin;
   int _rawMax;
 };
 
 #endif

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
   _knobPin = 36; // Default fallback
   _rawMin = 0;   // Set to default full range
   _rawMax = 4095;
 }
 
 void ExoNaut_Knob::begin(uint8_t port)
 {
   switch (port)
   {
   case 1:
     _knobPin = 36;
     break;
   case 2:
     _knobPin = 32;
     break;
   case 6:
     _knobPin = 33;
     break;
   case 8:
     _knobPin = 26;
     break;
   default:
     _knobPin = 36;
     break;
   }
 
   pinMode(_knobPin, INPUT);
   analogReadResolution(12);
 }
 
 int ExoNaut_Knob::getPercent()
 {
   int raw = analogRead(_knobPin);
 
   // Protect against invalid calibration
   if (_rawMin >= _rawMax)
     return 0;
 
   long value = map((long)raw, (long)_rawMin, (long)_rawMax, 0, 100);
   return constrain(value, 0, 100);
 }
 
 void ExoNaut_Knob::setCalibration(int rawMin, int rawMax)
 {
   _rawMin = rawMin;
   _rawMax = rawMax;
 }
 
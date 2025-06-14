/*remove the last two lines from the library.properties file

precompiled=true
ldflags=-lSpaceTrek_ExoNaut

And replace with 

dot_a_linkage=true

Then run compile this sketch.  The replace the original content of the library file

Then....

*/
#include "ExoNaut.h"

// Sensor libraries
#include "ExoNaut_AICam.h"           // AI Camera for object/face detection
#include "ExoNaut_AICamLF.h"         // AI Camera Line Following extension
#include "ExoNaut_IMU.h"             // Accelerometer and Gyroscope (MPU6050)
#include "ExoNaut_TempHumid.h"       // Temperature and Humidity sensor (AHT20/21/25)
#include "ExoNaut_Ultrasonic.h"      // RGB Ultrasonic distance sensor
#include "ExoNaut_LineFollower.h"    // Traditional line follower sensor

// Input/Control libraries
#include "ExoNaut_Knob.h"            // Analog knob/potentiometer

// Output/Display libraries
#include "ExoNaut_7Segment.h"        // 7-segment display (TM1640)
#include "ExoNaut_DotMatrix.h"       // LED dot matrix display (TM1640)
#include "ExoNaut_RGB_LED.h"         // External RGB LED strips

// Audio library
#include "ExoNaut_MP3.h"             // MP3 player module


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
 * ExoNaut_KnobExample.ino
 * 
 * Example sketch for the ExoNaut knob module.
 * This sketch demonstrates using a potentiometer to control
 * the robot's motors and LEDs.
 * 
 * Created: March 3, 2025
 */

#include <ExoNaut.h>       // Main ExoNaut library
#include "ExoNaut_Knob.h"  // Knob module library

// Create instances
exonaut robot;                // Main robot instance
ExoNaut_Knob knob;            // Knob on default pin (36)

// Control variables
int motorSpeed = 0;           // Current motor speed (0-100)
int knobValue = 0;            // Raw knob reading (0-4095)
int mappedValue = 0;          // Mapped knob reading (0-100)

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000); // Give time for serial to connect
  
  Serial.println("\n\n");
  Serial.println("ExoNaut Knob Example");
  Serial.println("===================");
  
  // Initialize the robot
  Serial.println("Initializing robot...");
  robot.begin();
  
  // Visual indication that the program is starting
  Serial.println("Startup sequence...");
  for (int i = 0; i < 3; i++) {
    robot.setColorAll(0, 0, 50);  // Blue
    robot.show();
    delay(300);
    robot.setColorAll(0, 0, 0);   // Off
    robot.show();
    delay(300);
  }
  
  // Initialize the knob
  Serial.println("Initializing knob module...");
  knob.begin();
  
  // Configure knob
  Serial.println("Configuring knob...");
  knob.setSmoothing(true, 8);  // Enable smoothing with 8 samples
  knob.setRange(0, 100);       // Map to 0-100 range
  
  Serial.println("Setup complete!");
  Serial.println("Turn the knob to control motors and LEDs.");
  Serial.println();
}

void loop() {
  // Update knob state
  knob.update();
  
  // Read knob values
  knobValue = knob.readRaw();
  mappedValue = knob.readMapped();
  
  // Only update if there's a significant change
  if (knob.hasChanged(20)) {
    // Update motor speed based on knob position
    motorSpeed = mappedValue;
    robot.set_motor_speed(motorSpeed, motorSpeed);
    
    // Update LEDs based on knob position
    updateLEDs(mappedValue);
    
    // Print values to serial monitor
    Serial.print("Knob: ");
    Serial.print(knobValue);
    Serial.print("/4095 | Mapped: ");
    Serial.print(mappedValue);
    Serial.print("% | Motor: ");
    Serial.print(motorSpeed);
    Serial.println("%");
  }
  
  // Small delay to prevent overwhelming the serial output
  delay(50);
}

// Update LEDs based on knob position
void updateLEDs(int value) {
  // Map knob value (0-100) to number of LEDs to light (0-6)
  int ledsToLight = map(value, 0, 100, 0, NUM_PIXELS);
  
  // Calculate color based on knob position
  // Gradient from green (low) to yellow to red (high)
  int r = map(value, 0, 100, 0, 255);
  int g = map(value, 0, 100, 255, 0);
  int b = 0;
  
  // Set all LEDs
  for (int i = 0; i < NUM_PIXELS; i++) {
    if (i < ledsToLight) {
      // Lit LEDs use the calculated color
      robot.setColor(i, r/5, g/5, b);  // Divide by 5 to reduce brightness
    } else {
      // Unlit LEDs are dim blue
      robot.setColor(i, 0, 0, 10);
    }
  }
  
  // Show the updated colors
  robot.show();
}

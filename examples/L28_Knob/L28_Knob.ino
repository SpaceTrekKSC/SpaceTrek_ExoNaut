/*
 * L10_Knob_Control.ino
 *
 * Example sketch for the ExoNaut knob module.
 * This sketch demonstrates using a potentiometer to control
 * the robot's motors and LEDs.
 * 
 * The program reads the knob position and uses it to adjust
 * motor speed and LED colors in real-time with visual feedback.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * Commands:
 * exonaut robot;                         //This command creates the main robot instance
 *                                        //This is the object that handles motors and core functions
 *
 * ExoNaut_Knob knob;                     //This command creates a Knob object called 'knob'
 *                                        //This is the object that handles potentiometer input
 *
 * knob.begin(PORT);                      //This command initializes the knob with the specified port number
 *                                        //The port number determines which analog pin is used
 *
 * knob.setSmoothing(enable, samples);    //This command enables or disables value smoothing
 *                                        //Parameters: boolean to enable/disable, number of samples to average
 *
 * knob.setRange(minVal, maxVal);         //This command sets the default output range for knob readings
 *                                        //Maps the raw analog value (0-4095) to the specified range
 *
 * knob.update();                         //This command updates the current knob reading
 *                                        //Should be called regularly in the loop before reading values
 *
 * knob.readRaw();                        //This command reads the raw analog value from the knob (0-4095)
 *
 * knob.readMapped();                     //This command reads the knob value mapped to the set range
 *
 * knob.hasChanged(threshold);            //This command detects if the knob value has changed
 *                                        //Parameter: threshold value for minimum change to detect
 *
 * robot.set_motor_speed(left, right);    //This command sets both motor speeds
 *                                        //Parameters: left motor speed, right motor speed
 *
 * robot.setColor(index, r, g, b);        //This command sets the color of an individual LED
 *                                        //Parameters: LED index, red, green, blue values (0-255)
 *
 * robot.setColorAll(r, g, b);            //This command sets all LEDs to the same color
 *                                        //Parameters: red, green, blue values (0-255)
 *
 * robot.show();                          //This command updates the physical LED display
 */

#include <ExoNaut.h>       // Main ExoNaut library
#include "ExoNaut_Knob.h"  // Knob module library

// Create instances
exonaut robot;                // Main robot instance
ExoNaut_Knob knob;            // Knob instance

// Define which port the knob is connected to (1, 2, 6, or 8)
#define KNOB_PORT 2          // Connected to port 2 (pin 32)

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
  
  // Initialize the knob with specified port
  Serial.println("Initializing knob module on port " + String(KNOB_PORT) + "...");
  knob.begin(KNOB_PORT);  // Initialize knob with port number
  
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

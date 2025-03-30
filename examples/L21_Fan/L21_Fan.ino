/*
 * L05_Fan_Control.ino
 *
 * This example uses basic digital output to control a fan motor
 * instead of PWM which might not be compatible with all ESP32 versions.
 * 
 * Connect:
 * - M- to GPIO 33
 * - M+ to GPIO 25
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025

Commands:
exonaut robot;                              //This command creates an ExoNaut object called 'robot'
                                           //This is the object that handles basic robot functions

robot.begin();                              //This command initializes the robot systems
                                           //It is used once at the beginning of the program

robot.setColorAll(r, g, b);                 //This command sets all LEDs to the same RGB color
                                           //Parameters are red (0-255), green (0-255), blue (0-255)

robot.show();                               //This command updates the physical LED display with current color settings

pinMode(pin, mode);                         //Arduino function to configure a GPIO pin
                                           //Parameters: pin number, INPUT or OUTPUT mode

digitalWrite(pin, value);                   //Arduino function to set a digital pin HIGH or LOW
                                           //Parameters: pin number, HIGH (1) or LOW (0)

delay(ms);                                  //Arduino function to pause program execution
                                           //Parameter: time in millisecondsRetryClaude can make mistakes. Please double-check responses.
 */

#include "ExoNaut.h"

// Pin definitions
#define FAN_PIN_1      33  // M-
#define FAN_PIN_2      25  // M+

// Create instance
exonaut robot;

void setup() {
  Serial.begin(115200);
  Serial.println("Simple Fan Test");
  
  // Initialize robot
  robot.begin();
  
  // Set LEDs to blue
  robot.setColorAll(0, 0, 255);
  robot.show();
  
  // Configure fan pins
  pinMode(FAN_PIN_1, OUTPUT);
  pinMode(FAN_PIN_2, OUTPUT);
  
  // Start with fan off
  digitalWrite(FAN_PIN_1, LOW);
  digitalWrite(FAN_PIN_2, LOW);
  
  delay(2000);
}

void loop() {
  // Test forward direction
  Serial.println("FORWARD direction");
  robot.setColorAll(0, 255, 0);
  robot.show();
  
  digitalWrite(FAN_PIN_2, HIGH);  // M+ HIGH = forward
  digitalWrite(FAN_PIN_1, HIGH);  // M- HIGH = on
  
  delay(5000);
  
  // Turn off
  Serial.println("OFF");
  robot.setColorAll(255, 0, 0);
  robot.show();
  
  digitalWrite(FAN_PIN_1, LOW);   // M- LOW = off
  
  delay(2000);
  
  // Test reverse direction
  Serial.println("REVERSE direction");
  robot.setColorAll(255, 0, 255);
  robot.show();
  
  digitalWrite(FAN_PIN_2, LOW);   // M+ LOW = reverse
  digitalWrite(FAN_PIN_1, HIGH);  // M- HIGH = on
  
  delay(5000);
  
  // Turn off
  Serial.println("OFF");
  robot.setColorAll(0, 0, 255);
  robot.show();
  
  digitalWrite(FAN_PIN_1, LOW);   // M- LOW = off
  
  delay(2000);
}

/*
 * Simple Fan Test for ESP32
 * 
 * This example uses basic digital output to control a fan motor
 * instead of PWM which might not be compatible with all ESP32 versions.
 * 
 * Connect:
 * - M- to GPIO 33
 * - M+ to GPIO 25
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

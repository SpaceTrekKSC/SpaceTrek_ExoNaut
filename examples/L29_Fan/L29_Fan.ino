/*
 * L29_Fan_Control.ino
 *
 * This example uses basic digital output to control a fan motor
 * instead of PWM which might not be compatible with all ESP32 versions.
 * 
 * Connect the fan to port 6 of the CoreX controller 
 *  M- is on PIN 33
 *  M+ is on PIN 25
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
#define FAN_PIN_1      33   // M- controlls if the motor is on or off. LOW is on and HIGH is off
                            // the speed can be adjusted by sending a PWM signal to the M- pin

#define FAN_PIN_2      25   // M+ controlls the direction of the motor. 
                            // HIGH is forward (CW)
                            // LOW is reverse (CCW)

// Create instance
exonaut robot;

void setup() {
  // Configure motor controller (fan) pins
  pinMode(FAN_PIN_1, OUTPUT);
  pinMode(FAN_PIN_2, OUTPUT);

  Serial.begin(115200);                       //Start the serial port
  Serial.println("Simple Fan Test");          //Tell the user that this is a simple fan test

  robot.begin();                              //start the robot object
  
  // Set LEDs to blue
  robot.setColorAll(0, 0, 255);
  robot.show();
  
  // Start with fan off
  digitalWrite(FAN_PIN_1, LOW);
  digitalWrite(FAN_PIN_2, LOW);
  
  delay(2000);
}

void loop() {
  // Test forward direction
  digitalWrite(FAN_PIN_2, HIGH);  // M+ HIGH = forward (CW)
  digitalWrite(FAN_PIN_1, LOW);  // M- LOW = on
  Serial.println("FORWARD direction");
  robot.setColorAll(0, 255, 0);
  robot.show();
  delay(3000);
  
  // Turn off
  digitalWrite(FAN_PIN_1, HIGH);   // M- HIGH = off
  Serial.println("OFF");
  robot.setColorAll(255, 0, 0);
  robot.show();
  delay(3000);

  //ramp speed from off to full speed
  for(int i = 255; i >= 0; i--){
    analogWrite(FAN_PIN_1, i);
    robot.setColorAll(0, 0, (255 - i));
    robot.show();
    delay(30);
  }
  delay(1000);
  pinMode(FAN_PIN_1, OUTPUT);       // needed here to release the PIN from analog mode

  // Turn off
  digitalWrite(FAN_PIN_1, HIGH);   // M- HIGH = off
  Serial.println("OFF");
  robot.setColorAll(255, 0, 0);
  robot.show();
  delay(3000);
  
  // Test reverse direction
  digitalWrite(FAN_PIN_2, LOW);   // M+ LOW = reverse
  digitalWrite(FAN_PIN_1, HIGH);  // M- HIGH = on         (This is opposite from forward)
  Serial.println("REVERSE direction");
  robot.setColorAll(255, 0, 255);
  robot.show();  
  delay(3000);
  
  // Turn off
  digitalWrite(FAN_PIN_1, LOW);   // M- LOW = off         (this is opposite from forward)
  Serial.println("OFF");
  robot.setColorAll(255, 0, 0);
  robot.show();
  delay(3000);
}

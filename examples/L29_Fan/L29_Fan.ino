/*
 * L29_Fan_Control.ino
 *
 * This example uses basic digital output to control a fan motor
 * instead of PWM which might not be compatible with all ESP32 versions.
 * 
 * Connect the fan to port 6 of the CoreX controller 
 *  If connected to Port 6
 *  M- is on PIN 33
 *  M+ is on PIN 25
 *
 *  If connected to Port 8
 *  M- is on PIN 26
 *  M+ is on PIN 27
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * Commands:
 * exonaut robot;                              //Creates an ExoNaut object called 'robot'
 * robot.begin();                              //Initializes the robot systems
 * robot.setColorAll(r, g, b);                 //Sets all LEDs to an RGB color
 * robot.show();                               //Updates LED display
 * pinMode(pin, mode);                         //Sets pin mode (INPUT/OUTPUT)
 * digitalWrite(pin, value);                   //Turns pin HIGH or LOW
 * delay(ms);                                  //Waits for a specified time
 */

#include "ExoNaut.h" // Include ExoNaut library

#define FAN_PIN_1 33  // M- pin (controls fan power)
#define FAN_PIN_2 25  // M+ pin (controls fan direction)

exonaut robot; // Create robot object

void setup() {
  pinMode(FAN_PIN_1, OUTPUT); // Set fan power pin as output
  pinMode(FAN_PIN_2, OUTPUT); // Set fan direction pin as output

  Serial.begin(115200); // Start Serial Monitor
  Serial.println("Simple Fan Test"); // Print test name

  robot.begin(); // Start robot systems

  robot.setColorAll(0, 0, 255); // Set LEDs to blue
  robot.show(); // Update LEDs

  digitalWrite(FAN_PIN_1, LOW); // Set M- LOW (off)
  digitalWrite(FAN_PIN_2, LOW); // Set M+ LOW (no direction yet)

  delay(2000); // Wait 2 seconds
}

void loop() {
  digitalWrite(FAN_PIN_2, HIGH); // M+ HIGH = forward
  digitalWrite(FAN_PIN_1, LOW);  // M- LOW = fan on
  Serial.println("FORWARD direction");
  robot.setColorAll(0, 255, 0); // Green = forward
  robot.show();
  delay(3000); // Fan runs for 3 seconds

  digitalWrite(FAN_PIN_1, HIGH); // M- HIGH = off
  Serial.println("OFF");
  robot.setColorAll(255, 0, 0); // Red = off
  robot.show();
  delay(3000); // Wait 3 seconds

  for(int i = 255; i >= 0; i--) { // Ramp up speed
    analogWrite(FAN_PIN_1, i); // Gradually turn on fan
    robot.setColorAll(0, 0, (255 - i)); // Fade LED from blue to white
    robot.show();
    delay(30); // Slow ramp
  }

  delay(1000); // Hold at full speed
  pinMode(FAN_PIN_1, OUTPUT); // Reset pin to digital mode

  digitalWrite(FAN_PIN_1, HIGH); // Fan off
  Serial.println("OFF");
  robot.setColorAll(255, 0, 0); // Red = off
  robot.show();
  delay(3000);

  digitalWrite(FAN_PIN_2, LOW); // M+ LOW = reverse
  digitalWrite(FAN_PIN_1, HIGH); // M- HIGH = fan on (reverse)
  Serial.println("REVERSE direction");
  robot.setColorAll(255, 0, 255); // Purple = reverse
  robot.show();
  delay(3000);

  digitalWrite(FAN_PIN_1, LOW); // M- LOW = off (reverse logic)
  Serial.println("OFF");
  robot.setColorAll(255, 0, 0); // Red = off
  robot.show();
  delay(3000);
}

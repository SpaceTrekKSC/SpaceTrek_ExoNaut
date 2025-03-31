/*
 * ExoNaut Claw Control - Using ExoNaut Library Functions
 */

#include "ExoNaut.h"

// Create an instance of the exonaut class
exonaut robot;

// Constants for the servo control
#define SERVO_CLAW_ID 1         // ID of the servo controlling the claw
#define SERVO_CLAW_OPEN 0     // Servo position for open claw
#define SERVO_CLAW_CLOSED 500   // Servo position for closed claw
#define SERVO_MOVE_TIME 500     // Time in ms for servo to move
#define CLAW_TOGGLE_DELAY 3000  // Time between opening and closing in ms

// State variables
bool clawOpen = true;
unsigned long lastToggleTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("ExoNaut Claw Control - Using Native Library");
  
  // Initialize the robot
  robot.begin();
  
  // Initialize the bus servo system
  robot.beginBusServo();
  
  // Start with the claw open
  openClaw();
  
  Serial.println("Setup complete. Claw will now toggle between open and closed.");
}

void loop() {
  if (millis() - lastToggleTime >= CLAW_TOGGLE_DELAY) {
    if (clawOpen) {
      closeClaw();
      delay(1000);
    } else {
      openClaw();
      delay(1000);
    }
    lastToggleTime = millis();
  }
}

void openClaw() {
  Serial.println("Opening claw...");
  robot.bus_servo_set_pose(SERVO_CLAW_ID, SERVO_CLAW_OPEN, SERVO_MOVE_TIME);
  clawOpen = true;
}

void closeClaw() {
  Serial.println("Closing claw...");
  robot.bus_servo_set_pose(SERVO_CLAW_ID, SERVO_CLAW_CLOSED, SERVO_MOVE_TIME);
  clawOpen = false;
}

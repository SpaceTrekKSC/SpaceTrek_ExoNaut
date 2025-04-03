/*
 * L30_Servo_Claw.ino
 *
 * This example demonstrates how to control a robotic claw
 * attached to the ExoNaut robot using bus servos.
 * 
 * The program toggles the claw between open and closed positions
 * at regular intervals, illustrating basic servo control.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 31, 2025
 *
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles all robot functions
 *
 * robot.begin();                        //This command initializes the robot systems
 *
 * robot.beginBusServo();                //This command initializes the bus servo system
 *                                       //Must be called before using any servo functions
 *
 * robot.bus_servo_set_pose(id, pos, time); //This command sets a servo to a specific position
 *                                          //Parameters: servo ID, position value, movement time in ms
 *                                          //Position value range depends on the specific servo
 *                                          //Movement time controls how fast the servo moves
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

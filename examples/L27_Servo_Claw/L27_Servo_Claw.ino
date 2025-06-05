#include "ExoNaut.h" // Include the main ExoNaut robot library

exonaut robot; // Create the robot object so we can control it

#define SERVO_CLAW_ID 1         // ID number for the claw's servo (usually 1)
#define SERVO_CLAW_OPEN 0       // Position for the servo when the claw is open
#define SERVO_CLAW_CLOSED 500   // Position for the servo when the claw is closed
#define SERVO_MOVE_TIME 500     // Time (in ms) for the servo to reach the position
#define CLAW_TOGGLE_DELAY 3000  // How long to wait before switching the claw (in ms)

bool clawOpen = true;           // Keep track of whether the claw is currently open
unsigned long lastToggleTime = 0; // Stores the last time we switched the claw

void setup() {
  Serial.begin(115200); // Start the Serial Monitor so we can see messages
  Serial.println("ExoNaut Claw Control - Using Native Library");

  robot.begin();         // Turn on the robot's systems
  robot.beginBusServo(); // Start the servo system

  openClaw(); // Start by opening the claw

  Serial.println("Setup complete. Claw will now toggle between open and closed.");
}

void loop() {
  // Check if it's been long enough to toggle the claw again
  if (millis() - lastToggleTime >= CLAW_TOGGLE_DELAY) {
    if (clawOpen) {
      closeClaw();     // If it was open, close it
      delay(1000);     // Wait a bit so we can see it move
    } else {
      openClaw();      // If it was closed, open it
      delay(1000);     // Wait a bit so we can see it move
    }
    lastToggleTime = millis(); // Save the time of this toggle
  }
}

void openClaw() {
  Serial.println("Opening claw..."); // Print a message to the Serial Monitor
  robot.bus_servo_set_pose(SERVO_CLAW_ID, SERVO_CLAW_OPEN, SERVO_MOVE_TIME); // Tell the servo to open
  clawOpen = true; // Remember that the claw is now open
}

void closeClaw() {
  Serial.println("Closing claw..."); // Print a message to the Serial Monitor
  robot.bus_servo_set_pose(SERVO_CLAW_ID, SERVO_CLAW_CLOSED, SERVO_MOVE_TIME); // Tell the servo to close
  clawOpen = false; // Remember that the claw is now closed
}

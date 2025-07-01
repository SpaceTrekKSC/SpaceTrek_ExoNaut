// Include the main robot library
#include <ExoNaut.h>
// Include the motion sensor (IMU) library
#include <ExoNaut_IMU.h>

// Create a robot object to control lights and motors
exonaut robot;
// Create a motion sensor object to read tilt data
ExoNautIMU motion;

void setup() {
  // Start the Serial Monitor (used for printing messages)
  Serial.begin(115200);

  // Wait half a second to let power settle
  delay(500);
  Serial.println("Starting ExoNaut...");

  // Start the robot hardware (motors, lights, etc.)
  robot.begin();
  Serial.println("Robot hardware initialized.");

  // Wait a bit so the sensor has time to turn on
  delay(250);
  // Start the motion sensor
  motion.start();
  Serial.println("IMU started.");

  // Show we are calibrating the sensor
  Serial.println("Calibrating IMU...");
  robot.setColorAll(0, 0, 255); // Turn lights blue

  // Calibrate the IMU (keep robot flat and still!)
  motion.calibrate();
  delay(500);

  // Show calibration is finished
  robot.setColorAll(0, 255, 0); // Turn lights green
  delay(1000);
  Serial.println("Calibration complete. Setup finished.");
}

void loop() {
  // --- Balancing Section ---

  // Read the current tilt angles from the sensor
  float pitch = motion.getPitchAngle(); // Front/back tilt
  float roll = motion.getRollAngle();   // Side-to-side tilt

  // If the robot is tipping forward
  if (pitch > 15.0) {
    robot.set_motor_speed(-40, -40); // Drive backward to fix
  }
  // If the robot is tipping backward
  else if (pitch < -15.0) {
    robot.set_motor_speed(40, 40); // Drive forward to fix
  }
  // If the robot is leaning right
  else if (roll > 15.0) {
    robot.set_motor_speed(40, -40); // Turn left to balance
  }
  // If the robot is leaning left
  else if (roll < -15.0) {
    robot.set_motor_speed(-40, 40); // Turn right to balance
  }
  // If the robot is mostly flat
  else {
    robot.stop_motor(0); // Stop moving
  }

  // Print out tilt angles to the Serial Monitor
  Serial.print("Pitch: ");
  Serial.println(pitch);
  Serial.print("Roll: ");
  Serial.println(roll);
  Serial.print(" "); // Blank line for spacing

  // Wait 20 milliseconds before checking again
  delay(20);
}

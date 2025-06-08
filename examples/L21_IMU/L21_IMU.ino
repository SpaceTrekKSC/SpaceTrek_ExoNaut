/*
 * L31_IMU.ino
 * 
 * A fun demo that shows off all the cool things the ExoNaut IMU can do!
 * It prints angles, motion, and simple answers like "Is it shaking?"
 * and uses lights to give quick feedback.
 * 
 * Must be plugged into port 3, 4, 5, or 9.
 * 
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: June 7, 2025
 *
 * Commands:
 * imu.start();              // Turn on the IMU
 * imu.calibrate();          // Calibrate gyro (must be still!)
 * imu.resetDirection();     // Set the new "straight ahead" direction
 *
 * Raw data:
 * imu.getPitchAngle(), imu.getRollAngle(), imu.getYawAngle()
 * imu.getPitchSpeed(), imu.getRollSpeed(), imu.getTurnSpeed()
 * imu.getForceX(), imu.getForceY(), imu.getForceZ()
 *
 * Easy answers:
 * imu.isLevel(), imu.isShaking(), imu.isUpsideDown(), etc.
 * imu.getNoseDirection(), imu.getSidePosition(), imu.getTurnDirection()
 */

#include <ExoNaut.h>         // Load the ExoNaut robot library
#include <ExoNaut_IMU.h>     // Load the IMU (sensor) library

exonaut robot;               // Create the robot object (so we can use motors/lights)
ExoNautIMU imu;              // Create the IMU object (so we can read motion and angles)

void setup() {
  Serial.begin(115200);     // Start the Serial Monitor so we can see messages
  robot.begin();            // Turn on the robot systems (motors, lights, etc.)

  if (imu.start()) {        // Try to start the IMU sensor
    Serial.println("IMU ready! Press 'c' to calibrate, 'r' to reset direction.");
  } else {
    Serial.println("IMU not detected! Must be plugged into Port 3, 4, 5, or 9.");
  }
}

void loop() {
  // === Check for User Input from Serial Monitor ===
  if (Serial.available()) {       // If a key was typed in the Serial Monitor
    char c = Serial.read();       // Read the typed key
    if (c == 'c') {               // If it was 'c', start calibration
      imu.calibrate();           // This tells the IMU what "still" looks like
    } else if (c == 'r') {        // If it was 'r', reset the direction
      imu.resetDirection();      // This makes the current facing direction = "straight"
    }
  }

  // === Read Angles ===
  float pitch = imu.getPitchAngle();  // Tilt forward/backward
  float roll = imu.getRollAngle();    // Tilt left/right
  float yaw = imu.getYawAngle();      // Compass direction
  float turn = imu.getTurnAngle();    // How far we turned from the start

  // === Read Motion Speeds ===
  float pitchSpeed = imu.getPitchSpeed();  // Speed of pitch rotation
  float rollSpeed  = imu.getRollSpeed();   // Speed of roll rotation
  float turnSpeed  = imu.getTurnSpeed();   // Speed of turning left/right

  // === Read Forces ===
  float forceX = imu.getForceX();     // Force side to side
  float forceY = imu.getForceY();     // Force forward/backward
  float forceZ = imu.getForceZ();     // Force up/down (gravity)

  // === Read Interpreted Answers ===
  String nose = imu.getNoseDirection();     // "up", "down", or "level"
  String side = imu.getSidePosition();      // "left side", "right side", or "flat"
  String turnDir = imu.getTurnDirection();  // "left", "right", or "straight"

  // === Print All Info to Serial Monitor ===
  Serial.println("=== ExoNaut IMU Status ===");

  Serial.print("Pitch: ");
  Serial.print(pitch);                 // Print pitch angle
  Serial.print("° (");
  Serial.print(nose);                  // Print nose direction (word)
  Serial.println(")");

  Serial.print("Roll : ");
  Serial.print(roll);                  // Print roll angle
  Serial.print("° (");
  Serial.print(side);                  // Print side direction (word)
  Serial.println(")");

  Serial.print("Yaw  : ");
  Serial.print(yaw);                   // Print yaw (compass) angle
  Serial.print("° (");
  Serial.print(turnDir);               // Print turn direction (word)
  Serial.println(")");

  Serial.print("Turn : ");
  Serial.print(turn);                  // Print how much we turned from start
  Serial.println("° from start");

  Serial.print("Motion Speeds (°/s): Pitch=");
  Serial.print(pitchSpeed);           // Print speed of pitch rotation
  Serial.print(" Roll=");
  Serial.print(rollSpeed);            // Print speed of roll
  Serial.print(" Turn=");
  Serial.println(turnSpeed);          // Print speed of turn

  Serial.print("Forces (g): X=");
  Serial.print(forceX);               // Force left/right
  Serial.print(" Y=");
  Serial.print(forceY);               // Force forward/back
  Serial.print(" Z=");
  Serial.println(forceZ);             // Force up/down

  // === Print Yes/No Answers ===
  if (imu.isMoving()) {               // Is the robot rotating?
    Serial.print("Moving? Yes");
  } else {
    Serial.print("Moving? No");
  }

  if (imu.isShaking()) {              // Is the robot vibrating?
    Serial.print(" | Shaking? Yes");
  } else {
    Serial.print(" | Shaking? No");
  }

  if (imu.isUpsideDown()) {           // Is the robot flipped over?
    Serial.println(" | Upside Down? Yes");
  } else {
    Serial.println(" | Upside Down? No");
  }

  // === LED Feedback Based on IMU State ===
  if (imu.isUpsideDown()) {
    robot.setColorAll(255, 0, 255);     // Purple = Upside down
  } else if (imu.isShaking()) {
    robot.setColorAll(255, 165, 0);     // Orange = Shaking
  } else if (imu.isMoving()) {
    robot.setColorAll(0, 255, 255);     // Cyan = Moving
  } else if (imu.isLevel()) {
    robot.setColorAll(0, 255, 0);       // Green = Level and calm
  } else {
    robot.setColorAll(255, 255, 0);     // Yellow = Tilted
  }

  robot.show();                         // Show the LED color update
  delay(400);                           // Wait a little before repeating everything
}

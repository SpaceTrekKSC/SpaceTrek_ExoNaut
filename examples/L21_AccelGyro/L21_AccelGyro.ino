/*
 * L06_AccelGyro_Monitor.ino
 *
 * This example creates an interactive serial monitor interface
 * to display and control the ExoNaut's MPU6050 accelerometer
 * and gyroscope sensor.
 * 
 * The program displays accelerometer, gyroscope, and orientation
 * data and allows calibration through a simple menu system.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * Commands:
 * ExoNautAccelGyro robot;                     //This command creates an AccelGyro object called 'robot'
 *                                            //This is the object that handles the MPU6050 sensor
 *
 * robot.beginIMU();                           //This command initializes the MPU6050 sensor
 *                                            //Returns true if successful, false if initialization failed
 *
 * robot.calibrateIMU();                       //This command performs gyroscope calibration
 *                                            //The robot should remain still during calibration
 *
 * robot.updateIMU();                          //This command reads the latest sensor data
 *                                            //Should be called regularly in the loop
 *
 * robot.getAcceleration(&ax, &ay, &az);       //This command retrieves the current acceleration values
 *                                            //Parameters are pointers to float variables for x, y, z axes
 *
 * robot.getRotation(&gx, &gy, &gz);           //This command retrieves the current rotation rate values
 *                                            //Parameters are pointers to float variables for x, y, z axes
 *
 * robot.getOrientation(&pitch, &roll, &yaw);  //This command retrieves the current orientation angles
 *                                            //Parameters are pointers to float variables for pitch, roll, yaw
 */

#include "ExoNaut.h"
#include "ExoNaut_AccelGyro.h"

ExoNautAccelGyro robot;
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 200;

// State machine variables
enum DisplayMode {
  MENU,           // Show menu
  ACCEL_GYRO,     // Show accelerometer and gyroscope readings
  ORIENTATION     // Show orientation readings
};

DisplayMode currentMode = MENU;

void setup() {
  Serial.begin(115200);
  Serial.println("ExoNaut MPU6050 Interactive Monitor");
  
  // Initialize robot
  robot.begin();
  
  // Initialize IMU
  if (robot.beginIMU()) {
    Serial.println("MPU6050 initialized successfully");
  } else {
    Serial.println("Failed to initialize MPU6050");
    while (1); // Halt if initialization failed
  }
  
  // Calibrate the gyroscope
  Serial.println("Calibrating gyroscope... keep the robot still!");
  robot.calibrateIMU();
  Serial.println("Calibration complete!");
  
  showMenu();
}

void loop() {
  // Update IMU data regardless of mode
  robot.updateIMU();
  
  // Check for commands
  if (Serial.available() > 0) {
    char command = Serial.read();
    processCommand(command);
    
    // Clear any remaining characters
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
  
  // Handle continuous display modes
  unsigned long currentTime = millis();
  if (currentTime - lastPrintTime >= printInterval) {
    lastPrintTime = currentTime;
    
    switch (currentMode) {
      case ACCEL_GYRO:
        displayAccelGyro();
        break;
        
      case ORIENTATION:
        displayOrientation();
        break;
        
      case MENU:
        // Nothing to display continuously in menu mode
        break;
    }
  }
  
  // Small delay to prevent CPU hogging
  delay(10);
}

void showMenu() {
  Serial.println("\n===== ExoNaut MPU6050 Monitor =====");
  Serial.println("  g - Continuous accelerometer & gyroscope readings");
  Serial.println("  o - Continuous orientation readings");
  Serial.println("  c - Calibrate gyroscope");
  Serial.println("  x - Return to this menu (when in continuous mode)");
  Serial.println("=====================================");
}

void processCommand(char cmd) {
  switch (cmd) {
    case 'g':
      currentMode = ACCEL_GYRO;
      Serial.println("\nStarting continuous accelerometer & gyroscope readings...");
      Serial.println("(Press 'x' to return to menu)");
      break;
      
    case 'o':
      currentMode = ORIENTATION;
      Serial.println("\nStarting continuous orientation readings...");
      Serial.println("(Press 'x' to return to menu)");
      break;
      
    case 'c':
      Serial.println("\nCalibrating gyroscope... keep the robot still!");
      robot.calibrateIMU();
      Serial.println("Calibration complete!");
      break;
      
    case 'x':
      if (currentMode != MENU) {
        currentMode = MENU;
        showMenu();
      }
      break;
      
    case '\n':
    case '\r':
      // Ignore newline characters
      break;
      
    default:
      if (currentMode == MENU) {
        Serial.println("Unknown command. Please select a valid option.");
      }
      break;
  }
}

void displayAccelGyro() {
  // Get accelerometer values
  float ax, ay, az;
  robot.getAcceleration(&ax, &ay, &az);
  
  // Get gyroscope values
  float gx, gy, gz;
  robot.getRotation(&gx, &gy, &gz);
  
  // Print values in a clean format
  Serial.print("Accel(g): ");
  Serial.print(ax, 2); Serial.print(", ");
  Serial.print(ay, 2); Serial.print(", ");
  Serial.print(az, 2);
  
  Serial.print(" | Gyro(deg/s): ");
  Serial.print(gx, 2); Serial.print(", ");
  Serial.print(gy, 2); Serial.print(", ");
  Serial.print(gz, 2);
  
  Serial.println();
}

void displayOrientation() {
  // Get orientation values
  float pitch, roll, yaw;
  robot.getOrientation(&pitch, &roll, &yaw);
  
  // Print orientation in a clean format
  Serial.print("Orientation(deg): Pitch=");
  Serial.print(pitch, 2);
  Serial.print(", Roll=");
  Serial.print(roll, 2);
  Serial.print(", Yaw=");
  Serial.print(yaw, 2);
  
  Serial.println();
}

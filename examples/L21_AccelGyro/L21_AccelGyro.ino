/*
 * L21_IMU.ino
 *
 * This example creates an interactive serial monitor interface
 * to display and control the ExoNaut's MPU6050 accelerometer
 * and gyroscope sensor using the ExoNautIMU class.
 * 
 * The program displays accelerometer, gyroscope, and orientation
 * data, and allows gyroscope calibration and mode switching via
 * the serial monitor.
 *
 * Must be plugged into ports 9, 5, 4, 3
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * Commands:
 * ExoNautIMU imu;                              //Creates an IMU object called 'imu'
 *                                            //Handles reading and processing sensor data
 *
 * imu.start();                                 //Initializes the MPU6050 sensor
 *                                            //Returns true if successful, false otherwise
 *
 * imu.calibrate();                             //Calibrates the gyroscope
 *                                            //Robot must remain still during this process
 *
 * imu.update();                                //Reads new data from the IMU
 *                                            //Should be called frequently in loop()
 *
 * imu.getAcceleration();                       //imu.getAccelX(), getAccelY(), getAccelZ()
 *                                            //Returns acceleration in g for each axis
 *
 * imu.getRotation();                           //imu.getGyroX(), getGyroY(), getGyroZ()
 *                                            //Returns angular velocity in deg/sec
 *
 * imu.getOrientation();                        //imu.getPitch(), getRoll(), getYaw()
 *                                            //Returns current orientation in degrees
 *
 * Serial commands:
 *   'c' - Calibrate the gyroscope
 *   'm' - Switch mode (0: Orientation, 1: Accel, 2: Gyro)
 */

 #include <Wire.h>
 #include "ExoNaut_IMU.h"
 
 ExoNautIMU imu;
 int mode = 0;
 
 void setup() {
   Serial.begin(115200);
   if (imu.start()) {
     Serial.println("IMU started successfully!");
     Serial.println("Send 'c' to calibrate, 'm' to switch mode (0: orientation, 1: accel, 2: gyro)");
   } else {
     Serial.println("Failed to start IMU.");
   }
 }
 
 void loop() {
   if (Serial.available()) {
     char command = Serial.read();
     if (command == 'c') {
       Serial.println("Calibrating gyroscope, keep the board still...");
       imu.calibrate();
       Serial.println("Calibration complete!");
     } else if (command == 'm') {
       mode = (mode + 1) % 3;
       Serial.print("Switched to mode: ");
       Serial.println(mode);
     }
   }
 
   imu.update();
 
   if (mode == 0) {
     Serial.print("Pitch: "); Serial.print(imu.getPitch());
     Serial.print(" Roll: "); Serial.print(imu.getRoll());
     Serial.print(" Yaw: "); Serial.println(imu.getYaw());
   } else if (mode == 1) {
     Serial.print("Accel X: "); Serial.print(imu.getAccelX());
     Serial.print(" Y: "); Serial.print(imu.getAccelY());
     Serial.print(" Z: "); Serial.println(imu.getAccelZ());
   } else if (mode == 2) {
     Serial.print("Gyro X: "); Serial.print(imu.getGyroX());
     Serial.print(" Y: "); Serial.print(imu.getGyroY());
     Serial.print(" Z: "); Serial.println(imu.getGyroZ());
   }
 
   delay(500);
 }
 
 
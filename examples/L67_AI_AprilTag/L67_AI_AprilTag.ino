/*
 * L67_AI_AprilTag.ino
 *
 * This sketch demonstrates how to use the AI camera's AprilTag detection 
 * functionality on the Space Trek ExoNaut Robot.
 * 
 * The program switches the camera into AprilTag detection mode, checks 
 * for detected tags, and prints out detailed tag information when found.
 *
 * The camera module is an i2c device. It must be plugged
 * into port 3, 4, 5, or 9. It will not work in any other ports.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2025
 *
 * Commands:
 * ExoNaut_AICam camera;                 //This command creates an AI Camera object
 *                                       //Provides access to the camera's computer vision functions
 *
 * camera.begin();                       //This command initializes the camera over I2C
 *
 * camera.changeFunc(APPLICATION_APRILTAG); //This command switches the camera into AprilTag detection mode
 *                                         //Returns true if successful
 *
 * camera.tagFound();                    //This command checks if an AprilTag is currently detected
 *                                       //Returns true if a tag is visible
 *
 * camera.printTagInfo();                 //This command prints detailed information about the detected tag
 *                                       //Outputs position, orientation, and ID data to Serial
 */

#include <Wire.h>
#include "ExoNaut_AICam.h"

ExoNaut_AICam camera;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();
  camera.begin();

  if (camera.changeFunc(APPLICATION_APRILTAG)) {
    Serial.println("Camera ready!");
  } else {
    Serial.println("Error switching modes");
  }

  delay(1000);
}

void loop() {
  if (camera.tagFound()) {
    Serial.println("AprilTag detected!");
    camera.printTagInfo();
  } else {
    Serial.println("No tags yet.");
  }
  
  delay(2000);
}
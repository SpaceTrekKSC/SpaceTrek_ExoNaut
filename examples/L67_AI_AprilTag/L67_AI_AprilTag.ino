/**************************************************
 * L67_AI_AprilTag.ino
 *
 * This sketch demonstrates how to use the AI camera's AprilTag detection 
 * functionality on the Space Trek ExoNaut Robot.
 * * The program switches the camera into AprilTag detection mode, checks 
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
 * ExoNaut_AICam camera;                //This command creates an AI Camera object
 * //Provides access to the camera's computer vision functions
 *
 * camera.begin();                      //This command initializes the camera over I2C
 *
 * camera.changeFunc(APPLICATION_APRILTAG); //This command switches the camera into AprilTag detection mode
 * //Returns true if successful
 *
 * camera.anyTagDetected();             //This command checks if an AprilTag is currently detected (updated from tagFound())
 * //Returns true if a tag is visible
 *
 * camera.printAllTagDetails();         //This command prints detailed information about all detected tags (updated from printTagInfo())
 * //Outputs position, orientation, and ID data to Serial
 **************************************************/

// These lines add the special instructions (libraries) our robot needs.
#include <Wire.h>           // For I2C communication, which the camera uses.
#include "ExoNaut_AICam.h"  // For the AI Camera's smart vision.

// Let's name our camera so we can command it.
ExoNaut_AICam camera;     // This is the robot's AI Camera.

// The setup() runs once when the robot (or just the microcontroller with the camera) turns on.
void setup() {
  Serial.begin(115200);   // Lets the robot send messages to the computer.
  while (!Serial);        // Wait until the Serial Monitor (computer screen) is open and ready.

  Wire.begin();           // Starts the I2C communication system.
  camera.begin();         // Wakes up the AI Camera.

  // Tell the camera to start looking for AprilTags.
  // AprilTags are like special square barcodes.
  if (camera.changeFunc(APPLICATION_APRILTAG)) {
    Serial.println("Camera ready to find AprilTags!"); // Message to computer: Camera is ready!
  } else {
    Serial.println("Error switching camera to AprilTag mode."); // Message: Something went wrong.
    // You might want to add 'while(1);' here to stop if it fails, like in other examples.
  }

  delay(1000); // Wait a second for the camera to get fully ready.
}

void loop() {
  camera.updateResult(); // Ask the camera to check what it sees right now.

  // Ask the camera if it sees any AprilTags.
  // camera.anyTagDetected() will be true if at least one tag is visible.
  if (camera.anyTagDetected()) { 
    Serial.println("AprilTag detected!"); // Yay, it found one (or more)!
    // Ask the camera to print all the details about the tags it sees.
    // This will show things like the tag's ID number, where it is in the camera's view, etc.
    camera.printAllTagDetails(); 
  } else {
    Serial.println("Looking for AprilTags... none found yet."); // Still looking, no tags seen right now.
  }
  
  delay(2000); // Wait for 2 seconds before checking for AprilTags again.
}

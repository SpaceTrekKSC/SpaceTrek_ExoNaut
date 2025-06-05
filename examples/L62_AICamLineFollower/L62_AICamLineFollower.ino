/*
 * L62_AICamLineFollower.ino
 *
 * This sketch demonstrates how to easily use the AI camera line following 
 * functionality on the Space Trek ExoNaut Robot using the simplified functions.
 * 
 * The program automatically detects lines with the camera and controls the robot's
 * movement to follow the line, including slowing down for sharp turns and 
 * pivoting to recover if the line is lost.
 *
 * The camera module is an i2c device. It must be plugged
 * into port 3, 4, 5, or 9. It will not work in any other ports.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2025
 *
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                 //This command creates an AI Camera object
 *                                       //Provides access to the camera's computer vision functions
 *
 * ExoNaut_AICamLF lineFollower;         //This command creates a specialized line follower object
 *                                       //Provides simplified easy-to-use line following commands
 *
 * robot.begin();                        //This command initializes the robot systems
 *
 * lineFollower.begin(&robot, &camera);  //This command initializes the line follower with robot and camera references
 *                                       //Returns true if initialization was successful
 *
 * lineFollower.setBaseSpeed(speed);     //This command sets the robot's base driving speed
 *                                       //Speed value from 0 to 100
 *
 * lineFollower.simpleFollowLine();      //This command automatically handles line following
 *                                       //It follows the line, slows down for sharp turns, and pivots if the line is lost
 */

// These lines add the special instructions (libraries) our robot needs.
#include "ExoNaut.h"          // For basic robot controls like moving.
#include "ExoNaut_AICam.h"      // For the AI Camera's smart vision.
#include "ExoNaut_AICamLF.h"    // For easy line following with the AI Camera.

// Let's name our robot and its parts so we can talk to them in code.
exonaut robot;                  // This is our main ExoNaut robot.
ExoNaut_AICam camera;           // This is the robot's AI Camera.
ExoNaut_AICamLF lineFollower;   // This is a helper that makes line following simple.

// The setup() runs once when the robot turns on.
void setup() {
  Serial.begin(115200); // Lets the robot send messages to the computer.
  robot.begin();        // Wakes up the robot.
  delay(500);           // Waits a little bit for things to get ready.

  // Wake up the line follower. It needs to know about our robot and camera.
  if (!lineFollower.begin(&robot, &camera)) {
    // If this message appears, the camera might not be plugged in right.
    Serial.println("Camera not connected or line follower failed to initialize!");
    while (1); // Stop the program if the camera isn't working.
  }

  // Set how fast the robot should try to go (0-100).
  lineFollower.setBaseSpeed(40); 
}

// The loop() runs over and over, forever!
void loop() {
  // This one command tells the robot to follow the line all by itself!
  // It will look for the line, drive along it, and try to fix mistakes.
  lineFollower.simpleFollowLine(); 
}

/*
 * L65_AI_FaceDetect.ino
 *
 * Example sketch for the ExoNaut AI Camera module.
 * This sketch demonstrates how to use Face Detection mode
 * to find faces and display their position and ID information.
 * 
 * The AI camera is connected via I2C and must be plugged into
 * port 3, 4, 5, or 9 on the robot.
 * 
 * Detected faces are printed in a neatly formatted table,
 * showing their coordinates and assigned IDs.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 27, 2025
 *
 * Commands:
 * exonaut robot;                  // Creates the main robot object
 * ExoNaut_AICam camera;            // Creates the AI camera object
 * robot.begin();                   // Initializes the robot systems
 * camera.begin();                  // Initializes the AI camera
 * camera.changeFunc(APPLICATION_FACEDETECT);  // Switches camera to Face Detection mode
 * camera.printFaceTable();         // Prints a table of detected faces with position and ID
 */

 #include "ExoNaut.h"
 #include "ExoNaut_AICam.h"
 
 // Create robot and camera objects
 exonaut robot;
 ExoNaut_AICam camera;
 
 void setup() {
   Serial.begin(115200);
   robot.begin();
   delay(500);
 
   camera.begin();
   camera.setLed(false); // Turn camera light ON
 
   // Switch camera into Face Detection mode
   if (camera.changeFunc(APPLICATION_FACEDETECT)) {
     Serial.println("Camera ready for Face Detection!");
   } else {
     Serial.println("Error: Could not set Face Detection mode.");
     while (1); // Stop if cannot set mode
   }
 
   delay(500);
 }
 
 void loop() {
   // Print a nice table of detected faces
   camera.printFaceTable();
 
 
   delay(1000); // Update every second
 }
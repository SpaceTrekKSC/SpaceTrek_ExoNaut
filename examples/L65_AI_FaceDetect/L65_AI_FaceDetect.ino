/**************************************************
 * L65_AI_FaceDetect.ino
 *
 * Example sketch for the ExoNaut AI Camera module.
 * This sketch demonstrates how to use Face Detection mode
 * to find faces and display their position and ID information.
 * * The AI camera is connected via I2C and must be plugged into
 * port 3, 4, 5, or 9 on the robot.
 * * Detected faces are printed in a neatly formatted table,
 * showing their coordinates and assigned IDs.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 27, 2025
 *
 * Commands:
 * exonaut robot;                       // Creates the main robot object
 * ExoNaut_AICam camera;                // Creates the AI camera object
 * robot.begin();                       // Initializes the robot systems
 * camera.begin();                      // Initializes the AI camera
 * camera.changeFunc(APPLICATION_FACEDETECT);  // Switches camera to Face Detection mode
 * camera.printFaceTable();             // Prints a table of detected faces with position and ID
 **************************************************/

 // These lines add the special instructions (libraries) our robot needs.
 #include "ExoNaut.h"        // For basic robot controls.
 #include "ExoNaut_AICam.h"    // For the AI Camera's smart vision.
 
 // Let's name our robot and camera so we can command them.
 exonaut robot;              // This is our main ExoNaut robot.
 ExoNaut_AICam camera;       // This is the robot's AI Camera.
 
 // The setup() runs once when the robot turns on.
 void setup() {
   Serial.begin(115200);   // Lets the robot send messages to the computer.
   robot.begin();            // Wakes up the main robot.
   delay(500);               // Waits a little bit for things to get ready.
 
   camera.begin();           // Wakes up the AI Camera.
   camera.setLed(true);      // Turn the camera's little light ON (true means ON, false means OFF).
 
   // Tell the camera to start looking for faces.
   if (camera.changeFunc(APPLICATION_FACEDETECT)) {
     Serial.println("Camera ready for Face Detection!"); // Message to computer: Camera is ready!
   } else {
     Serial.println("Error: Could not set Face Detection mode."); // Message: Something went wrong.
     while (1); // Stop the program if the camera couldn't switch to face detection mode.
   }
 
   delay(500); // Wait a little bit for the camera to get ready.
 }
 
 // The loop() runs over and over, forever!
 void loop() {
   // Ask the camera to print a list (a table) of all the faces it sees.
   // This list will show up on your computer screen in the Serial Monitor.
   camera.printFaceTable(); 
 
   delay(1000); // Wait for 1 second before checking for faces again.
 }

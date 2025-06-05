/**************************************************
 * L66_AI_Image_Classification.ino
 * A program to identify objects using AI camera and indicate confidence with LED colors
 *
 * The camera module is an i2c device.  It must be plugged
 * into port 3, 4, 5 or 9.  It will not work in any other ports.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2, 2025
 * * Program Flow:
 * - Camera captures images and classifies objects in view
 * - Classification results displayed on serial monitor
 * - RGB LEDs indicate confidence level:
 * - Green: High confidence (>80%)
 * - Yellow: Medium confidence (50-80%)
 * - Red: Low confidence (<50%)
 * * Commands:
 * exonaut robot;                       //This command creates the main robot instance
 * //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                //This command creates an AI Camera object
 * //Handles vision-based detection and recognition
 *
 * robot.begin();                       //This command initializes the robot systems
 *
 * camera.begin();                      //This command initializes the camera module
 *
 * camera.firmwareVersion(version);     //This command retrieves the camera firmware version
 * //Stores the version string in the provided variable
 *
 * camera.changeFunc(mode);             //This command changes the camera's operating mode
 * //Set to APPLICATION_CLASSIFICATION for this program
 *
 * camera.updateResult();               //This command updates recognition results from the camera
 *
 * camera.classIdOfMaxProb();           //This command returns the class ID with highest probability
 *
 * camera.classMaxProb();               //This command returns the confidence level of top classification
 *
 * camera.classProbOfId(id);            //This command returns the probability for a specific class ID
 *
 * robot.setColorAll(r, g, b);          //This command sets the color of all onboard NeoPixel LEDs
 * //Parameters are RGB values (0-255)
 *
 * robot.show();                        //This command updates the LED display with new colors
 **************************************************/

// These lines add the special instructions (libraries) our robot needs.
#include "ExoNaut.h"        // For basic robot controls like lights.
#include "ExoNaut_AICam.h"    // For the AI Camera's smart vision.

// Let's name our robot and camera so we can command them.
exonaut robot;              // This is our main ExoNaut robot.
ExoNaut_AICam camera;       // This is the robot's AI Camera.

// This is a list of names for objects the camera might recognize.
// The camera gives us an ID number, and we use this list to find the name.
const char* objectNames[] = {
  "Unknown",              // ID 0 - If the camera doesn't know.
  "Cigarette Ends",       // ID 1
  "Oral Liquid Bottle",   // ID 2
  "Marker",               // ID 3
  "Storage Battery",      // ID 4
  "Toothbrush",           // ID 5
  "Plastic Bottle",       // ID 6
  "Umbrella",             // ID 7
  "Banana Peel",          // ID 8
  "Ketchup",              // ID 9
  "Broken Bones",         // ID 10 - Could be toy bones or real ones if you're a dog!
  "Diposable Chopsticks", // ID 11 - Meant to be "Disposable Chopsticks".
  "Plate"                 // ID 12
  // Add more names here if your camera model knows more objects!
};

// The setup() runs once when the robot turns on.
void setup() {
  Serial.begin(115200);   // Lets the robot send messages to the computer.
  delay(2000);              // Wait 2 seconds for the computer to be ready for messages.
  
  Serial.println("ExoNaut Image Classification Demo"); // Print a title.
  Serial.println("--------------------------------");
  
  robot.begin();            // Wakes up the main robot.
  camera.begin();           // Wakes up the AI Camera.
  
  // Ask the camera what version its software (firmware) is.
  char version[16];         // A place to store the version text.
  if (camera.firmwareVersion(version)) { // If the camera tells us its version...
    Serial.print("Camera firmware version: ");
    Serial.println(version); // ...print it to the computer.
  } else {
    Serial.println("Failed to get camera firmware version"); // Uh oh, couldn't get the version.
  }
  
  // Tell the camera to start trying to classify (identify) objects.
  if (camera.changeFunc(APPLICATION_CLASSIFICATION)) {
    Serial.println("Camera set to classification mode"); // Good, it's ready to identify things!
  } else {
    Serial.println("Failed to set camera mode"); // Problem setting the mode.
  }
  
  camera.setLed(WONDERCAM_LED_ON); // Turn on the camera's little light.
  
  robot.setColorAll(0, 0, 255); // Make the robot's lights blue to start.
  robot.show();                   // Update the lights to show the blue color.
  
  Serial.println("Setup complete. Starting classification...");
  Serial.println(); // Print an empty line for spacing.
}

// The loop() runs over and over, forever!
void loop() {
  camera.updateResult(); // Ask the camera what it sees right now.
  
  // Ask the camera: "What object do you think this is?"
  int classId = camera.classIdOfMaxProb();    // Get the ID number of the most likely object.
  float confidence = camera.classMaxProb();   // Get how sure the camera is (from 0.0 to 1.0).
  
  // Only show results if the camera saw something (classId > 0) and is at least a little sure (confidence > 0.1).
  if (classId > 0 && confidence > 0.1) {
    Serial.print("Detected: "); // Print "Detected: " to the computer.
    
    // Check if the ID number is in our list of names.
    if (classId < sizeof(objectNames) / sizeof(objectNames[0])) {
      Serial.print(objectNames[classId]); // If yes, print the name from our list.
    } else {
      // If the ID is not in our list, just print the ID number.
      Serial.print("Unknown (ID: ");
      Serial.print(classId);
      Serial.print(")");
    }
    Serial.print(" - Confidence: ");
    Serial.print(confidence * 100); // Print confidence as a percentage (e.g., 75%).
    Serial.println("%");
    
    // Change the robot's lights based on how sure the camera is.
    if (confidence > 0.8) {         // If more than 80% sure...
      robot.setColorAll(0, 255, 0); // ...make lights GREEN (very sure!).
    } else if (confidence > 0.5) {  // If more than 50% sure...
      robot.setColorAll(255, 255, 0); // ...make lights YELLOW (kinda sure).
    } else {                        // If less than 50% sure...
      robot.setColorAll(255, 0, 0);   // ...make lights RED (not very sure).
    }
    robot.show(); // Update the lights to show the new color.
    
    // Let's see if the camera thought it might be other things too.
    for (int i = 1; i <= 12; i++) { // Check our known object IDs (1 to 12 from the list).
      if (i != classId) { // Don't check the one we already know is the top guess.
        float otherProb = camera.classProbOfId(i); // Ask camera: "How sure are you it's THIS object ID?"
        if (otherProb > 0.1) { // If it's more than 10% sure...
          Serial.print("  Also possible: "); // ...print that it might also be this other thing.
          Serial.print(objectNames[i]);
          Serial.print(" (");
          Serial.print(otherProb * 100);
          Serial.println("%)");
        }
      }
    }
    Serial.println(); // Add an empty line for better readability.
  }
  
  delay(500); // Wait for half a second before checking again.
}

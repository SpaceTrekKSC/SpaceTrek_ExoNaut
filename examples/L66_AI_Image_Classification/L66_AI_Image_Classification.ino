/**************************************************
 * L66_AI_Image_Classification.ino
 * A program to identify objects using AI camera and indicate confidence with LED colors
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2, 2025
 * 
 * Program Flow:
 * - Camera captures images and classifies objects in view
 * - Classification results displayed on serial monitor
 * - RGB LEDs indicate confidence level:
 *   - Green: High confidence (>80%)
 *   - Yellow: Medium confidence (50-80%)
 *   - Red: Low confidence (<50%)
 * 
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                 //This command creates an AI Camera object
 *                                       //Handles vision-based detection and recognition
 *
 * robot.begin();                        //This command initializes the robot systems
 *
 * camera.begin();                       //This command initializes the camera module
 *
 * camera.firmwareVersion(version);      //This command retrieves the camera firmware version
 *                                       //Stores the version string in the provided variable
 *
 * camera.changeFunc(mode);              //This command changes the camera's operating mode
 *                                       //Set to APPLICATION_CLASSIFICATION for this program
 *
 * camera.updateResult();                //This command updates recognition results from the camera
 *
 * camera.classIdOfMaxProb();            //This command returns the class ID with highest probability
 *
 * camera.classMaxProb();                //This command returns the confidence level of top classification
 *
 * camera.classProbOfId(id);             //This command returns the probability for a specific class ID
 *
 * robot.setColorAll(r, g, b);           //This command sets the color of all onboard NeoPixel LEDs
 *                                       //Parameters are RGB values (0-255)
 *
 * robot.show();                         //This command updates the LED display with new colors
 **************************************************/

#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

// Create instances of required objects
exonaut robot;
ExoNaut_AICam camera;

// Names of object classes for object detection
const char* objectNames[] = {
  "Unknown",
  "Cigarette Ends", "Oral Liquid Bottle", "Marker", "Storage Battery", "Toothbrush",
  "Plastic Bottle", "Umbrella", "Banana Peel", "Ketchup", "Broken Bones",
  "Diposable Chopsticks", "Plate"
};

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Wait for serial to be ready
  delay(2000);
  
  Serial.println("ExoNaut Image Classification Demo");
  Serial.println("--------------------------------");
  
  // Initialize the robot
  robot.begin();
  
  // Initialize the camera
  camera.begin();
  
  // Check camera firmware version
  char version[16];
  if (camera.firmwareVersion(version)) {
    Serial.print("Camera firmware version: ");
    Serial.println(version);
  } else {
    Serial.println("Failed to get camera firmware version");
  }
  
  // Set camera to classification mode
  if (camera.changeFunc(APPLICATION_CLASSIFICATION)) {
    Serial.println("Camera set to classification mode");
  } else {
    Serial.println("Failed to set camera mode");
  }
  
  // Turn on the camera LED
  camera.setLed(WONDERCAM_LED_ON);
  
  // Set all robot LEDs to blue initially
  robot.setColorAll(0, 0, 255);
  robot.show();
  
  Serial.println("Setup complete. Starting classification...");
  Serial.println();
}

void loop() {
  // Update camera results
  camera.updateResult();
  
  // Get classification result
  int classId = camera.classIdOfMaxProb();
  float confidence = camera.classMaxProb();
  
  // Only display results if there's something detected
  if (classId > 0 && confidence > 0.1) {
    // Print the result
    Serial.print("Detected: ");
    if (classId < sizeof(objectNames) / sizeof(objectNames[0])) {
      Serial.print(objectNames[classId]);
    } else {
      Serial.print("Unknown (ID: ");
      Serial.print(classId);
      Serial.print(")");
    }
    Serial.print(" - Confidence: ");
    Serial.print(confidence * 100);
    Serial.println("%");
    
    // Set LEDs based on confidence
    if (confidence > 0.8) {
      // High confidence - green
      robot.setColorAll(0, 255, 0);
    } else if (confidence > 0.5) {
      // Medium confidence - yellow
      robot.setColorAll(255, 255, 0);
    } else {
      // Low confidence - red
      robot.setColorAll(255, 0, 0);
    }
    robot.show();
    
    // Check confidence of other classes
    for (int i = 1; i <= 20; i++) {
      if (i != classId) {  // Skip the top class which we already printed
        float otherProb = camera.classProbOfId(i);
        if (otherProb > 0.1) {  // Only show if probability > 10%
          Serial.print("  Also possible: ");
          Serial.print(objectNames[i]);
          Serial.print(" (");
          Serial.print(otherProb * 100);
          Serial.println("%)");
        }
      }
    }
    
    Serial.println();
  }
  
  // Small delay to avoid flooding the serial monitor
  delay(500);
}

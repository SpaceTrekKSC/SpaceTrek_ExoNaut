/**************************************************
 * L65_AI_FaceDetect.ino
 * A program to detect faces and respond with LED color based on face ID
 *
 * The camera module is an i2c device.  It must be plugged
 * into port 3, 4, 5 or 9.  It will not work in any other ports.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2, 2025
 * 
 * Program Flow:
 * - Detects faces using AI Camera
 * - Face ID 1: Sets all LEDs to Red
 * - Face ID 2: Sets all LEDs to Blue
 * - Unknown faces: Sets all LEDs to Green
 * - No faces: LEDs turned off
 * 
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam aiCamera;               //This command creates an AI Camera object
 *                                       //Handles vision-based detection and recognition
 *
 * robot.begin();                        //This command initializes the robot systems
 *
 * aiCamera.begin();                     //This command initializes the camera module
 *
 * aiCamera.changeFunc(mode);            //This command changes the camera's operating mode
 *                                       //Set to APPLICATION_FACEDETECT for this program
 *
 * aiCamera.updateResult();              //This command updates recognition results from the camera
 *
 * aiCamera.anyFaceDetected();           //This command checks if any faces are detected
 *
 * aiCamera.faceOfIdDetected(id);        //This command checks if a specific face ID is detected
 *
 * aiCamera.anyUnlearnedFaceDetected();  //This command checks if any unknown faces are detected
 *
 * robot.setColorAll(r, g, b);           //This command sets the color of all onboard NeoPixel LEDs
 *                                       //Parameters are RGB values (0-255)
 *
 * robot.show();                         //This command updates the LED display with new colors
 *
 * robot.clear();                        //This command turns off all LEDs
 **************************************************/

#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

// Create instances
exonaut robot;
ExoNaut_AICam aiCamera;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  // Initialize robot
  robot.begin();
  
  // Initialize AI Camera
  aiCamera.begin();
  
  // Set camera to face detection mode
  if (aiCamera.changeFunc(APPLICATION_FACEDETECT)) {
    Serial.println("Successfully switched to Face Detection mode");
  } else {
    Serial.println("Failed to switch to Face Detection mode");
  }
  
  // Turn on camera LED
  aiCamera.setLed(WONDERCAM_LED_ON);
  
  // Clear all LEDs on the robot
  robot.clear();
  
  Serial.println("Face Recognition Demo Started");
  Serial.println("------------------------------");
  Serial.println("Face ID 1: Red LEDs");
  Serial.println("Face ID 2: Blue LEDs");
  Serial.println("Unknown faces: Green LEDs");
  Serial.println("No faces: LEDs off");
  Serial.println("------------------------------");
}

void loop() {
  // Update camera results
  aiCamera.updateResult();
  
  // Check if any faces are detected
  if (aiCamera.anyFaceDetected()) {
    int totalFaces = aiCamera.numOfTotalFaceDetected();
    int learnedFaces = aiCamera.numOfTotalLearnedFaceDetected();
    
    Serial.print("Detected ");
    Serial.print(totalFaces);
    Serial.print(" face(s), ");
    Serial.print(learnedFaces);
    Serial.println(" with known ID");
    
    // Check for specific face IDs
    if (aiCamera.faceOfIdDetected(1)) {
      // Face ID 1 detected - set LEDs to red
      Serial.println("Face ID 1 detected - Setting LEDs to RED");
      robot.setColorAll(255, 0, 0);
      robot.show();
      
      // Get face details and print them
      WonderCamFaceDetectResult faceData;
      if (aiCamera.getFaceOfId(1, &faceData)) {
        printFaceDetails(1, &faceData);
      }
    } 
    else if (aiCamera.faceOfIdDetected(2)) {
      // Face ID 2 detected - set LEDs to blue
      Serial.println("Face ID 2 detected - Setting LEDs to BLUE");
      robot.setColorAll(0, 0, 255);
      robot.show();
      
      // Get face details and print them
      WonderCamFaceDetectResult faceData;
      if (aiCamera.getFaceOfId(2, &faceData)) {
        printFaceDetails(2, &faceData);
      }
    } 
    else if (aiCamera.anyUnlearnedFaceDetected()) {
      // Unknown face detected - set LEDs to green
      Serial.println("Unknown face detected - Setting LEDs to GREEN");
      robot.setColorAll(0, 255, 0);
      robot.show();
      
      // Get first unknown face details
      WonderCamFaceDetectResult faceData;
      if (aiCamera.getFaceOfIndex(0, &faceData)) {
        printFaceDetails(0, &faceData);
      }
    }
  } 
  else {
    // No faces detected - turn off LEDs
    Serial.println("No faces detected - LEDs OFF");
    robot.clear();
  }
  
  // Small delay before next detection
  delay(500);
}

// Helper function to print face details
void printFaceDetails(int id, WonderCamFaceDetectResult* face) {
  Serial.print("Face ID: ");
  Serial.print(id == 0 ? "Unknown" : String(id));
  Serial.print(", Position: (");
  Serial.print(face->x);
  Serial.print(", ");
  Serial.print(face->y);
  Serial.print("), Size: ");
  Serial.print(face->w);
  Serial.print(" x ");
  Serial.println(face->h);
}

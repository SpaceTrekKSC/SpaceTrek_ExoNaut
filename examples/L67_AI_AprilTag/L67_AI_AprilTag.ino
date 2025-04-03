/************************************************** 
 * L67_AI_AprilTag.ino
 * A program to detect and analyze AprilTags using AI camera
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2, 2025
 *
 * Program Flow:
 * - Camera captures images and detects AprilTags in view
 * - Detection results displayed on serial monitor
 * - Provides detailed information about detected tags:
 *   - Tag ID
 *   - Estimated distance
 *   - Orientation
 *
 * Commands:
 * exonaut robot; //This command creates the main robot instance
 * //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera; //This command creates an AI Camera object
 * //Handles vision-based AprilTag detection
 *
 * robot.begin(); //This command initializes the robot systems
 *
 * camera.begin(); //This command initializes the camera module
 *
 * camera.firmwareVersion(version); //This command retrieves the camera firmware version
 * //Stores the version string in the provided variable
 *
 * camera.changeFunc(mode); //This command changes the camera's operating mode
 * //Set to APPLICATION_APRILTAG for this program
 *
 * camera.updateResult(); //This command updates AprilTag detection results from the camera
 *
 * cam.listDetectedTagIds(); //This command lists all detected AprilTag IDs
 *
 * cam.getTagInfo(tagId, &tag); //This command retrieves detailed information for a specific tag
 *
 * cam.estimateTagDistance(&tag); //This command estimates the distance to a detected tag
 *
 * cam.getTagOrientation(&tag); //This command retrieves the orientation of a detected tag
 *
 * cam.printAllTagDetails(); //This command prints detailed information for all detected tags
 **************************************************/
#include <Wire.h>
#include "ExoNaut_AICam.h"

ExoNaut_AICam cam;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial monitor to open

  Wire.begin();
  cam.begin();

  // Switch to AprilTag mode
  if (cam.changeFunc(APPLICATION_APRILTAG)) {
    Serial.println("Switched to AprilTag mode");
  } else {
    Serial.println("Failed to switch to AprilTag mode");
  }
  
  // Give the camera time to update its results
  delay(1000);
}

void loop() {
  // Update the camera's result data
  cam.updateResult();
  
  // Get the number of detected AprilTags from result_summ[1]
  uint8_t tagCount = cam.result_summ[1];
  Serial.print("Detected ");
  Serial.print(tagCount);
  Serial.println(" AprilTag(s)");
  
  // List all detected tag IDs
  cam.listDetectedTagIds();
  
  // For each detected tag, get and print its details
  for (int i = 0; i < tagCount; i++) {
    // The tag IDs are stored starting at result_summ[2]
    uint16_t tagId = cam.result_summ[2 + i];
    WonderCamAprilTagResult tag;
    if (cam.getTagInfo(tagId, &tag)) {
      Serial.print("Tag ID: ");
      Serial.print(tagId);
      Serial.print(" | Estimated Distance: ");
      float distance = cam.estimateTagDistance(&tag);
      Serial.print(distance);
      Serial.print(" | Orientation (z_r): ");
      Serial.println(cam.getTagOrientation(&tag));
    } else {
      Serial.print("Failed to get details for tag ID: ");
      Serial.println(tagId);
    }
  }
  
  // Also print detailed information for all tags using the helper function
  cam.printAllTagDetails();

  // Wait a bit before updating again
  delay(2000);
}

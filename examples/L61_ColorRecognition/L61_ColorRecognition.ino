#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

// Create instances of ExoNaut and ExoNaut_AICam
exonaut robot;
ExoNaut_AICam camera;

// Movement parameters
const float FORWARD_SPEED = 30.0;  // Speed when moving forward
const float STOP_SPEED = 0.0;      // Speed when stopping

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize the robot
  robot.begin();
  
  // Initialize the camera
  camera.begin();
  
  // Change camera function to color detection
  camera.changeFunc(APPLICATION_COLORDETECT);
  
  // Turn on camera LED
  camera.setLed(WONDERCAM_LED_ON);
  
  // Brief delay to allow camera to initialize
  delay(1000);
}

void loop() {
  // Update camera results
  camera.updateResult();
  
  // Check if any color is detected
  if (camera.anyColorDetected()) {
    // Get number of detected colors
    int colorCount = camera.numOfColorDetected();
    Serial.print("Colors detected: ");
    Serial.println(colorCount);
    
    // Check for specific color IDs
    if (camera.colorIdDetected(1)) {
      // Color ID 1 detected - move forward
      Serial.println("Color ID 1 detected - Moving Forward");
      robot.set_motor_speed(FORWARD_SPEED, FORWARD_SPEED);
    }
    else if (camera.colorIdDetected(2)) {
      // Color ID 2 detected - stop
      Serial.println("Color ID 2 detected - Stopping");
      robot.set_motor_speed(STOP_SPEED, STOP_SPEED);
    }
  }
  else {
    // No colors detected - optional default behavior
    Serial.println("No colors detected");
    robot.set_motor_speed(STOP_SPEED, STOP_SPEED);
  }
  
  // Small delay to prevent overwhelming the system
  delay(100);
}

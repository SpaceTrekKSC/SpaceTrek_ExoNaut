/***************************************************
 * L61_Color_Control.ino
 *
 * A simple program that lets kids control the robot
 * using color cards and the ExoNaut AI Camera!
 *
 * 🎨 Color ID 1 = Go Forward
 * 🟥 Color ID 2 = Stop
 * ❌ No color = Stop
 *
 * 📍 Plug the AI Camera into port 3, 4, 5, or 9.
 *
 * Author: Ryan Bori
 * Email:  ryan.bori@spacetrek.com
 * Date:   March 30, 2025
 *
 * 🛠️ Commands used:
 * - exonaut robot;                 → makes the robot object
 * - ExoNaut_AICam camera;          → makes the camera object
 * - camera.begin();                → turns the camera on
 * - camera.changeFunc(mode);      → sets the camera mode
 * - camera.setLed(ON or OFF);     → turns on the camera light
 * - camera.updateResult();        → gets latest camera data
 * - camera.anyColorDetected();    → checks if any color is seen
 * - camera.colorIdDetected(id);   → checks if color ID (1–7) is seen
 * - robot.set_motor_speed(L, R);  → sets the left & right wheel speeds
 ***************************************************/

#include <ExoNaut.h>
#include <ExoNaut_AICam.h>

exonaut robot;            // The robot
ExoNaut_AICam camera;     // The camera

void setup() {
  Serial.begin(115200);         // Start the Serial Monitor
  robot.begin();                // Start the robot
  camera.begin();               // Start the camera
  camera.changeFunc(APPLICATION_COLORDETECT); // Look for colors
  delay(1000);                  // Wait a bit to get ready

  Serial.println("👀 Show a color card to the camera!");
}

void loop() {
  camera.updateResult();        // Get latest camera data

  if (camera.anyColorDetected()) {
    Serial.println("🎨 Color seen!");

    if (camera.colorIdDetected(1)) {
      Serial.println("➡️ Go Forward!");
      robot.set_motor_speed(30, 30);
    } 
    else if (camera.colorIdDetected(2)) {
      Serial.println("🛑 Stop!");
      robot.set_motor_speed(0, 0);
    }
  } 
  else {
    Serial.println("❌ No color found.");
    robot.set_motor_speed(0, 0); // Always stop if no color
  }

  delay(200);  // Slow down the loop a bit
}


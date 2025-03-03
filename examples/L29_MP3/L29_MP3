/*
 * ExoNaut_MP3_Example.ino
 * 
 * Example sketch for demonstrating the ExoNaut_MP3 library
 * with the Space Trek ExoNaut robot.
 * 
 * This sketch provides basic MP3 playback functionality using:
 * - Button A (PIN 0): Next Track
 * - Button B (PIN 2): Play/Pause
 * - LED indicators for playback status
 * 
 * For Hiwonder MP3 Module (I2C communication)
 * 
 * Date: March 3rd, 2025
 */

#include <ExoNaut.h>
#include <Wire.h>
#include "ExoNaut_MP3.h"

// Create objects
exonaut robot;
ExoNaut_MP3 mp3;

// Simple testing state
int currentTrack = 1;
boolean isPlaying = false;
int trackCount = 0;  // Will be set during setup

// Button debounce variables
unsigned long lastButtonPressA = 0;
unsigned long lastButtonPressB = 0;
const unsigned long debounceTime = 300; // milliseconds

void setup() {
  // Initialize serial monitor for debugging
  Serial.begin(115200);
  Serial.println("ExoNaut MP3 Module Example (I2C Mode)");
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize the robot
  robot.begin();
  
  // Set all LEDs to green during initialization
  robot.setColorAll(0, 100, 0);
  robot.show();
  
  // Initialize the MP3 module
  Serial.println("Initializing MP3 module...");
  mp3.begin();
  
  // Set initial volume (0-30)
  mp3.setVolume(20);
  Serial.println("Volume set to 20");
  
  // Get total track count
  trackCount = mp3.getTotalTracks();
  Serial.print("Total tracks detected: ");
  Serial.println(trackCount);
  
  // If track count is incorrect (we know there are only 6), adjust it
  if (trackCount != 6) {
    Serial.println("Overriding track count to 6");
    trackCount = 6;
  }
  
  // Brief delay
  delay(1000);
  
  // Play startup animation
  startupAnimation();
  
  // Configure button pins with internal pull-ups/pull-downs as needed
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);   // Button A uses pull-up: LOW when pressed
  pinMode(BUTTON_B_PIN, INPUT);          // Button B uses different logic: HIGH when pressed
  
  // Print instructions
  Serial.println("\nMP3 Controls:");
  Serial.println("- Button A (PIN 0): Next track");
  Serial.println("- Button B (PIN 2): Play/Pause current track");
  Serial.println("- Hold Button A for 2 sec: Volume up");
  Serial.println("- Hold Button B for 2 sec: Volume down");
  
  // Ignore button presses for the first second to avoid initial false readings
  Serial.println("\nIgnoring button inputs for 1 second to stabilize...");
  unsigned long startIgnoreTime = millis();
  while (millis() - startIgnoreTime < 1000) {
    // Flash LEDs to show we're waiting
    if ((millis() - startIgnoreTime) % 200 < 100) {
      robot.setColorAll(50, 0, 0);  // Red
    } else {
      robot.setColorAll(0, 0, 0);   // Off
    }
    robot.show();
    delay(10);
  }
  
  Serial.println("System ready! Controls are now active.");
  
  // Set LEDs to standby mode (dim blue)
  robot.setColorAll(0, 0, 30);
  robot.show();
}

void loop() {
  // Current time for debounce checks
  unsigned long currentTime = millis();
  
  // Read button states with stable readings
  bool buttonAPressed = (digitalRead(BUTTON_A_PIN) == LOW);   // Button A: LOW when pressed
  bool buttonBPressed = (digitalRead(BUTTON_B_PIN) == HIGH);  // Button B: HIGH when pressed (reversed)
  
  // Button A (Next Track) with debounce
  if (buttonAPressed && (currentTime - lastButtonPressA > debounceTime)) {
    // Register button press time
    lastButtonPressA = currentTime;
    Serial.println("Button A pressed - waiting for release...");
    
    // LED feedback to show button is detected
    robot.setColorAll(100, 100, 0); // Yellow for A press
    robot.show();
    
    // Wait for button release and measure press duration
    long pressStartTime = millis();
    bool longPressHandled = false;
    
    while (digitalRead(BUTTON_A_PIN) == LOW) {
      long pressDuration = millis() - pressStartTime;
      
      // After 2 seconds, indicate long press is detected
      if (pressDuration > 2000 && !longPressHandled) {
        // Flash the LEDs to indicate long press recognized
        robot.setColorAll(0, 100, 0); // Green for volume up
        robot.show();
        delay(200);
        robot.setColorAll(100, 100, 0); // Back to yellow
        robot.show();
        
        longPressHandled = true;
      }
      
      delay(10);
    }
    
    // Measure final press duration
    long pressDuration = millis() - pressStartTime;
    
    // Different actions based on press duration
    if (pressDuration > 2000) {
      // Long press: Volume up
      Serial.println("Long press confirmed - Volume Up");
      volumeUpEffect();
    }
    else {
      // Short press: Next track
      Serial.println("Short press - Next track");
      currentTrack++;
      if (currentTrack > trackCount || trackCount == 0) {
        currentTrack = 1;
      }
      
      Serial.print("Playing track: ");
      Serial.println(currentTrack);
      
      // Play the new track
      mp3.playTrack(currentTrack);
      isPlaying = true;
      
      // Show visual track change
      trackChangeEffect(currentTrack);
      
      // Set LEDs to blue to indicate playing
      robot.setColorAll(0, 0, 100);
      robot.show();
    }
    
    // Add extra debounce delay after release
    delay(100);
  }
  
  // Button B (Play/Pause) with debounce
  if (buttonBPressed && (currentTime - lastButtonPressB > debounceTime)) {
    // Register button press time
    lastButtonPressB = currentTime;
    Serial.println("Button B pressed - waiting for release...");
    
    // LED feedback to show button is detected
    robot.setColorAll(100, 0, 100); // Purple for B press
    robot.show();
    
    // Wait for button release and measure press duration
    long pressStartTime = millis();
    bool longPressHandled = false;
    
    while (digitalRead(BUTTON_B_PIN) == HIGH) {  // Changed: Wait while Button B is HIGH (pressed)
      long pressDuration = millis() - pressStartTime;
      
      // After 2 seconds, indicate long press is detected
      if (pressDuration > 2000 && !longPressHandled) {
        // Flash the LEDs to indicate long press recognized
        robot.setColorAll(100, 0, 0); // Red for volume down
        robot.show();
        delay(200);
        robot.setColorAll(100, 0, 100); // Back to purple
        robot.show();
        
        longPressHandled = true;
      }
      
      delay(10);
    }
    
    // Measure final press duration
    long pressDuration = millis() - pressStartTime;
    
    // Different actions based on press duration
    if (pressDuration > 2000) {
      // Long press: Volume down
      Serial.println("Long press confirmed - Volume Down");
      volumeDownEffect();
    }
    else {
      // Short press: Play/Pause
      if (!isPlaying) {
        Serial.println("Short press - Playing");
        if (mp3.getCurrentTrack() == 0) {
          // If no track is set, play track 1
          mp3.playTrack(1);
        } else {
          // Otherwise resume current track
          mp3.play();
        }
        isPlaying = true;
        
        // Set LEDs to blue to indicate playing
        robot.setColorAll(0, 0, 100);
        robot.show();
      } else {
        Serial.println("Short press - Pausing");
        mp3.pause();
        isPlaying = false;
        
        // Set LEDs to purple to indicate paused
        robot.setColorAll(50, 0, 50);
        robot.show();
      }
    }
    
    // Add extra debounce delay after release
    delay(100);
  }
  
  // Small delay to prevent CPU hogging
  delay(10);
}

// Visual effects

void startupAnimation() {
  // Perform a color wipe animation
  for (int i = 0; i < NUM_PIXELS; i++) {
    robot.clear();
    for (int j = 0; j <= i; j++) {
      robot.setColor(j, 0, 100, 100);  // Teal color
    }
    robot.show();
    delay(100);
  }
  
  // Flash all LEDs
  robot.setColorAll(100, 100, 100);
  robot.show();
  delay(200);
  robot.clear();
  robot.show();
  delay(200);
  robot.setColorAll(100, 100, 100);
  robot.show();
  delay(200);
  robot.clear();
  robot.show();
}

void volumeUpEffect() {
  // Increase volume
  int oldVolume = mp3.getVolume();
  mp3.volumeUp();
  int newVolume = mp3.getVolume();
  
  // Show visual effect for volume change
  for (int i = 0; i < 3; i++) {
    // Flash green
    robot.setColorAll(0, 100, 0);
    robot.show();
    delay(100);
    
    // Return to appropriate state color
    if (isPlaying) {
      robot.setColorAll(0, 0, 100);  // Blue for playing
    } else {
      robot.setColorAll(50, 0, 50);  // Purple for paused
    }
    robot.show();
    delay(100);
  }
  
  Serial.print("Volume changed from ");
  Serial.print(oldVolume);
  Serial.print(" to ");
  Serial.println(newVolume);
}

void volumeDownEffect() {
  // Decrease volume
  int oldVolume = mp3.getVolume();
  mp3.volumeDown();
  int newVolume = mp3.getVolume();
  
  // Show visual effect for volume change
  for (int i = 0; i < 3; i++) {
    // Flash red
    robot.setColorAll(100, 0, 0);
    robot.show();
    delay(100);
    
    // Return to appropriate state color
    if (isPlaying) {
      robot.setColorAll(0, 0, 100);  // Blue for playing
    } else {
      robot.setColorAll(50, 0, 50);  // Purple for paused
    }
    robot.show();
    delay(100);
  }
  
  Serial.print("Volume changed from ");
  Serial.print(oldVolume);
  Serial.print(" to ");
  Serial.println(newVolume);
}

void trackChangeEffect(int trackNum) {
  // Flash a unique color based on track number
  for (int i = 0; i < 2; i++) {
    switch (trackNum % 5) {
      case 0:
        robot.setColorAll(100, 0, 0);    // Red
        break;
      case 1:
        robot.setColorAll(100, 100, 0);  // Yellow
        break;
      case 2:
        robot.setColorAll(0, 100, 0);    // Green
        break;
      case 3:
        robot.setColorAll(0, 100, 100);  // Cyan
        break;
      case 4:
        robot.setColorAll(100, 0, 100);  // Magenta
        break;
    }
    robot.show();
    delay(100);
    robot.clear();
    robot.show();
    delay(100);
  }
}

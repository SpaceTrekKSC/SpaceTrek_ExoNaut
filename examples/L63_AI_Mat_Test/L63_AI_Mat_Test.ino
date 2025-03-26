/**************************************************
 * ExoNaut_Multi_Station_Navigation.ino
 * A program to navigate through 3 stations with different sensing modes
 *
 * Author: [Your Name]
 * Date: March 26, 2025
 * 
 * Station 1: Color detection (wait for color ID 1)
 * Station 2: Landmark recognition (ID 1 = right turn, ID 2 = left turn)
 * Station 3: Take another right or left based on Station 2 decision
 * 
**************************************************/

#include <ExoNaut.h>                          // Include the ExoNaut library
#include <ExoNaut_LineFollower.h>             // Include the ExoNaut Line Follower library
#include <ExoNaut_AICam.h>                    // Include the ExoNaut AI Camera library

// Create objects
exonaut robot;                                // Robot control object
lineFollower lf;                              // Line follower object
ExoNaut_AICam camera;                         // AI Camera object

// Variables
uint8_t lineData = 0;                         // Variable to store line sensor data
int currentStation = 0;                       // Current station (0 = between stations, 1-3 = at station)
bool stationDetected = false;                 // Flag to detect station entry
unsigned long stationEntryTime = 0;           // Time when entering a station
int directionTaken = 0;                       // Direction taken at station 2 (1 = right, 2 = left)
int currentMode = 0;                          // Current mode (0 = line following, 1 = at station)

void setup() {
  Serial.begin(115200);                       // Initialize serial communication for debugging
  robot.begin();                              // Initialize robot
  camera.begin();                             // Initialize AI camera
  robot.stop_motor(0);                        // Stop both motors initially
  
  // Set initial mode to line following
  camera.changeFunc(APPLICATION_LINEFOLLOW);
  currentMode = 0;
  
  delay(1000);                                // Wait 1 second before starting
  Serial.println("Robot navigation started");
}

void loop() {
  // Update line follower data
  lf.readLineFollower(lineData);
  
  // Detect station entry (all sensors detect white)
  if (lineData == 0b0000 && !stationDetected) {
    stationDetected = true;
    stationEntryTime = millis();
    currentStation = (currentStation % 3) + 1;  // Cycle through stations 1-3
    currentMode = 1;  // Switch to station mode
    robot.stop_motor(0);
    
    Serial.print("Entered Station ");
    Serial.println(currentStation);
    
    // Configure camera for appropriate detection mode at this station
    handleStationEntry();
  }
  
  // At a station - handle according to current station
  if (currentMode == 1) {
    handleStationLogic();
  }
  // Between stations - follow the line
  else {
    // Reset station detection when line is detected again after station
    if (stationDetected && lineData != 0b0000) {
      stationDetected = false;
    }
    
    // Line following logic
    followLine();
  }
  
  delay(10);  // Small delay for stability
}

void handleStationEntry() {
  switch(currentStation) {
    case 1:
      // At station 1, switch to color detection mode
      camera.changeFunc(APPLICATION_COLORDETECT);
      Serial.println("Switched to color detection mode");
      break;
      
    case 2:
      // At station 2, switch to landmark recognition mode
      camera.changeFunc(APPLICATION_LANDMARK);
      Serial.println("Switched to landmark recognition mode");
      break;
      
    case 3:
      // At station 3, no special sensing needed, just make the same turn as station 2
      Serial.println("At station 3, will make turn based on station 2 decision");
      break;
  }
}

void handleStationLogic() {
  // Update camera results
  camera.updateResult();
  
  switch(currentStation) {
    case 1:
      // Station 1: Wait for color ID 1
      if (camera.colorIdDetected(1)) {
        Serial.println("Color ID 1 detected, moving forward");
        robot.set_motor_speed(40, 40);  // Move forward
        delay(1000);  // Move for 1 second
        
        // Return to line following mode
        camera.changeFunc(APPLICATION_LINEFOLLOW);
        currentMode = 0;
      }
      break;
      
    case 2:
      while(lineData == 0b0000){
        lf.readLineFollower(lineData);
        robot.set_motor_speed(10, 10);

      }
      robot.stop_motor(0);
      // Station 2: Check for landmarks and decide direction
      if (camera.landmarkIdDetected(1)) {
        Serial.println("Landmark ID 1 detected, turning right");
        robot.set_motor_speed(40, 5);  // Turn right
        directionTaken = 1;  // Remember we turned right
        delay(1000);  // Turn for 1 second
        
        // Return to line following mode
        camera.changeFunc(APPLICATION_LINEFOLLOW);
        currentMode = 0;
      }
      else if (camera.landmarkIdDetected(2)) {
        Serial.println("Landmark ID 2 detected, turning left");
        robot.set_motor_speed(5, 40);  // Turn left
        directionTaken = 2;  // Remember we turned left
        delay(1000);  // Turn for 1 second
        
        // Return to line following mode
        camera.changeFunc(APPLICATION_LINEFOLLOW);
        currentMode = 0;
      }
      break;
      
    case 3:
      // Station 3: Make the same turn as in station 2
      if (directionTaken == 1) {
        Serial.println("At station 3, turning right again");
        robot.set_motor_speed(40, 5);  // Turn right
        delay(1000);  // Turn for 1 second
      }
      else if (directionTaken == 2) {
        Serial.println("At station 3, turning left again");
        robot.set_motor_speed(5, 40);  // Turn left
        delay(1000);  // Turn for 1 second
      }
      
      // Return to line following mode
      camera.changeFunc(APPLICATION_LINEFOLLOW);
      currentMode = 0;
      break;
  }
}

void followLine() {
  switch(lineData) {
    case 0b0000:                              // All detect white
      // This is handled in the main loop as station detection
      break;
    case 0b0001:                              // Only sensor 1 detects black
      robot.set_motor_speed(40, -10);         // Turn right
      break;
    case 0b0010:                              // Only sensor 2 detects black
      robot.set_motor_speed(40, 6);
      break;
    case 0b0011:                              // Sensor 1 and 2 detect black
      robot.set_motor_speed(40, -5);
      break;
    case 0b0100:                              // Only sensor 3 detects black
      robot.set_motor_speed(6, 40);
      break;
    case 0b0110:                              // Sensor 2 and 3 detect black
      robot.set_motor_speed(40, 40);          // Go straight
      break;
    case 0b0111:                              // Sensor 1, 2 and 3 detect black
      robot.set_motor_speed(40, 0);           // Turn right
      break;
    case 0b1000:                              // Only sensor 4 detects black
      robot.set_motor_speed(-10, 40);         // Turn left
      break;
    case 0b1100:                              // Sensor 3 and 4 detect black
      robot.set_motor_speed(-5, 40);
      break;
    case 0b1110:                              // Sensor 2, 3 and 4 detect black
      robot.set_motor_speed(0, 40);           // Turn left
      break;
    case 0b1111:                              // All detect black
      robot.set_motor_speed(30, 30);          // This is possibly an intersection, go straight
      break;
    case 0b0101:                              // Sensor 1 and 3 detect black
      robot.set_motor_speed(40, 20);
      break;
    case 0b1001:                              // Sensor 1 and 4 detect black
      robot.set_motor_speed(30, 30);          // Possibly an intersection, go straight
      break;
    case 0b1010:                              // Sensor 2 and 4 detect black
      robot.set_motor_speed(20, 40);
      break;
    case 0b1011:                              // Sensor 1, 2 and 4 detect black
      robot.set_motor_speed(20, 20);          // Slow down for complex pattern
      break;
    case 0b1101:                              // Sensor 1, 3 and 4 detect black
      robot.set_motor_speed(20, 20);          // Slow down for complex pattern
      break;
    default:                                  // Unexpected value
      robot.set_motor_speed(20, 20);          // Slow down
      break;
  }
}

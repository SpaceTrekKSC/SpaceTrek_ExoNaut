/**************************************************
 * L63_AI_Mat.ino
 * A simplified program to navigate through 3 stations with different sensing modes
 *
 * The camera module is an i2c device. It must be plugged
 * into port 3, 4, 5 or 9. It will not work in any other ports.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: May 29, 2025
 * 
 * Program Flow:
 * Station 1: Color detection (wait for color ID 1, then move forward)
 * Station 2: Landmark recognition (ID 1 = right turn, ID 2 = left turn)
 * Station 3: Take another right or left based on Station 2 decision
 * 
 * Commands:
 * exonaut robot;                        //Creates the main robot instance
 *                                       //Handles motors and core functions
 *
 * lineFollower lf;                      //Creates a line follower object
 *                                       //Used to read the state of line following sensors
 *
 * ExoNaut_AICam camera;                 //Creates an AI Camera object
 *                                       //Handles vision-based detection and recognition
 *
 * robot.begin();                        //Initializes the robot systems
 *
 * camera.begin();                       //Initializes the camera module
 *
 * camera.changeFunc(mode);              //Changes the camera's operating mode
 *                                       //Modes: APPLICATION_COLORDETECT, APPLICATION_LANDMARK
 *
 * lf.readLineFollower(lineData);        //Reads the line follower sensors
 *                                       //Updates lineData with sensor values
 *
 * camera.updateResult();                //Updates recognition results from the camera
 *
 * camera.colorIdDetected(id);           //Checks if a specific color ID is detected
 *
 * camera.landmarkIdDetected(id);        //Checks if a specific landmark ID is detected
 *
 * robot.set_motor_speed(left, right);   //Sets the motor speeds (-100 to 100)
 **************************************************/

#include <ExoNaut.h>
#include <ExoNaut_LineFollower.h>
#include <ExoNaut_AICam.h>

// Create objects
exonaut robot;
lineFollower lf;
ExoNaut_AICam camera;

// Simple state tracking
uint8_t lineData = 0;
int station = 0;           // 0=line following, 1=station1, 2=station2, 3=station3
int savedDirection = 0;    // 1=right, 2=left (from station 2)

void setup() {
  Serial.begin(115200);
  
  // Initialize everything
  robot.begin();
  camera.begin();
  camera.changeFunc(APPLICATION_LINEFOLLOW);
  
  Serial.println("Starting navigation...");
}

void loop() {
  lf.readLineFollower(lineData);
  
  // Check for station (all sensors see white)
  if (lineData == 0b0000 && station == 0) {
    station = (station % 3) + 1;  // Go to next station (1, 2, or 3)
    robot.set_motor_speed(0, 0);  // Stop
    
    Serial.print("At Station ");
    Serial.println(station);
    
    // Set up camera for this station
    if (station == 1) {
      camera.changeFunc(APPLICATION_COLORDETECT);
    } else if (station == 2) {
      camera.changeFunc(APPLICATION_LANDMARK);
    }
    
  } else if (station > 0) {
    // At a station - handle detection
    handleStation();
    
  } else {
    // Following line - simple line following
    followLine();
  }
  
  delay(50);
}

void handleStation() {
  camera.updateResult();
  
  if (station == 1) {
    // Station 1: Wait for red color
    if (camera.colorIdDetected(1)) {
      Serial.println("Red detected! Moving forward");
      robot.set_motor_speed(40, 40);
      delay(1000);
      
      // Back to line following
      camera.changeFunc(APPLICATION_LINEFOLLOW);
      station = 0;
    }
    
  } else if (station == 2) {
    // Station 2: Check landmarks
    if (camera.landmarkIdDetected(1)) {
      Serial.println("Landmark 1: Turning right");
      robot.set_motor_speed(40, -20);
      delay(800);
      savedDirection = 1;  // Remember right turn
      
      camera.changeFunc(APPLICATION_LINEFOLLOW);
      station = 0;
      
    } else if (camera.landmarkIdDetected(2)) {
      Serial.println("Landmark 2: Turning left");
      robot.set_motor_speed(-20, 40);
      delay(800);
      savedDirection = 2;  // Remember left turn
      
      camera.changeFunc(APPLICATION_LINEFOLLOW);
      station = 0;
    }
    
  } else if (station == 3) {
    // Station 3: Repeat the saved direction
    if (savedDirection == 1) {
      Serial.println("Station 3: Turning right again");
      robot.set_motor_speed(40, -20);
      delay(800);
    } else if (savedDirection == 2) {
      Serial.println("Station 3: Turning left again");
      robot.set_motor_speed(-20, 40);
      delay(800);
    }
    
    camera.changeFunc(APPLICATION_LINEFOLLOW);
    station = 0;
  }
}

void followLine() {
  // Simple line following logic
  switch(lineData) {
    case 0b0001:  // Far right sensor
      robot.set_motor_speed(40, -10);
      break;
    case 0b0010:  // Right sensor
      robot.set_motor_speed(40, 10);
      break;
    case 0b0100:  // Left sensor  
      robot.set_motor_speed(10, 40);
      break;
    case 0b1000:  // Far left sensor
      robot.set_motor_speed(-10, 40);
      break;
    case 0b0110:  // Center sensors - go straight
      robot.set_motor_speed(40, 40);
      break;
    case 0b1111:  // All sensors - intersection
      robot.set_motor_speed(30, 30);
      break;
    default:      // Other combinations
      robot.set_motor_speed(25, 25);
      break;
  }
}
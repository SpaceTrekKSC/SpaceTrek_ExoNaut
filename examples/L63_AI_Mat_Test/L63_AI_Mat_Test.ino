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
 * * Program Flow:
 * Station 1: Color detection (wait for color ID 1, then move forward)
 * Station 2: Landmark recognition (ID 1 = right turn, ID 2 = left turn)
 * Station 3: Take another right or left based on Station 2 decision
 * * Commands:
 * exonaut robot;                       //Creates the main robot instance
 * //Handles motors and core functions
 *
 * lineFollower lf;                     //Creates a line follower object
 * //Used to read the state of line following sensors
 *
 * ExoNaut_AICam camera;                //Creates an AI Camera object
 * //Handles vision-based detection and recognition
 *
 * robot.begin();                       //Initializes the robot systems
 *
 * camera.begin();                      //Initializes the camera module
 *
 * camera.changeFunc(mode);             //Changes the camera's operating mode
 * //Modes: APPLICATION_COLORDETECT, APPLICATION_LANDMARK
 *
 * lf.readLineFollower(lineData);       //Reads the line follower sensors
 * //Updates lineData with sensor values
 *
 * camera.updateResult();               //Updates recognition results from the camera
 *
 * camera.colorIdDetected(id);          //Checks if a specific color ID is detected
 *
 * camera.landmarkIdDetected(id);       //Checks if a specific landmark ID is detected
 *
 * robot.set_motor_speed(left, right);  //Sets the motor speeds (-100 to 100)
 **************************************************/

// These lines add the special instructions (libraries) our robot needs.
#include <ExoNaut.h>              // For basic robot controls.
#include <ExoNaut_LineFollower.h> // For the simple line following sensors.
#include <ExoNaut_AICam.h>          // For the AI Camera's smart vision.

// Let's name our robot and its parts so we can use them in the code.
exonaut robot;                  // This is our main ExoNaut robot.
lineFollower lf;                // This is for the line follower sensors on the robot's belly.
ExoNaut_AICam camera;           // This is the robot's AI Camera.

// These variables help the robot remember what it's doing.
uint8_t lineData = 0;       // Stores data from the line follower sensors.
int station = 0;            // Keeps track of which station the robot is at (0 = following a line).
int savedDirection = 0;     // Remembers if the robot turned left (2) or right (1) at Station 2.

// The setup() runs once when the robot turns on.
void setup() {
  Serial.begin(115200);     // Lets the robot send messages to the computer.
  
  // Wake up all the robot parts!
  robot.begin();            // Wakes up the main robot.
  camera.begin();           // Wakes up the AI Camera.
  camera.changeFunc(APPLICATION_LINEFOLLOW); // Tell the camera to start by looking for lines.
  
  Serial.println("Starting navigation..."); // Send a message to the computer.
}

// The loop() runs over and over, forever!
void loop() {
  lf.readLineFollower(lineData); // Read the line follower sensors to see where the line is.
  
  // Check if the robot has arrived at a station (all sensors see white, meaning no line).
  // And also check if it was previously following a line (station == 0).
  if (lineData == 0b0000 && station == 0) {
    station = (station % 3) + 1;  // Go to the next station (1, then 2, then 3, then back to 1...).
    robot.set_motor_speed(0, 0);  // Stop the robot.
    
    Serial.print("At Station "); // Print which station it's at.
    Serial.println(station);
    
    // Get the camera ready for the current station.
    if (station == 1) {
      camera.changeFunc(APPLICATION_COLORDETECT); // At Station 1, look for colors.
    } else if (station == 2) {
      camera.changeFunc(APPLICATION_LANDMARK);    // At Station 2, look for landmarks.
    }
    // For Station 3, we don't need to change the camera mode from what it was at Station 2.
    
  } else if (station > 0) {
    // If the robot is at a station (station is 1, 2, or 3)...
    handleStation(); // ...go to the instructions for handling that station.
    
  } else {
    // If the robot is not at a station, it means it should be following a line.
    followLine(); // Go to the line following instructions.
  }
  
  delay(50); // Wait a tiny bit before the loop starts again.
}

// This function contains the instructions for what to do at each station.
void handleStation() {
  camera.updateResult(); // Ask the camera what it sees.
  
  if (station == 1) {
    // Station 1: Look for a specific color (ID 1, which is usually Red for this camera).
    if (camera.colorIdDetected(1)) { // Color ID 1 is often the first color learned or default red.
      Serial.println("Red detected! Moving forward");
      robot.set_motor_speed(40, 40); // Move forward a bit.
      delay(1000);                  // Keep moving for 1 second.
      
      // Done with Station 1, now go back to line following.
      camera.changeFunc(APPLICATION_LINEFOLLOW);
      station = 0; // Reset station to 0, meaning "now following line".
    }
    
  } else if (station == 2) {
    // Station 2: Look for landmarks.
    if (camera.landmarkIdDetected(1)) { // Landmark ID 1 might be one picture.
      Serial.println("Landmark 1: Turning right");
      robot.set_motor_speed(40, -20); // Turn right.
      delay(800);                    // Turn for 0.8 seconds.
      savedDirection = 1;             // Remember we turned right.
      
      camera.changeFunc(APPLICATION_LINEFOLLOW); // Go back to line following.
      station = 0;
      
    } else if (camera.landmarkIdDetected(2)) { // Landmark ID 2 might be a different picture.
      Serial.println("Landmark 2: Turning left");
      robot.set_motor_speed(-20, 40); // Turn left.
      delay(800);                    // Turn for 0.8 seconds.
      savedDirection = 2;             // Remember we turned left.
      
      camera.changeFunc(APPLICATION_LINEFOLLOW); // Go back to line following.
      station = 0;
    }
    
  } else if (station == 3) {
    // Station 3: Do the same turn we did at Station 2.
    if (savedDirection == 1) { // If we turned right at Station 2...
      Serial.println("Station 3: Turning right again");
      robot.set_motor_speed(40, -20); // ...turn right again.
      delay(800);
    } else if (savedDirection == 2) { // If we turned left at Station 2...
      Serial.println("Station 3: Turning left again");
      robot.set_motor_speed(-20, 40); // ...turn left again.
      delay(800);
    }
    
    camera.changeFunc(APPLICATION_LINEFOLLOW); // Go back to line following.
    station = 0;
  }
}

// This function tells the robot how to follow a line based on its sensors.
void followLine() {
  // The 'lineData' tells us which sensors see the line.
  // 0b0001 means only the far right sensor sees the line.
  // 0b0010 means the right sensor sees the line.
  // And so on...
  switch(lineData) {
    case 0b0001:  // Line is far to the right.
      robot.set_motor_speed(40, -10); // Turn sharply right.
      break;
    case 0b0010:  // Line is to the right.
      robot.set_motor_speed(40, 10);  // Turn gently right.
      break;
    case 0b0100:  // Line is to the left.
      robot.set_motor_speed(10, 40);  // Turn gently left.
      break;
    case 0b1000:  // Line is far to the left.
      robot.set_motor_speed(-10, 40); // Turn sharply left.
      break;
    case 0b0011: // Line is a bit to the right, but broad
      robot.set_motor_speed(40, 20); // Gentle right
      break;
    case 0b0110:  // Line is centered under the two middle sensors.
      robot.set_motor_speed(40, 40);  // Go straight.
      break;
    case 0b1100: // Line is a bit to the left, but broad
      robot.set_motor_speed(20, 40); // Gentle left
      break;
    case 0b1111:  // All sensors see the line (maybe an intersection or the robot is on the station spot).
      robot.set_motor_speed(30, 30);  // Go straight slowly, or stop if it's a station.
      break;
    default:      // If the sensors see something else (or nothing).
      robot.set_motor_speed(25, 25);  // Try to go straight slowly to find the line.
      break;
  }
}

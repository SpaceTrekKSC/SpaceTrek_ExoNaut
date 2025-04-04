/*
 * L60_AICam.ino
 *
 * This sketch allows control of the Space Trek ExoNaut Robot and AI Camera
 * through serial monitor commands. It provides an interactive interface
 * to explore different camera vision modes, motor control, and LED settings
 * with full feedback through the serial monitor.
 *
 * The camera module is an i2c device.  It must be plugged
 * into port 3, 4, 5 or 9.  It will not work in any other ports.
 *
 * Features:
 * - Multiple vision modes: face detection, object detection, color detection, etc.
 * - Motor control with speed adjustment
 * - RGB LED color setting
 * - Camera LED control
 * - I2C device scanning and communication testing
 * - Detailed status reports
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * Commands:
 * exonaut robot;                          //This command creates the main robot instance
 *                                         //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                   //This command creates an AI Camera object called 'camera'
 *                                         //This is the object that handles vision detection functions
 *
 * camera.begin();                         //This command initializes the camera module and sets up I2C communication
 *
 * camera.currentFunc();                   //This command gets the current application mode that the camera is operating in
 *
 * camera.changeFunc(mode);                //This command changes the camera's operating mode
 *                                         //Parameters: mode number (APPLICATION_FACEDETECT, APPLICATION_OBJDETECT, etc.)
 *
 * camera.updateResult();                  //This command updates all recognition results from the camera
 *                                         //Should be called before checking detection results
 *
 * camera.setLed(state);                   //This command turns the camera's LED on or off
 *                                         //Parameter: boolean state (true for on, false for off)
 *
 * camera.numOfTotalFaceDetected();        //This command returns the total number of faces detected
 *
 * camera.getFaceOfIndex(index, &result);  //This command gets information about a face at a specific index
 *                                         //Parameters: face index, pointer to result structure
 *
 * camera.objIdDetected(id);               //This command checks if a specific object ID has been detected
 *                                         //Parameter: object ID constant (Person, Dog, Cat, etc.)
 *
 * camera.qrCodeDetected();                //This command checks if a QR code has been detected
 *
 * camera.anyLineDetected();               //This command checks if any line has been detected
 *
 * robot.set_motor_speed(left, right);     //This command sets both motor speeds
 *                                         //Parameters: left motor speed, right motor speed (-100 to 100)
 *
 * robot.setColorAll(r, g, b);             //This command sets all LEDs to the same color
 *                                         //Parameters: red, green, blue values (0-255)
 */

#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

// Define I2C pins for ESP32 (default is GPIO 21 for SDA, GPIO 22 for SCL)
#define SDA_PIN 21
#define SCL_PIN 22

// Create instances of our classes
exonaut robot;
ExoNaut_AICam camera;

// Define command buffer size
#define CMD_BUF_SIZE 64
char cmdBuffer[CMD_BUF_SIZE];
int cmdIndex = 0;

// Current camera mode
int currentMode = -1;

// Function declarations (prototypes)
void processCommand();
void printHelp();
void printCurrentMode();
void changeMode(String params);
void printStatus();
void setMotorSpeed(String params);
void setLedColor(String params);
void turnRobot(String params);
void detectObjects();
void resetEncoders();
void setCameraLed(String params);
void scanI2CDevices();
bool testCameraI2C();

void setup() {
  // Initialize Serial communication for commands
  Serial.begin(115200);
  
  // Wait for serial to connect
  delay(1000);
  
  Serial.println("\nInitializing...");
  
  // Initialize I2C with specific pins for ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000); // Set to standard 100kHz
  
  // Initialize the ExoNaut robot
  robot.begin();
  
  // Initialize the AI Camera
  camera.begin();
  
  // Scan for I2C devices to help with debugging
  scanI2CDevices();
  
  // Test camera communication
  testCameraI2C();
  
  // Get the current camera mode
  currentMode = camera.currentFunc();
  
  // Welcome message
  Serial.println("\n--------------------------------------");
  Serial.println("ExoNaut Robot Serial Control Interface");
  Serial.println("--------------------------------------");
  printHelp();
  printCurrentMode();
}

void loop() {
  // Check if serial data is available
  if (Serial.available() > 0) {
    // Read the incoming byte
    char inChar = Serial.read();
    
    // Process the character
    if (inChar == '\n') {
      // Null terminate the string
      cmdBuffer[cmdIndex] = '\0';
      
      // Process the command
      processCommand();
      
      // Reset the buffer index
      cmdIndex = 0;
    } else if (cmdIndex < CMD_BUF_SIZE - 1) {
      // Add character to buffer
      cmdBuffer[cmdIndex++] = inChar;
    }
  }
  
  // Update camera results periodically
  static unsigned long lastUpdateTime = 0;
  if (millis() - lastUpdateTime > 200) {  // Update every 200ms
    camera.updateResult();
    lastUpdateTime = millis();
  }
}

void processCommand() {
  // Convert command to lowercase
  String cmd = String(cmdBuffer);
  cmd.trim();
  cmd.toLowerCase();
  
  // Handle empty commands
  if (cmd.length() == 0) {
    return;
  }
  
  // Split command and parameters
  int spaceIndex = cmd.indexOf(' ');
  String command = (spaceIndex == -1) ? cmd : cmd.substring(0, spaceIndex);
  String params = (spaceIndex == -1) ? "" : cmd.substring(spaceIndex + 1);
  
  // Process commands
  if (command == "help" || command == "h") {
    printHelp();
  } 
  else if (command == "mode") {
    changeMode(params);
  }
  else if (command == "status") {
    printStatus();
  }
  else if (command == "motor" || command == "m") {
    setMotorSpeed(params);
  }
  else if (command == "stop" || command == "s") {
    robot.set_motor_speed(0, 0);
    Serial.println("Motors stopped");
  }
  else if (command == "led") {
    setLedColor(params);
  }
  else if (command == "turn") {
    turnRobot(params);
  }
  else if (command == "detect") {
    detectObjects();
  }
  else if (command == "reset") {
    resetEncoders();
  }
  else if (command == "cam" || command == "camera") {
    setCameraLed(params);
  }
  else if (command == "scan") {
    scanI2CDevices();
  }
  else if (command == "testcam") {
    testCameraI2C();
  }
  else {
    Serial.println("Unknown command. Type 'help' for available commands.");
  }
}

void printHelp() {
  Serial.println("\nAvailable commands:");
  Serial.println("  help         - Show this help message");
  Serial.println("  mode [num]   - Change camera mode (1-9)");
  Serial.println("    1: Face Detection    2: Object Detection");
  Serial.println("    3: Classification    4: Feature Learning");
  Serial.println("    5: Color Detection   6: Line Following");
  Serial.println("    7: AprilTag          8: QR Code");
  Serial.println("    9: Barcode");
  Serial.println("  status       - Show current status");
  Serial.println("  motor [L] [R]- Set motor speeds (-100 to 100)");
  Serial.println("  stop         - Stop all motors");
  Serial.println("  led [R] [G] [B] - Set LED color (0-255)");
  Serial.println("  turn [speed] [angle] - Turn robot at speed (deg/sec) for angle (deg)");
  Serial.println("  detect       - Run detection based on current mode");
  Serial.println("  reset        - Reset encoder counters");
  Serial.println("  cam [on/off] - Turn camera LED on/off");
  Serial.println("  scan         - Scan for I2C devices");
  Serial.println("  testcam      - Test camera I2C communication");
  Serial.println("");
}

void printCurrentMode() {
  Serial.print("Current camera mode: ");
  switch (currentMode) {
    case APPLICATION_FACEDETECT:      Serial.println("Face Detection"); break;
    case APPLICATION_OBJDETECT:       Serial.println("Object Detection"); break;
    case APPLICATION_CLASSIFICATION:  Serial.println("Classification"); break;
    case APPLICATION_FEATURELEARNING: Serial.println("Feature Learning"); break;
    case APPLICATION_COLORDETECT:     Serial.println("Color Detection"); break;
    case APPLICATION_LINEFOLLOW:      Serial.println("Line Following"); break;
    case APPLICATION_APRILTAG:        Serial.println("AprilTag"); break;
    case APPLICATION_QRCODE:          Serial.println("QR Code"); break;
    case APPLICATION_BARCODE:         Serial.println("Barcode"); break;
    case APPLICATION_NONE:            Serial.println("None"); break;
    default:                          Serial.println("Unknown"); break;
  }
}

void changeMode(String params) {
  int mode = params.toInt();
  
  if (mode >= APPLICATION_NONE && mode < APPLICATION_MAX) {
    Serial.print("Changing camera mode to: ");
    
    // Print the mode name
    switch (mode) {
      case APPLICATION_FACEDETECT:      Serial.println("Face Detection"); break;
      case APPLICATION_OBJDETECT:       Serial.println("Object Detection"); break;
      case APPLICATION_CLASSIFICATION:  Serial.println("Classification"); break;
      case APPLICATION_FEATURELEARNING: Serial.println("Feature Learning"); break;
      case APPLICATION_COLORDETECT:     Serial.println("Color Detection"); break;
      case APPLICATION_LINEFOLLOW:      Serial.println("Line Following"); break;
      case APPLICATION_APRILTAG:        Serial.println("AprilTag"); break;
      case APPLICATION_QRCODE:          Serial.println("QR Code"); break;
      case APPLICATION_BARCODE:         Serial.println("Barcode"); break;
      case APPLICATION_NONE:            Serial.println("None"); break;
    }
    
    // Change the mode
    if (camera.changeFunc(mode)) {
      currentMode = mode;
      Serial.println("Mode changed successfully");
    } else {
      Serial.println("Failed to change mode");
    }
  } else {
    Serial.println("Invalid mode. Choose a number between 0-9.");
  }
}

void printStatus() {
  // Print current mode
  printCurrentMode();
  
  // Print motor speeds
  float speeds[2];
  robot.encoder_motor_get_speed(speeds);
  Serial.print("Motor speeds: L=");
  Serial.print(speeds[0]);
  Serial.print(" R=");
  Serial.println(speeds[1]);
  
  // Print encoder counts
  float counts[2];
  robot.get_encoder_count(counts);
  Serial.print("Encoder counts: L=");
  Serial.print(counts[0]);
  Serial.print(" R=");
  Serial.println(counts[1]);
  
  // Print detection results based on current mode
  detectObjects();
}

void setMotorSpeed(String params) {
  // Parse the left and right motor speeds
  int spaceIndex = params.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println("Invalid command format. Use 'motor [L] [R]'");
    return;
  }
  
  float leftSpeed = params.substring(0, spaceIndex).toFloat();
  float rightSpeed = params.substring(spaceIndex + 1).toFloat();
  
  // Limit speeds to valid range
  leftSpeed = constrain(leftSpeed, -100, 100);
  rightSpeed = constrain(rightSpeed, -100, 100);
  
  // Set motor speeds
  robot.set_motor_speed(leftSpeed, rightSpeed);
  
  Serial.print("Motors set to L=");
  Serial.print(leftSpeed);
  Serial.print(" R=");
  Serial.println(rightSpeed);
}

void setLedColor(String params) {
  // Parse the RGB values
  int firstSpace = params.indexOf(' ');
  int lastSpace = params.lastIndexOf(' ');
  
  if (firstSpace == -1 || lastSpace == -1 || firstSpace == lastSpace) {
    Serial.println("Invalid command format. Use 'led [R] [G] [B]'");
    return;
  }
  
  int r = params.substring(0, firstSpace).toInt();
  int g = params.substring(firstSpace + 1, lastSpace).toInt();
  int b = params.substring(lastSpace + 1).toInt();
  
  // Limit RGB values to valid range
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);
  
  // Set LED color for all LEDs
  robot.setColorAll(r, g, b);
  
  Serial.print("LEDs set to R=");
  Serial.print(r);
  Serial.print(" G=");
  Serial.print(g);
  Serial.print(" B=");
  Serial.println(b);
}

void turnRobot(String params) {
  // Parse the speed and angle
  int spaceIndex = params.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println("Invalid command format. Use 'turn [speed] [angle]'");
    return;
  }
  
  float speed = params.substring(0, spaceIndex).toFloat();
  float angle = params.substring(spaceIndex + 1).toFloat();
  
  // Turn the robot
  Serial.print("Turning at ");
  Serial.print(speed);
  Serial.print(" deg/s for ");
  Serial.print(angle);
  Serial.println(" degrees");
  
  robot.encoder_motor_turn(speed, angle);
  
  Serial.println("Turn complete");
}

void detectObjects() {
  // Update camera results
  camera.updateResult();
  
  // Process results based on current mode
  switch (currentMode) {
    case APPLICATION_FACEDETECT: {
      int faces = camera.numOfTotalFaceDetected();
      Serial.print("Detected ");
      Serial.print(faces);
      Serial.println(" faces");
      
      // Display position of each face
      for (int i = 0; i < faces; i++) {
        WonderCamFaceDetectResult face;
        if (camera.getFaceOfIndex(i, &face)) {
          Serial.print("  Face ");
          Serial.print(i+1);
          Serial.print(": X=");
          Serial.print(face.x);
          Serial.print(" Y=");
          Serial.print(face.y);
          Serial.print(" W=");
          Serial.print(face.w);
          Serial.print(" H=");
          Serial.println(face.h);
        }
      }
      break;
    }
    
    case APPLICATION_OBJDETECT: {
      int objects = camera.numOfObjDetected();
      Serial.print("Detected ");
      Serial.print(objects);
      Serial.println(" objects");
      
      // Check for specific objects of interest
      if (camera.objIdDetected(Person)) {
        Serial.println("  Person detected!");
        WonderCamObjDetectResult obj;
        if (camera.objDetected(Person, 0, &obj)) {
          Serial.print("    Person at X=");
          Serial.print(obj.x);
          Serial.print(" Y=");
          Serial.print(obj.y);
          Serial.print(" W=");
          Serial.print(obj.w);
          Serial.print(" H=");
          Serial.println(obj.h);
        }
      }
      break;
    }
    
    case APPLICATION_COLORDETECT: {
      int colors = camera.numOfColorDetected();
      Serial.print("Detected ");
      Serial.print(colors);
      Serial.println(" colors");
      
      // Display detected color IDs
      for (int i = 1; i <= 7; i++) {
        if (camera.colorIdDetected(i)) {
          WonderCamColorDetectResult color;
          if (camera.colorId(i, &color)) {
            Serial.print("  Color ");
            Serial.print(i);
            Serial.print(": X=");
            Serial.print(color.x);
            Serial.print(" Y=");
            Serial.print(color.y);
            Serial.print(" W=");
            Serial.print(color.w);
            Serial.print(" H=");
            Serial.println(color.h);
          }
        }
      }
      break;
    }
    
    case APPLICATION_LINEFOLLOW: {
      if (camera.anyLineDetected()) {
        int lines = camera.numOfLineDetected();
        Serial.print("Detected ");
        Serial.print(lines);
        Serial.println(" lines");
        
        // Display line information
        for (int i = 1; i <= lines; i++) {
          if (camera.lineIdDetected(i)) {
            WonderCamLineResult line;
            if (camera.lineId(i, &line)) {
              Serial.print("  Line ");
              Serial.print(i);
              Serial.print(": Angle=");
              Serial.print(line.angle);
              Serial.print(" Offset=");
              Serial.println(line.offset);
            }
          }
        }
      } else {
        Serial.println("No lines detected");
      }
      break;
    }
    
    case APPLICATION_APRILTAG: {
      int tags = camera.numOfTotalTagDetected();
      Serial.print("Detected ");
      Serial.print(tags);
      Serial.println(" AprilTags");
      
      // Display tag IDs
      for (int i = 0; i < tags; i++) {
        // Note: This part is simplified as we'd need to know the actual tag IDs
        Serial.print("  AprilTag ");
        Serial.println(i+1);
      }
      break;
    }
    
    case APPLICATION_QRCODE: {
      if (camera.qrCodeDetected()) {
        Serial.println("QR Code detected");
        int length = camera.qrCodeDataLength();
        if (length > 0) {
          uint8_t data[256]; // Buffer for QR code data
          camera.qrCodeData(data);
          Serial.print("  QR Code content: ");
          Serial.write(data, length);
          Serial.println();
        }
      } else {
        Serial.println("No QR Code detected");
      }
      break;
    }
    
    case APPLICATION_BARCODE: {
      if (camera.barCodeDetected()) {
        Serial.println("Barcode detected");
        int length = camera.barCodeDataLength();
        if (length > 0) {
          uint8_t data[256]; // Buffer for barcode data
          camera.barCodeData(data);
          Serial.print("  Barcode content: ");
          Serial.write(data, length);
          Serial.println();
        }
      } else {
        Serial.println("No Barcode detected");
      }
      break;
    }
    
    default:
      Serial.println("No detection available in current mode");
      break;
  }
}

void resetEncoders() {
  robot.reset_encoder_counter(0); // Reset both encoders
  Serial.println("Encoder counters reset");
}

void setCameraLed(String params) {
  params.trim();
  params.toLowerCase();
  
  if (params == "on") {
    camera.setLed(true);
    Serial.println("Camera LED turned ON");
  } else if (params == "off") {
    camera.setLed(false);
    Serial.println("Camera LED turned OFF");
  } else {
    Serial.println("Invalid command. Use 'cam on' or 'cam off'");
  }
}

// Function to scan for I2C devices
void scanI2CDevices() {
  byte error, address;
  int nDevices;
  
  Serial.println("Scanning for I2C devices...");
  
  nDevices = 0;
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");
      
      if (address == CAM_DEFAULT_I2C_ADDRESS) {
        Serial.println("  --> AI Camera found!");
      }
      
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }    
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.print("Found ");
    Serial.print(nDevices);
    Serial.println(" device(s)");
  }
}

// Function to test basic I2C communication with the camera
bool testCameraI2C() {
  char version[17] = {0};  // Buffer for version string (16 chars + null)
  
  Serial.println("Testing I2C communication with AI Camera...");
  
  // Try to read firmware version
  if (camera.firmwareVersion(version)) {
    Serial.print("Camera firmware version: ");
    Serial.println(version);
    return true;
  } else {
    Serial.println("Failed to read camera firmware version");
    return false;
  }
}

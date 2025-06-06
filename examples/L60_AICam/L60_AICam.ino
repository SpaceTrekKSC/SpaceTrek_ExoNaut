// These lines include the necessary libraries for the robot and its AI camera.
#include "ExoNaut.h"       // The main library for the ExoNaut robot.
#include "ExoNaut_AICam.h"   // The special library for controlling the AI Camera module.

// Here, we're creating objects to represent our robot and its camera.
// This is like giving them names so we can command them.
exonaut robot;
ExoNaut_AICam camera;

// This variable will keep track of which AI function the camera is currently using.
// It starts with APPLICATION_NONE, meaning no function is active yet.
int currentMode = APPLICATION_NONE;

// The setup() function runs only once when the robot first turns on.
void setup() {
  Serial.begin(115200); // Starts the communication so we can see messages on the computer.
  delay(2500);          //give the user time to open the serial monitor
  
  robot.begin();        // Wakes up and initializes the robot's main board.
  camera.begin();       // Wakes up and initializes the AI camera module.
  delay(500);           // A brief pause to let everything stabilize.
  
  Serial.println("ExoNaut AI Camera Demo!"); // Print a welcome message.
  printMenu();      // Call the function that displays the list of options.
}

// The loop() function runs continuously after setup() is complete.
void loop() {
  // Check if you have typed a command into the Serial Monitor on your computer.
  if (Serial.available() > 0) {
    char selection = Serial.read(); // Read the single character you typed.
    handleSelection(selection);     // Send the character to our function that handles choices.
    delay(500); // Wait a moment for the camera to switch modes.
  }

  camera.updateResult(); // Ask the camera to get the latest results of what it sees.
  displayDetection();    // Show the results on the computer screen.
  
  delay(2000); // Wait for 2 seconds before checking and displaying again.
}

// This function just prints the menu of choices to the computer screen.
void printMenu() {
  Serial.println("\nSelect a Mode:"); // \n creates a new line for spacing.
  Serial.println("1 = Color Detection");
  Serial.println("2 = Face Detection");
  Serial.println("3 = AprilTag Detection");
  Serial.println("4 = Line Following");
  Serial.println("5 = Number Recognition");
  Serial.println("6 = Landmark Recognition");
  Serial.println("7 = Image Classification");
  Serial.println("8 = Feature Learning");
  Serial.print("Enter 1-8: ");
}

// This function takes the character you typed and tells the camera to switch modes.
void handleSelection(char selection) {
  // The 'switch' statement checks your selection and sets the 'currentMode' variable.
  switch (selection) {
    case '1': currentMode = APPLICATION_COLORDETECT; break;
    case '2': currentMode = APPLICATION_FACEDETECT; break;
    case '3': currentMode = APPLICATION_APRILTAG; break;
    case '4': currentMode = APPLICATION_LINEFOLLOW; break;
    case '5': currentMode = APPLICATION_NUMBER_REC; break;
    case '6': currentMode = APPLICATION_LANDMARK; break;
    case '7': currentMode = APPLICATION_CLASSIFICATION; break;
    case '8': currentMode = APPLICATION_FEATURELEARNING; break;
    default: // If you pick anything else...
      Serial.println();
      Serial.println("Invalid choice! Try again.");
      printMenu(); // Show the menu again.
      return;      // Exit the function.
  }
  
  // Now we use the changeFunc() from the library to tell the camera hardware to switch its active program.
  if (camera.changeFunc(currentMode)) {
    Serial.println("Mode changed successfully!");
  } else {
    Serial.println("Failed to change mode!");
  }
}

// This function checks the current mode and displays the correct information.
void displayDetection() {
  switch (currentMode) {
    case APPLICATION_COLORDETECT: {
      // The anyColorDetected() function checks if the camera sees any pre-trained colors.
      if (camera.anyColorDetected()) {
        Serial.println("Color detected!");
      } else {
        Serial.println("No color detected.");
      }
      break;
    }

    case APPLICATION_FACEDETECT: {
      // The anyFaceDetected() function checks if the camera sees any human faces.
      if (camera.anyFaceDetected()) {
        Serial.println("Face detected!");
      } else {
        Serial.println("No face detected.");
      }
      break;
    }

    case APPLICATION_APRILTAG: {
      // AprilTags are special black and white squares, like simple QR codes.
      if (camera.anyTagDetected()) {
        Serial.println("AprilTag detected!");
      } else {
        Serial.println("No AprilTag detected.");
      }
      break;
    }

    case APPLICATION_LINEFOLLOW: {
      // The anyLineDetected() function checks for lines on the ground.
      if (camera.anyLineDetected()) {
        Serial.println("Line detected!");
      } else {
        Serial.println("No line detected.");
      }
      break;
    }

    case APPLICATION_NUMBER_REC: {
      // This gets the number that the camera is most sure it's seeing.
      int number = camera.numberWithMaxProb();
      if (number >= 0) { // If a number was confidently detected...
        Serial.print("Detected number: ");
        Serial.println(number);
      } else {
        Serial.println("No number detected.");
      }
      break;
    }

    case APPLICATION_LANDMARK: {
      // Checks for pre-trained landmarks.
      if (camera.anyLandmarkDetected()) {
        Serial.println("Landmark detected!");
      } else {
        Serial.println("No landmark detected.");
      }
      break;
    }

    case APPLICATION_CLASSIFICATION: {
      // This gets the Class ID of the object the camera thinks it sees (e.g., ID 1 might be "Aeroplane").
      int classId = camera.classIdOfMaxProb();
      if (classId > 0) { // If an object was classified...
        Serial.print("Detected Class ID: ");
        Serial.println(classId);
      } else {
        Serial.println("No object classified.");
      }
      break;
    }

    case APPLICATION_FEATURELEARNING: {
      // This is for recognizing custom objects you have taught the camera.
      int featureId = camera.featureIdOfMaxProb();
      if (featureId > 0) { // If a learned object is seen...
        Serial.print("Detected Feature ID: ");
        Serial.println(featureId);
      } else {
        Serial.println("No feature detected.");
      }
      break;
    }
  }
}
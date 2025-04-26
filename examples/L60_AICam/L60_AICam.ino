#include "ExoNaut.h"
#include "ExoNaut_AICam.h"

exonaut robot;
ExoNaut_AICam camera;

int currentMode = APPLICATION_NONE;

void setup() {
  Serial.begin(115200);
  delay(500);
  
  robot.begin();
  camera.begin();
  
  Serial.println("ExoNaut AI Camera Demo!");
  printMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char selection = Serial.read();
    handleSelection(selection);
    delay(500); // Wait a little after mode switch
  }

  camera.updateResult();
  displayDetection();
  
  delay(2000); // Show results every 2 seconds
}

void printMenu() {
  Serial.println("\nSelect a Mode:");
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

void handleSelection(char selection) {
  switch (selection) {
    case '1': currentMode = APPLICATION_COLORDETECT; break;
    case '2': currentMode = APPLICATION_FACEDETECT; break;
    case '3': currentMode = APPLICATION_APRILTAG; break;
    case '4': currentMode = APPLICATION_LINEFOLLOW; break;
    case '5': currentMode = APPLICATION_NUMBER_REC; break;
    case '6': currentMode = APPLICATION_LANDMARK; break;
    case '7': currentMode = APPLICATION_CLASSIFICATION; break;
    case '8': currentMode = APPLICATION_FEATURELEARNING; break;
    default: 
      Serial.println("Invalid choice! Try again.");
      printMenu();
      return;
  }
  
  if (camera.changeFunc(currentMode)) {
    Serial.println("Mode changed successfully!");
  } else {
    Serial.println("Failed to change mode!");
  }
}

void displayDetection() {
  switch (currentMode) {
    case APPLICATION_COLORDETECT: {
      if (camera.anyColorDetected()) {
        Serial.println("Color detected!");
      } else {
        Serial.println("No color detected.");
      }
      break;
    }

    case APPLICATION_FACEDETECT: {
      if (camera.anyFaceDetected()) {
        Serial.println("Face detected!");
      } else {
        Serial.println("No face detected.");
      }
      break;
    }

    case APPLICATION_APRILTAG: {
      if (camera.anyTagDetected()) {
        Serial.println("AprilTag detected!");
      } else {
        Serial.println("No AprilTag detected.");
      }
      break;
    }

    case APPLICATION_LINEFOLLOW: {
      if (camera.anyLineDetected()) {
        Serial.println("Line detected!");
      } else {
        Serial.println("No line detected.");
      }
      break;
    }

    case APPLICATION_NUMBER_REC: {
      int number = camera.numberWithMaxProb();
      if (number >= 0) {
        Serial.print("Detected number: ");
        Serial.println(number);
      } else {
        Serial.println("No number detected.");
      }
      break;
    }

    case APPLICATION_LANDMARK: {
      if (camera.anyLandmarkDetected()) {
        Serial.println("Landmark detected!");
      } else {
        Serial.println("No landmark detected.");
      }
      break;
    }

    case APPLICATION_CLASSIFICATION: {
      int classId = camera.classIdOfMaxProb();
      if (classId > 0) {
        Serial.print("Detected Class ID: ");
        Serial.println(classId);
      } else {
        Serial.println("No object classified.");
      }
      break;
    }

    case APPLICATION_FEATURELEARNING: {
      int featureId = camera.featureIdOfMaxProb();
      if (featureId > 0) {
        Serial.print("Detected Feature ID: ");
        Serial.println(featureId);
      } else {
        Serial.println("No feature detected.");
      }
      break;
    }
  }
}


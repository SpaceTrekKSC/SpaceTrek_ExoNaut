/**************************************************
 * L02_Buttons.ino
 * An example program to demonstrate how to use the A and B
 * buttons on the CoreX controller.
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 *
 * Commands:
 * exonaut robot;                           //This command sets up the exonaut robot object.  exonaut is the class and robot is the name of the object.
 *                                          //This is the object that handles all of the motor commands and the features on the CoreX controller.
 * 
 * robot.begin();                           //This command initializes or begins the things in the robot object.
 *                                          //It is used once at the beginning of the program
 *
**************************************************/

#include <ExoNaut.h>                            //include the main ExoNaut library

exonaut robot;                                  //define the robot object

void setup(){                                   //the setup() function runs a single time
  robot.begin();                                //start the robot object
  Serial.begin(115200);                         //start the Serial communication with the robot at 115200 baud
  delay(1500);                                  //wait 1.5 seconds
}

void loop(){                                    //the loop() function runs forever in a loop
  if(digitalRead(BUTTON_A_PIN) == LOW){         //check if button A is pressed (LOW)
    Serial.println("Button A was pressed");     //tell the user that button A has been pressed
    while(digitalRead(BUTTON_A_PIN) == LOW);    //wait for the button to be released
  }
  if(digitalRead(BUTTON_B_PIN) == HIGH){        //check if button B is pressed (HIGH)
    Serial.println("Button B was pressed");     //tell the user that button B has been pressed
    while(digitalRead(BUTTON_B_PIN) == HIGH);   //wait for the button to be released
  }
}

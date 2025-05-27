/**************************************************
 * L03_Buttons_Interrupt.ino
 * An example program to demonstrate how to use the A and B
 * buttons on the CoreX controller with hardware interrupts.
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 * 
 * 
 * If your program has a lot of things going on it is possible a user
 * can press a button and let it go before the program has a chance to
 * check the pin state.  A more reliable way to capture button presses
 * is to use a hardware interrupt.  An interrupt will make the processor
 * stop what it is doing and go deal with the interrupt; it interrupts
 * the regular flow of the program. This lesson demonstrates using an
 * interrupt to capture button presses.
 *
 *
 * Commands:
 * exonaut robot;                           //This command sets up the exonaut robot object.  exonaut is the class and robot is the name of the object.
 *                                          //This is the object that handles all of the motor commands and the features on the CoreX controller.
 * 
 * robot.begin();                           //This command initializes or begins the things in the robot object.
 *                                          //It is used once at the beginning of the program
 *
**************************************************/

#include <ExoNaut.h>                                    //include the main ExoNaut library

volatile bool buttonA = false;                          //A variable to store if the A button has been pressed or not  
volatile bool buttonB = false;                          //A variable to store if the B button has been pressed or not 
                                                        //They are volatile because they're accessed from within an interrupt service routine

exonaut robot;                                          //define the robot object

void setup(){                                           //the setup() function runs a single time
  robot.begin();                                        //start the robot object
  Serial.begin(115200);                                   //start the Serial communication with the robot at 9600 baud
  attachInterrupt(BUTTON_A_PIN, isrA, FALLING);         //attach button A to an interrupt routine.  Button A has a LOW voltage when pressed
  attachInterrupt(BUTTON_B_PIN, isrB, RISING);          //attach button B to an interrupt routine.  Button B has a HIGH voltage when pressed
  delay(1500);                                          //wait 1.5 seconds
}

void loop(){                                            //the loop() function runs forever in a loop
  if(buttonA){                                          //if buttonA is true the button has been pressed
    Serial.println("Button A was pressed");             //tell the user that button A has been pressed
    buttonA = false;                                    //don't forget to set buttonA back to false
  }

  if(buttonB){                                          //if buttonA is true the button has been pressed
    Serial.println("Button B was pressed");             //tell the user that button B has been pressed
    buttonB = false;                                    //don't forget to set buttonB back to false
  }
}

void isrA(){                                            //the interrupt service routine for button A
  if(digitalRead(BUTTON_A_PIN) == LOW){                 //confirm that button A is LOW to reduce button bounce
    buttonA = true;                                     //if button A is LOW, then set buttonA to true
  }
}

void isrB(){                                            //the interrupt service routine for button B
  if(digitalRead(BUTTON_B_PIN) == HIGH){                //confirm that button B is HIGH to reduce button bounce
    buttonB = true;                                     //if button B is HIGH, then set buttonB to true
  }
}
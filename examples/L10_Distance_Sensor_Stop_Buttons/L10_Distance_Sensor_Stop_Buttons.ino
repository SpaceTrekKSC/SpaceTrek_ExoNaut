/**************************************************
 * L10_Distance_Sensor_Stop_Buttons.ino
 * An example program to stop the robot when it gets too close to an object
 * with the addition of using the A and B buttons to control if the robot
 * is allowed to move or not.
 *
 * Press the A button to make the robot start moving
 * Press the B button to make the robot stop moving
 * When moving, if the robot gets within STOP_DISTANCE of an object it will stop
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 * 
 * 
 * Commands:
 * exonaut robot;                           //This command sets up the exonaut robot object.  exonaut is the class and robot is the name given to the object
 *                                          //This is the object that handles all of the motor commands and the features on the CoreX controller.
 * 
 * robot.begin();                           //This command initializes or begins the things in the robot object.
 *                                          //It is used once at the beginning of the program
 * 
 * robot.set_motor_speed(left, right);      //This command sets both motor speeds.  It takes integer numbers for the left and right parameters.
 *                                          //Positive numbers will make the motor move forward, negative numbers will make it move backwards.
 * 
 * robot.stop_motor(motorID);               //This command stops the motors.
 *                                          //MotorID 0 is both motors, motorID 1 is the left, and motorID 2 is the right.
 *
 * ultrasonic us;                           //This command sets up the ultrasonic us object. ultrasonic is the class and us is the name given to the object
 *                                          //This is the object that handles all of the ultrasonic distance sensor features
 *
 * us.getDistance()                         //This command returns the distance from an object detected by the sensor in mm
 *
**************************************************/

#include <ExoNaut.h>                                //include the ExoNaut library
#include <ExoNaut_Ultrasonic.h>                     //include the ultrasonic distance sensor library

#define STOP_DISTANCE     100                       //the distance in mm to stop the robot

volatile bool buttonA = false;                      //A variable to store if the A button has been pressed or not  
                                                    //It is volatile because it is accessed from within an interrupt service routine
volatile bool buttonB = false;                      //A variable to store if the B button has been pressed or not

exonaut robot;                                      //declare the exonaut object named robot
ultrasonic us;                                      //declare the ultrasonic object name us

bool move = false;                                  //a variable to store if the robot should be moving or not

void setup(){                                       //the setup() function runs once
  attachInterrupt(BUTTON_A_PIN, isrA, FALLING);     //attach button A to an interrupt routine.  Button A has a LOW voltage when pressed
  attachInterrupt(BUTTON_B_PIN, isrB, RISING);      //attach button B to an interrupt routine.  Button B has a HIGH voltage when pressed
  robot.begin();                                    //start the robot object
  robot.stop_motor(0);                              //stops both motors
  delay(1000);                                      //wait 1.0 seconds
}                                                   //end of the setup() function

void loop(){                                        //the loop() function runs forever in a loop
  if(buttonA){                                      //if buttonA is true the button has been pressed
    buttonA = false;                                //make sure you set buttonA back to false
    move = true;                                    //set move to true so the robot will move
  }

  if(buttonB){                                      //if buttonB is true the button has been pressed
    buttonB = false;                                //make sure you set buttonA back to false
    move = false;                                   //set move to false so the robot stop moving
  }

  if(us.getDistance() <= STOP_DISTANCE){            //if the distance sensor detected an object closer than STOP_DISTANCE
    move = false;                                   //then set the move variable to false so the robot stops moving
  }

  if(move){                                         //if the move variable is true
    robot.set_motor_speed(30, 30);                  //set both motors to turn at 30 rpm
  }
  else{                                             //otherwise
    robot.stop_motor(0);                            //stop both motors
  }
}


//The below two Interrupt Service Routines are used to toggle 
//a variable to keep track if the buttons have been pressed
//
void isrA(){                                        //the interrupt service routine for button A
  if(digitalRead(BUTTON_A_PIN) == LOW){             //confirm that button A is LOW to reduce button bounce
    buttonA = true;                                 //if button A is LOW, then set buttonA to true
  }
}

void isrB(){                                        //the interrupt service routine for button B
  if(digitalRead(BUTTON_B_PIN) == HIGH){            //confirm that button B is HIGH to reduce button bounce
    buttonB = true;                                 //if button B is HIGH, then set buttonB to true
  }
}

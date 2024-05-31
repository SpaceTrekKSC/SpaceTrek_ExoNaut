/**************************************************
 * L07_Turn_Right.ino
 * An example program to make the ExoNaut robot turn right for one second
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 * 
 * 
 * Commands:
 * exonaut robot;                           //This command sets up the exonaut robot object.  exonaut is the class and robot is the name of the object.
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
**************************************************/

#include <ExoNaut.h>                          //include the ExoNaut library

exonaut robot;                                //declare the exonaut object named robot

void setup() {                                //the setup() function runs once
  robot.begin();                              //start the robot object
  delay(1500);                                //1.5 seconds
  robot.set_motor_speed(50, 25);              //set the speed of left motor to 50 and right motor to 35
                                              //To turn left make the left motor speed less than the right motor speed
  delay(2000);                                //wait for two seconds
  
  robot.stop_motor(0);                        //stops both motors
}                                             //end of the setup() function

void loop() {
  // Nothing in loop()
}
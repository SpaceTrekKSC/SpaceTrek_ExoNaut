/**************************************************
 * L05_Go_Straight.ino
 * An example program to make the ExoNaut robot drive straight for 1 second
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
 *                                          //The minimum value is 0, the maximum value should be kept below 150.
 *                                          //The speed unit is an ExoDash.  It is a proprietary speed unit used by the ExoNauts
 *                                          //and they don't want to share with the humans what it is.  But it can be converted
 *                                          //to something useful like meters per second with a bit of experimentation. 
 * 
 * robot.stop_motor(motorID);               //This command stops the motors.
 *                                          //MotorID 0 is both motors, motorID 1 is the left, and motorID 2 is the right.
 * 
**************************************************/

#include <ExoNaut.h>                          //include the ExoNaut library

exonaut robot;                                //declare the exonaut object named robot
int speed = 50;                               //declare and initialize a variable speed set to the value 50

void setup() {                                //the setup() function runs once
  robot.begin();                              //start the robot object
  robot.set_motor_speed(speed, speed);        //set the speed of both motors to the value of the speed variable
  delay(1000);                                //wait for 1 second
  robot.stop_motor(0);                        //stop both motors
}                                             //end of the setup() function

void loop() {
  // Nothing in loop()
}
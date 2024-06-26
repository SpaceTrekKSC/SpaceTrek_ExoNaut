/**************************************************
 * L26_Move_Block.ino
 * An example program to drive up to a block, grip it with the gripper, and then return to the starting location
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: June 20th, 2024

 * 
**************************************************/

#include <ExoNaut.h>                          //include the ExoNaut library

exonaut robot;                                //declare the exonaut object named robot
int speed = 50;                               //declare and initialize a variable speed set to the value 50

void setup() {                                //the setup() function runs once
  delay(5000);
  robot.begin();                              //start the robot object
  delay(1500);

  robot.bus_servo_set_pose(1,200,1000);
  delay(2000);

  robot.set_motor_speed(speed, speed);        //set the speed of both motors to the value of the speed variable
  delay(1000);                                //wait for 1 second
  robot.stop_motor(0);                        //stop both motors
}                                             //end of the setup() function

void loop() {
  // Nothing in loop()
}
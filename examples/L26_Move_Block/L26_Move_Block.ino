/**************************************************
 * L26_Move_Block.ino
 * An example program to move a foam block
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: June 26th, 2024
 *
 * 
**************************************************/

#include <ExoNaut.h>                          //include the ExoNaut library

exonaut robot;                                //declare the exonaut object named robot
int speed = 40;                               //initialize a variable to use as the speed

void setup() {                                //the setup() function runs once
  robot.begin();                              //start the robot object
  robot.beginBusServo();                      //start the bus servos

  robot.bus_servo_set_pose(1,200,100);        //open the gripper
  delay(500);                                 //wait half a second

  robot.setColorAll(150, 0, 0);               //set all LEDs to red
  robot.set_motor_speed(speed, speed);        //set the speed of both motors to the value of the speed variable
  delay(1000);                                //wait for 1 second (move forward to the block)
  robot.stop_motor(0);                        //stop both motors
  delay(500);                                 //wait half a second

  robot.setColorAll(0, 150, 0);               //set all LEDs to green
  robot.bus_servo_set_pose(1,450,100);        //close the gripper on the block
  delay(500);                                 //wait half a second

  robot.setColorAll(0, 0, 150);               //set all LEDs to blue
  robot.set_motor_speed(-speed, -speed);      //set the motors to move in reverse
  delay(1000);                                //wait for 1 second (move back with the block)
  robot.stop_motor(0);                        //stop both motors
  delay(500);                                 //wait half a second

  robot.bus_servo_set_pose(1,200,100);        //open the gripper
  delay(500);                                 //wait half a second

  robot.set_motor_speed(-speed, -speed);      //set the motors to move in reverse
  delay(350);                                 //wait for 1 second (move away from the block)
  robot.stop_motor(0);                        //stop both motors
  delay(150);

  robot.set_motor_speed(speed, -speed);       //set the motors to pivot (turn about 180 degrees)
  delay(775);                                 //wait 0.775 seconds
  robot.stop_motor(0);                        //stop both motors
  delay(250);

  robot.set_motor_speed(speed, speed);        //set the motors to move in reverse
  delay(500);                                 //wait for 0.5 seconds (move away from the block)
  robot.stop_motor(0);                        //stop both motors

  robot.setColorAll(0, 0, 0);                 //set all LEDs to OFF
}                                             //end of the setup() function

void loop() {
  //nothing in lopp()
}
/**************************************************
 * L12_Line_Follower_Basic.ino
 * An example program to make the ExoNaut follow a line
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
 * lineFollower lf;                         //This command sets up the lineFollower lf object. lineFollower is the class and lf is the name given to the object
 *                                          //This is the object that handles all of the line follower commands
 *
 * lf.readLineFollower(lineData);           //reads the line follower sensor and stores the data in lineData
 *
 * LF_CH1(lineData)                         //returns a 0 or 1 based on the state of the line follower channel 
 *                                          //because the line is black a 0 indicates when the sensor is over the line
 *                                          //remember, a 0 is false and anything not zero, such as a 1, is true
 *
 *
 *
 * Line follower sensor numbers (viewed from top)
 * 
 *     __1__2__3__4___
 *     |             |
 *     |             |
 *     |             |
 *   |||             |||
 *   |||             |||
 *   |||_____________|||
 * 
 * 
**************************************************/

#include <ExoNaut.h>                          //include the ExoNaut library
#include <ExoNaut_LineFollower.h>             //include the ExoNaut Line Follower library

exonaut robot;                                //declare the exonaut object named robot
lineFollower lf;                              //declare the lineFollower object names lf

uint8_t lineData = 0;                         //declare a variable to store the line follower data

void setup() {                                //the setup() function runs once
  robot.begin();                              //start the robot object
  robot.stop_motor(0);                        //stops both motors
  delay(1000);                                //wait 1.0 seconds
}                                             //end of the setup() function

void loop() {
  lf.readLineFollower(lineData);
  if(LF_CH1(lineData) && LF_CH2(lineData) && LF_CH3(lineData) && LF_CH4(lineData)){               //if all of the sensors detect a line
    robot.set_motor_speed(0, 0);                                                                  //set the speed of both motors to zero (i.e. stop)
  }
  else if(!LF_CH1(lineData) && !LF_CH2(lineData) && !LF_CH3(lineData) && !LF_CH4(lineData)){      //if all of the sensors do not detect a line
    robot.set_motor_speed(0, 0);                                                                  //set the speed of both motors to zero (i.e. stop)
  }
  else if(!LF_CH1(lineData) && LF_CH2(lineData) && LF_CH3(lineData) && !LF_CH4(lineData)){        //if only the center two sensors detect a line the robot is centered over the line
    robot.set_motor_speed(40, 40);                                                                //so make it drive in a straight line
  }
  else if(!LF_CH1(lineData) && LF_CH2(lineData) && !LF_CH3(lineData) && !LF_CH4(lineData)){       //if only one sensor detects the line then the robot must not be centered over the line
    robot.set_motor_speed(6, 40);                                                                 //and it needs to turn to get back over the line
  }
  else if(!LF_CH1(lineData) && !LF_CH2(lineData) && LF_CH3(lineData) && !LF_CH4(lineData)){       //if only one sensor detects the line then the robot must not be centered over the line
    robot.set_motor_speed(40, 6);                                                                 //and it needs to turn to get back over the line
  }
  delay(10);                                                                                      //slow things down a little bit so the line follower can keep up with sending data
}
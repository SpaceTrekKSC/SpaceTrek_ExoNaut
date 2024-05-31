/**************************************************
 * L13_Line_Follower_Advanced.ino
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
lineFollower lf;

uint8_t lineData = 0;                         //a variable to store the line data in

void setup() {                                //the setup() function runs once
  robot.begin();                              //start the robot object
  robot.stop_motor(0);                        //stops both motors
  delay(1000);                                //wait 1.0 seconds
}                                             //end of the setup() function

//In this example we are using a switch/case statement to handle a lot more options than we
//had in the previous line follower example. For each of the cases we are looking at a binary
//form of the data.  Because we have not used the LF_CHX() functions, a 0 is white and an 1 
//is black.  In the 0bxxxx number, the x located on the far right is sensor 1 and the far left
//is sensor 4.

void loop() {
  lf.readLineFollower(lineData);              //read the line follower and store the data in lineData
  switch(lineData){
    case 0b0000:                              //all detect white
      robot.stop_motor(0);
      break;
    case 0b0001:                              //only sensor 1 detects black. Sensor 2, 3 and 4 detect white
      //what to do?
      break;
    case 0b0010:                              //only sensor 2 detects black. Sensor 1, 3 and 4 detect white
      robot.set_motor_speed(6, 40);
      break;
    case 0b0011:                              //Sensor 1 and 2 detect black. Sensor 3 and 4 detect white
      robot.set_motor_speed(-5, 40);
      break;
    case 0b0100:                              //only sensor 3 detects black. Sensor 1, 2 and 4 detect white
      robot.set_motor_speed(40, 6);
      break;
    case 0b0110:                              //Sensor 2 and 3 detect black. Sensor 1 and 4 detect white
      robot.set_motor_speed(40, 40);
      break;
    case 0b0111:                              //Sensor 1, 2 and 3 detect black. Only sensor 4 detects white
      //what to do?
      break;
    case 0b1000:                              //only sensor 4 detects black. Sensor 1, 2 and 3 detect white
      //what to do?
      break;
    case 0b1100:                              //Sensor 3 and 4 detect black. Sensor 1 and 2 detect white
      robot.set_motor_speed(40, -5);
      break;
    case 0b1110:                              //Sensor 2, 3 and 4 detect black. Only sensor 1 detects white
      //what to do?
      break;
    case 0b1111:                              //all detect black
      robot.stop_motor(0);
      break;
    case 0b0101:                              //Sensor 1 and 3 detect black. Sensor 2 and 4 detect white
      //what to do?
      break;
    case 0b1001:                              //Sensor 1 and 4 detect black. Sensor 2 and 3 detect white
      //what to do?
      break;
    case 0b1010:                              //Sensor 2 and 4 detect black. Sensor 1 and 3 detect white
      //what to do?
      break;
    case 0b1011:                              //Sensor 1, 2 and 4 detect black. Only sensor 3 detects white
      //what to do?
      break;
    case 0b1101:                              //Sensor 1, 3 and 4 detect black. Only sensor 2 detects white
      //what to do?
      break;
    default:                                  //unexpected value in lineData
      robot.stop_motor(0);            
      break;
  }
  delay(5);
}
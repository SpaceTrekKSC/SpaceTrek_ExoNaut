/**************************************************
 * L25_Bus_Servo_Control.ino
 * An example program to show how to control the serial bus servos included with the
 * ExoNaut robot.  Serial bus servos work by sending data to them with a serial connection.
 * One advantage of them is that multiple servos can be connected to the same "bus" or port.
 * Each bus servo has an ID and when it receives a command with its ID it responds.
 *
 * The gripper has serial motor ID 1 and has a range of motion from 0 to 550
 * The elbow has serial motor ID 2 and has a range of motion from 0 to 1000
 *
 * The bus servos must be plugged into port 10, they will not work on any other port
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: June 10th, 2024
 *
 *
 * Commands:
 * exonaut robot;                                   //This command sets up the exonaut robot object.  exonaut is the class and robot is the name of the object.
 *                                                  //This is the object that handles all of the motor commands and the features on the CoreX controller.
 * 
 * robot.begin();                                   //This command initializes or begins the things in the robot object.
 *                                                  //It is used once at the beginning of the program.
 *
 * robot.beginBusServo();                           //start the bus servo
 * 
 * robot.bus_servo_set_pose(ID, Position, Time);    //sets a servo position.  The ID is on the side of the servos
 *                                                  //the position can be between 0 and 1000 with 500 being in the middle
 *                                                  //the time in milliseconds is how long it will take the servo to make the move
 *                                                  //a longer time makes the servo move slower
 *
**************************************************/

#include <ExoNaut.h>                                    //include the main ExoNaut library

#define SERVO_ID    1                                   //define the servo ID

exonaut robot;                                          //define the robot object

int position = 500;                                     //initialize a variable names position to the value 500

void setup() {                                          //the setup() function runs a single time
  Serial.begin(9600);                                   //start the serial port
  robot.begin();                                        //start the robot object
  robot.beginBusServo();                                //start the bus servo
  delay(1500);                                          //wait 1.5 seconds
  robot.bus_servo_set_pose(SERVO_ID,position,1000);     //set the initial position of the servo to 500
  delay(500);                                           //wait 0.5 seconds
  Serial.println("Send a number between 0 and 1000 to change the position of the servo");     //Tell the user how to use the program
}

void loop() {                                           //the loop() function runs forever in a loop
  if(Serial.available()){                               //checks to see if any serial data has been received by the robot
    position = Serial.parseInt();                       //stores the first integer it finds into the variable position

    delay(5);                                           //a small delay to allow any remaining characters to arrive in the serial buffer
    while(Serial.available()){                          //if there are extra characters that were sent to the robot
      Serial.read();                                    //this while loop will read them out of the serial buffer so that
    }                                                   //they are not there the next time something is sent to the robot

    Serial.print("Setting servo position to:\t");       //Tell the user what the servo is being set to
    Serial.println(position);                           //outputs the stored position and a new line character

    robot.bus_servo_set_pose(SERVO_ID,position,250);    //sets the servo to the new position
  }
}

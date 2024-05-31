/**************************************************
 * L11_Distance_Sensor_LEDs.ino
 * An example program that shows how to change the LEDs on the distance sensor

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

exonaut robot;                                      //declare the exonaut object named robot
ultrasonic us;                                      //declare the ultrasonic object name us

void setup(){                                       //the setup() function runs once
  robot.begin();                                    //start the robot object
  robot.stop_motor(0);                              //stops both motors
  delay(1000);                                      //wait 1.0 seconds
}                                                   //end of the setup() function

void loop(){                                        //the loop() function runs forever in a loop
  if(us.getDistance() <= STOP_DISTANCE){            //if the distance sensor detected an object closer than STOP_DISTANCE
    robot.stop_motor(0);                            //stops both motors
  }
  else{                                             //otherwise
    robot.set_motor_speed(30, 30);                  //set both motors to turn at 30 rpm
  }
}





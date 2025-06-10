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
 *
 * ultrasonic us;                           //This command sets up the ultrasonic us object. ultrasonic is the class and us is the name given to the object
 *                                          //This is the object that handles all of the ultrasonic distance sensor features
 *
 * us.getDistance()                         //This command returns the distance from an object detected by the sensor in mm
 *
 * us.color(R1, G1, B1, R2, G2, B2);        //This command sets the color of the LEDs in the distance sensor using the RGB method.
 *                                          //If you are looking at the robot from the front LED 1 is on the left and 2 is on the right
 *
**************************************************/

#include <ExoNaut.h>                    //include the ExoNaut library
#include <ExoNaut_Ultrasonic.h>         //include the ultrasonic distance sensor library

exonaut robot;                          //declare the exonaut object named robot
ultrasonic us;                          //declare the ultrasonic object name us

void setup(){                           //the setup() function runs once
  robot.begin();                        //start the robot object
}                                       //end of the setup() function

void loop(){                            //the loop() function runs forever in a loop
  us.color(150, 0, 150, 150, 0, 150);   //set LED1 to r=150, g=0, b=150 and LED2 to r=150, g=0, b=150
  delay(1500);                          //wait 1.5 seconds
  us.color(150, 0, 0, 0, 150, 0);       //set LED1 to red=150, green=0, blue=0 and LED2 to red=0, green = 150, blue=0
  delay(1500);                          //wait 1.5 seconds
}


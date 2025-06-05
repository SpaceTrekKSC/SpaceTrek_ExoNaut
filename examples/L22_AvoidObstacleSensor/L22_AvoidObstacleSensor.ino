/**************************************************
 * L22_AvoidObstacleSensor.ino
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: March 5th, 2025
 * Updated: Use onboard LEDs instead of external RGB module
 *
 * An example program to demonstrate how to use the Avoid Obstacle module with the ExoNaut.
 * Attach the Avoid Obstacle module to port 8 (pin 26)
 * You must use the pin number, not the port number, for the Avoid Obstacle module
 *
 * The port to pin mapping is:
 *	___________________________
 *	 PORT Number |	Pin Number
 *        2      |      32
 *        6      |      33
 *        8      |      26
 *
 *
 * Commands for onboard LEDs:
 * robot.setColor(pixel-num, red, green, blue);     // Set the color of one onboard LED
 * robot.setColorAll(red, green, blue);             // Set all onboard LEDs to same color and display immediately
 * robot.show();                                    // Update LEDs with colors in memory
 * robot.clear();                                   // Turn off all onboard LEDs
 *
 * digitalRead(PIN_NUMBER);                         // digitalRead() is a native Arduino command.  It returns a HIGH/LOW (1/0) 
 *                                                  // value based on the state of the pin it reads.
 *
**************************************************/
#include <ExoNaut.h>

exonaut robot;                  //define an object for the robot

#define AO 26                   // define AO as 26 so that we can use AO instead of 26

void setup(){
  robot.begin();                // Start the robot object (this initializes the onboard LEDs too)
  pinMode(AO, INPUT);           // Set the digital pin AO to an input

  delay(1500);
}

void loop(){
  int AO_value = digitalRead(AO);   // Take a reading from the AO Sensor

  if (AO_value == 1){               //the sensor is NOT close to an object
    robot.setColor(0, 0, 200, 0);   //turn onboard pixel 0 green
    robot.setColor(1, 0, 0, 0);     //turn onboard pixel 1 off
  }
  else{                             //the sensor IS close to an object
    robot.setColor(0, 0, 0, 0);     //turn onboard pixel 0 off
    robot.setColor(1, 200, 0, 0);   //turn onboard pixel 1 red
  }
  robot.show();                     //update the onboard LEDs to show the new settings
  delay(1);                         //A short delay to slow things down a little
}
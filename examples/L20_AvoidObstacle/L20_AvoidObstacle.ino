/**************************************************
 * L20_AvoidObstacle.ino
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: March 5th, 2025
 *
 * An example program to demonstrate how to use the Avoid Obstacle module with the ExoNaut.
 * Make sure to attach the RGB LED module to port 6 and the Avoid Obstacle module to port 8 (pin 26)
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
 * Commands:
 * RGB rgb;                                         // Create the RGB object
 * rgb.begin(port);                                 // Initialize the RGB module on specified port (2, 6, or 8)
 * rgb.setColor(pixel-num, red, green, blue);       // Set the color of one LED (call rgb.show() to display)
 * rgb.setColorAll(red, green, blue);               // Set all LEDs to same color and display immediately
 * rgb.show();                                      // Update LEDs with colors in memory
 * rgb.clear();                                     // Turn off all LEDs
 *
 * digitalRead(PIN_NUMBER);                         // digitalRead() is a native Arduino command.  It returns a HIGH/LOW (1/0) 
 *                                                  // value based on the state of the pin it reads.
 *
**************************************************/
#include <ExoNaut.h>
#include <ExoNaut_RGB_LED.h>

exonaut robot;                  //define an object for the robot
RGB rgb;                        //define the object for the RGB LEDs module 

#define AO 26                   // define AO as 26 so that we can use AO instead of 26

void setup(){
  robot.begin();                // Start the robot object 
  rgb.begin(6);                 // Start the RGB LEDs on port 6
  pinMode(AO, INPUT);           // Set the digital pin AO to an input

  delay(1500);
}

void loop(){
  int AO_value = digitalRead(AO);   // Take a reading from the AO Sensor

  if (AO_value == 1){               //the sensor is NOT close to an object
    rgb.setColor(0, 0, 200, 0);     //turn pixel 0 green
    rgb.setColor(1, 0, 0, 0);       //turn pixel 1 off
  }
  else{                             //the sensor IS close to an object
    rgb.setColor(0, 0, 0, 0);       //turn pixel 0 off
    rgb.setColor(1, 200, 0, 0);     //turn pixel 1 red
  }
  rgb.show();                       //update the RGB LEDs to show the new settings
}

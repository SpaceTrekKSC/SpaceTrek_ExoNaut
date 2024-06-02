/**************************************************
 * L04_CoreX_LEDs.ino
 * An example program to demonstrate how to use the LEDs built into the CoreX controller.
 * The built in LEDs are RGB Neo Pixels.  These provide a full range of colors and are
 * convenient to use from Arduino.
 *
 * The RGB LEDs are NeoPixels.  Adafruit has a very good library for using them. If you would
 * like to experiment with features not supported by the ExoNaut library you can used the 
 * Adafruit NeoPixel library.  There are six LEDs and they are on pin 23.
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
 *                                          //It is used once at the beginning of the program.
 * 
 * robot.setColor(pixel-num, red, green, blue);     //This command sets the color of one of the RGB LEDs in memory
 *                                                  //You must call robot.show() to activate changes to the RGB colors.
 * 
 * robot.setColorAll(red, green, blue);     //This command sets the RGB values for all six LEDs and automatically 
 *                                          //calls the robot.show() command.
 *
 * robot.show();                            //This command makes the RGB LEDs show the colors set in their memory.
 *
 * robot.clear();                           //This command clears all RGB LEDs and turns them off.    
 *
**************************************************/

#include <ExoNaut.h>                              //include the main ExoNaut library

exonaut robot;                                    //define the robot object

void setup() {                                    //the setup() function runs a single time
  robot.begin();                                  //start the robot object
  delay(1500);                                    //wait 1.5 seconds
}

void loop() {                                     //the loop() function runs forever in a loop
  robot.setColorAll(150, 0, 0);                   //sets all six LEDs to RED at a brightness of 150 and shows it 
  delay(1000);                                    //wait for 1 second
  robot.setColorAll(0, 150, 0);                   //sets all six LEDs to GREEN at a brightness of 150 and shows it
  delay(1000);                                    //wait for 1 second
  robot.setColorAll(0, 0, 150);                   //sets all six LEDs to BLUE at a brightness of 150 and shows it
  delay(1000);                                    //wait for 1 second
  robot.clear();                                  //clears the LEDs and turns them off
  delay(1000);                                    //wait for 1 second

  for(int i = 0; i < NUM_PIXELS; i++){            //a for loop to iterate through 0 to 5 to work on each LED's color
    robot.setColor(i, 0, 0, 200);                 //sets pixel number i to BLUE at a brightness of 200
    robot.show();                                 //shows the change to the RGB values
    delay(500);                                   //waits for 0.5 seconds
  }
  delay(1000);                                    //waits for 1 second
  robot.clear();                                  //clears the LEDs
  delay(1000);                                    //waits for 1 second

  robot.setColor(0, 200, 0, 0);                   //these six commands set each LED individually
  robot.setColor(1, 0, 200, 0);                   //without showing them.
  robot.setColor(2, 0, 0, 200);
  robot.setColor(3, 200, 200, 0);
  robot.setColor(4, 200, 0, 200);
  robot.setColor(5, 0, 200, 200);
  robot.show();                                   //once all six have been set, show() makes them all change at the same time
  delay(2000);                                    //wait for 2 seconds
  robot.clear();                                  //clears the LEDs
  delay(1000);                                    //waits for 1 second

  for(int i = 0; i < NUM_PIXELS; i++){            //a for loop to change each LED
    robot.setColor(i, 200, 200, 200);
  }
  robot.show();                                   //shows the change to the RGB values  
  delay(2000);                                    //waits for 2 seconds

  robot.clear();                                  //clears the LEDs
  delay(1000);                                    //waits for 1 second
}

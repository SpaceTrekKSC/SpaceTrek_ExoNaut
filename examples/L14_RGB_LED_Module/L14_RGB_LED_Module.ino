/**************************************************
 * L14_RGB_LED_Module.ino
 * An example program to demonstrate how to use the LEDs built into the CoreX controller.
 * The built in LEDs are RGB Neo Pixels.  These provide a full range of colors and are
 * convenient to use from Arduino.
 *
 * The RGB LEDs are Neo Pixels.  Adafruit has a very good library for using them. If you would
 * like to experiment with features not supported by the ExoNaut library you can used the 
 * Adafruit NeoPixel library.  There are two LEDs and the port to pin mapping is given below.
 *	___________________________
 *	 PORT Number |	Pin Number
 *        2      |      32
 *        6      |      33
 *        8      |      26
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 *
 *
 * Commands:
 * RGB rgb;                                         //This command sets up the RGB rgb object.  RGB is the class and rgb is the name of the object.
 * 
 * rgb.begin();                                     //This command initializes or begins the things in the rgb object.
 *                                                  //It is used once at the beginning of the program.
 * 
 * rgb.setColor(pixel-num, red, green, blue);       //This command sets the color of one of the RGB LEDs in memory
 *                                                  //You must call rgb.show() to activate changes to the RGB colors.
 * 
 * rgb.setColorAll(reg, green, blue);               //This command sets the RGB values for both LEDs and automatically 
 *                                                  //calls the rgb.show() command.
 *
 * rgb.show();                                      //This command makes the RGB LEDs show the colors set in their memory.
 *
 * rgb.clear();                                     //This command clears all RGB LEDs and turns them off.    
 *
**************************************************/

#include <ExoNaut.h>                              //include the main ExoNaut library
#include <ExoNaut_RGB_LED.h>                      //

RGB rgb;                                          //define the rgb object

void setup(){                                     //the setup() function runs a single time
  rgb.begin(10);                                  //start the robot object
  delay(1500);                                    //wait 1.5 seconds
}

void loop(){                                      //the loop() function runs forever in a loop
  rgb.setColorAll(150, 0, 0);                     //sets both LEDs to RED at a brightness of 150 and shows it 
  delay(1000);                                    //wait for 1 second
  rgb.setColorAll(0, 150, 0);                     //sets both LEDs to RED at a brightness of 150 and shows it
  delay(1000);                                    //wait for 1 second
  rgb.setColorAll(0, 0, 150);                     //sets both LEDs to RED at a brightness of 150 and shows it
  delay(1000);                                    //wait for 1 second
  rgb.clear();                                    //clears the LEDs and turns them off
  delay(1000);                                    //wait for 1 second

  for(int i = 0; i < 2; i++){                     //a for loop to iterate through 0 to 5 to work on each LED's color
    rgb.setColor(i, 0, 0, 200);                   //sets pixel number i to GREEN at a brightness of 200
    rgb.show();                                   //shows the change to the RGB values
    delay(1000);                                  //waits for 0.5 seconds
  }
  delay(1000);                                    //waits for 1 second
  rgb.clear();                                    //clears the LEDs
  delay(1000);                                    //waits for 1 second

  rgb.setColor(0, 200, 0, 0);                     //these two commands set each LED individually
  rgb.setColor(1, 0, 200, 0);                     //without showing them.
  rgb.show();                                     //once both have been set, show() makes them all change at the same time
  delay(2000);                                    //wait for 2 seconds
  rgb.clear();                                    //clears the LEDs
  delay(1000);                                    //waits for 1 second

  for(int i = 0; i < 2; i++){                     //a for loop to change each LED
    rgb.setColor(i, 200, 200, 200);
  }
  rgb.show();                                     //shows the change to the RGB values  
  delay(2000);                                    //waits for 2 seconds

  rgb.clear();                                    //clears the LEDs
  delay(1000);                                    //waits for 1 second
}

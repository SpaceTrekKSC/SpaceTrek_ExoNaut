/**************************************************
 * L14_RGB_LED_Module.ino
 * An example program to demonstrate how to use the RGB LED module with the ExoNaut.
 * MAke sure to Attach the RGB LED module to port 6
 * The RGB LEDs are Neo Pixels. The module has two LEDs and the port to pin mapping is:
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
 * Commands:
 * RGB rgb;                                         // Create the RGB object
 * rgb.begin(port);                                 // Initialize the RGB module on specified port (2, 6, or 8)
 * rgb.setColor(pixel-num, red, green, blue);       // Set the color of one LED (call rgb.show() to display)
 * rgb.setColorAll(red, green, blue);               // Set all LEDs to same color and display immediately
 * rgb.show();                                      // Update LEDs with colors in memory
 * rgb.clear();                                     // Turn off all LEDs
 *
**************************************************/
#include <ExoNaut.h>
#include <ExoNaut_RGB_LED.h>

exonaut robot;
RGB rgb;

#define IR 26 // Port 8 Accessing the Avoid Obstacle Module

void setup()
{
Serial.begin(115200);
robot.begin(); 
rgb.begin(6); // Port Accessing the RGB Module
pinMode(IR, INPUT); 

delay(1500);
}

void loop()
{
int IR_value = digitalRead(IR); // Take in reading from the IR Sensor

// IR sensor is green if sensor isn't blocked
if (IR_value == 1){ 
  rgb.setColor(0, 0, 200, 0);
  rgb.setColor(1, 0, 0, 0);
}

// IR sensor is red when object is detected
else{
  rgb.setColor(1, 200, 0, 0);
  rgb.setColor(0, 0, 0, 0);
}
rgb.show();
}

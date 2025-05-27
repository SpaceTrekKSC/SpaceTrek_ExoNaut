/**************************************************
 * L01_Hello_ExoNaut.ino
 * An example program to demonstrate uploading a program to
 * the ExoNaut robot.  Ensure you have the board file esp32
 * by Espressif Systems installed.  And that you have selected
 * the ESP32 Dev Module as the board.
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
**************************************************/

#include <ExoNaut.h>                          //Include the ExoNaut library

void setup() {                                //The setup() function runs once
  Serial.begin(115200);                         //Setup the serial communication from the ExoNaut to the computer at 9600 baud
  delay(1000);                                //A one second delay to allow the program time to start
}                                             //End of the setup() function

void loop() {
  Serial.println("Hello ExoNaut!");           //Print the text "Hello ExoNaut!" to the serial monitor
  delay(1000);                                //A one second delay before repeating

}
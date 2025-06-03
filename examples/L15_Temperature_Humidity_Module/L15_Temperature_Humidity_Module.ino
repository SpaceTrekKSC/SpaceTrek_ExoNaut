/**************************************************
 * L15_Temperature_Humidity_Module.ino
 * An example program to demonstrate how to use the Temperature & Humidity sensor.
 * If you would like to look further into the sensor, it is the AHT10.
 *
 * Author: Andrew Gafford
 * Email: agafford@spacetrek.com
 * Date: May 20th, 2024
 *
 * The temperature & humidity module is an i2c device.  It must be plugged
 * into port 3, 4, 5 or 9.  It will not work in any other ports.
 *
 *
 * Commands:
 * TempHumid th;                            //This command set up the TempHumid th object.  TempHumid is the class and th is the name given to the object
 *                                          //This is the object that handles getting data from the AHT10 sensor
 *
 * th.begin();                              //This command initializes the sensor
 *
 * th.readSensor();                         //to get new data from the sensor this command must be called. It requests new data from
 *                                          //the sensor and stores it in temperature, temperatureC, and humidity variables that are
 *                                          //part of the object.
 *
 * th.temperature                           //the variable that contains the last temperature read from the sensor in units F
 *
 * th.temperatureC                          //the temperature in units C
 *
 * th.humidity                              //the variable that contains the last humidity read from the sensor in units % relative humidity
 *
**************************************************/

#include <ExoNaut.h>                        //include the main ExoNaut library
#include <ExoNaut_TempHumid.h>              //include the Temperature and Humidity library

exonaut robot;                              //declare the main robot object (needed for I2C initialization)
TempHumid th;                               //declare the TempHumid th object.  
                                            //TempHumid is the class th is the name given to the object

void setup(){                               //the setup() function runs a single time
  Serial.begin(9600);                       //start the Serial connection between the robot and the computer at 9600 baud
  robot.begin();                            //initialize the robot (this sets up I2C)
  th.begin();                               //start the th object
}

void loop(){                                //the loop() function runs forever in a loop
  th.readSensor();                          // read the sensor and update the values

  Serial.print("The temperature is:\t");    //outputs the text to the serial monitor with a tab to align the data
  Serial.print(th.temperature);             //outputs the value in th.temperature.  To get temperature in C use .temperatureC
  Serial.println(" F");                     //outputs the units followed by the carriage return and new line characters

  Serial.print("The humidity is:\t");       //outputs the text to the serial monitor with a tab to align the data
  Serial.print(th.humidity);                //outputs the value in th.humidity
  Serial.println(" %rH");                   //outputs the units followed by the carriage return and new line characters

  Serial.println();                         //sends a blank line with the carriage return and new line characters
  delay(1000);                              //wait for 1 second before sending data again
}
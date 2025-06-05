/***************************************************
 * L17_Display_Temp.ino
 * An example program to show how to use the four digit seven segment display
 *
 * Author:	Andrew Gafford
 * Email:		agafford@spacetrek.com
 * Date:		May 20th, 2024
 *
 * The temperature & humidity module is an i2c device.  It must be plugged
 * into port 3, 4, 5 or 9.  It will not work in any other ports.
 *
 * The seven segment display will only work if it is plugged into port 6 or port 8.
 * If it is plugged into port 6 the data pin is 33 and the clock pin is 25
 * If it is plugged into port 8 the data pin is 26 and the clock pin is 27
 *
 *
 * Commands:
 *
 * exonaut robot;                           //This command sets up the exonaut robot object.  exonaut is the class and robot is the name given to the object
 *                                          //This is the object that handles all of the motor commands and the features on the CoreX controller.
 * 
 * robot.begin();                           //This command initializes or begins the things in the robot object.
 *                                          //It is used once at the beginning of the program
 *
 * TempHumid th;                            //This command sets up the TempHumid th object.  TempHumid is the class and th is the name given to the object
 *                                          //This is the object that handles getting data from the AHT10 sensor
 *
 * TM1640 display(data pin, clock pin, number of digits);       //This command sets up the TM1640 display object. TM1640 is the class and display is the object
 *
 * th.begin();                              //This command initializes the sensor
 *
 * th.readSensor();                         //to get new data from the sensor this command must be called. It requests new data from
 *                                          //the sensor and stores it in temperature, temperatureC, and humidity variables that are
 *                                          //part of the object.
 *
 * th.temperature                           //the variable that contains the last temperature read from the sensor in units F
 *
 * display.clearDisplay();                                      //this command clears the display
 *
 * display.clearDisplayDigit(digit);                            //this command clears a single digit on the display
 *                                                              //the digits are numbered from 0 to 3 left to right
 *
 * display.setDisplayToDecNumber(number, _BV(digit));           //this command will put an integer number on the display and 
 *                                                              //turn on the decimal point at the digit specified in _BV()
 *                                                              //it is important to note that for _BV digits are numbered 1 to 4
 *                                                              //from right to left.
 *
***************************************************/


#include <ExoNaut.h>                          //Include the ExoNaut library
#include <ExoNaut_TempHumid.h>                //include the Temperature and Humidity library
#include <TM1640.h>                           //Include the TM1640 display driver library

//PORT 6
#define DISP_DATA_PIN     33                  //use these for PORT 6
#define DISP_CLK_PIN      25

//PORT8 8
// #define DISP_DATA_PIN     26                  //use these for PORT 8
// #define DISP_CLK_PIN      27

//The display has four digits
#define DISP_NUM_DIGITS   4                   //the display has four digits

exonaut robot;                                //declare the main robot object (needed for I2C initialization)
TempHumid th;                                 //declare the TempHumid th object.  
                                              //TempHumid is the class th is the name given to the object

TM1640 display(DISP_DATA_PIN, DISP_CLK_PIN, DISP_NUM_DIGITS);     //declare the TM1640 display object.  
                                                                  //TM1640 is the class display is the name given to the object

int displayTemp = 0;                          //an integer variable to hold the value to be displayed 

void setup(){                                 //setup() runs once at the beginning of the program
  robot.begin();
  th.begin();                                 //start the th object
}

void loop(){
  th.readSensor();                            // read the sensor and update the values
  displayTemp = th.temperature*10;            //store the floating point value of .temperature in an integer
                                              //it is multiplied by 10 so that the first digit past the decimal becomes the ones digit

  display.clearDisplay();                                 //clears the display
  display.setDisplayToDecNumber(displayTemp, _BV(1));     //digit 1 will be the decimal so put the decimal point on digit 2

  if(th.temperature < 100.0){                             //if the temperature only has two digits in front of the decimal
    display.clearDisplayDigit(0);                         //clear digit 0 so it doesn't show a zero
    if(th.temperature < 10.0){                            //if the temperature only has one digit in front of the decimal
      display.clearDisplayDigit(1);                       //clear digit 1 so it doesn't show a zero
    }
  }
  delay(1000);                                          
}

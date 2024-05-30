/***************************************************
 * L17_Display_Temp.ino
 * An example program to show how to use the four digit seven segment display
 *
 * Author:	Andrew Gafford
 * Email:		agafford@spacetrek.com
 * Date:		May 20th, 2024
 *
 * 
 *
 *
 * Commands:
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
***************************************************/


#include <ExoNaut.h>
#include <ExoNaut_TempHumid.h>              //include the Temperature and Humidity library
#include <TM1640.h>

//PORT 6
#define DISP_DATA_PIN     33
#define DISP_CLK_PIN      25

//PORT8 8
// #define DISP_DATA_PIN     26
// #define DISP_CLK_PIN      27

//The display has four digits
#define DISP_NUM_DIGITS   4

TempHumid th;                               //declare the TempHumid th object.  
                                            //TempHumid is the class th is the name given to the object

TM1640 display(DISP_DATA_PIN, DISP_CLK_PIN, DISP_NUM_DIGITS);

int displayTemp = 0;

void setup(){
  th.begin();                               //start the th object
}

void loop(){
  th.readSensor();                          // read the sensor and update the values
  displayTemp = th.temperature*10;          //store the floating point value of .temperature in an integer
                                            //it is multiplied by 10 so that the first digit past the decimal becomes the ones digit

  display.clearDisplay();
  display.setDisplayToDecNumber(displayTemp, _BV(2));       //digit 1 will be the decimal so put the decimal point on digit 2

  if(th.temperature < 100.0){                               //if the temperature only has two digits in front of the decimal
    display.clearDisplayDigit(0);                           //clear digit 0 so it doesn't show a zero
    if(th.temperature < 10.0){                              //if the temperature only has one digit in front of the decimal
      display.clearDisplayDigit(1);                         //clear digit 1 so it doesn't show a zero
    }
  }
  delay(1000);                                          
}

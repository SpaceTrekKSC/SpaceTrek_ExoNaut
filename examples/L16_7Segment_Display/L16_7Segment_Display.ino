/***************************************************
 * L16_7Segment_Display.ino
 * An example program to show how to use the four digit seven segment display
 *
 * Author:	Andrew Gafford
 * Email:		agafford@spacetrek.com
 * Date:		May 20th, 2024
 *
 * The seven segment display will only work if it is plugged into port 6 or port 8.
 * If it is plugged into port 6 the data pin is 33 and the clock pin is 25
 * If it is plugged into port 8 the data pin is 26 and the clock pin is 27
 *
 *
 * Commands:
 * TM1640 display(data pin, clock pin, number of digits);       //This command sets up the TM1640 display object. TM1640 is the class and display is the object
 *  
 * display.setDisplayToString("text");                          //this command will output text to the display.  There are 
 *                                                              //limitations to what can be displayed with a seven segment
 *                                                              //display. So some characters don't look right. 
 *
 * display.clearDisplay();                                      //this command clears the display
 *
 * display.clearDisplayDigit(digit);                            //this command clears a single digit on the display
 *                                                              //the digits are numbered from 0 to 3 left to right
 *
 * display.setDisplayDigit(number, digit);                      //this command will put a single number at a specific digit
 *                                                              //digits are numbered 0 to 3 left to right
 *
 * display.setDisplayToDecNumber(number, _BV(digit));           //this command will put an integer number on the display and 
 *                                                              //turn on the decimal point at the digit specified in _BV()
 *                                                              //it is important to note that for _BV digits are numbered 1 to 4
 *                                                              //from right to left.  This is opposite of the above two commands
 *
***************************************************/


#include <ExoNaut.h>                    //Include the ExoNaut library
#include <TM1640.h>                     //Include the TM1640 display driver library

//PORT 6
#define DISP_DATA_PIN     33            //use these for PORT 6
#define DISP_CLK_PIN      25

//PORT8 8
// #define DISP_DATA_PIN     26           //use these for PORT 8
// #define DISP_CLK_PIN      27

//The display has four digits
#define DISP_NUM_DIGITS   4             //the display has four digits

TM1640 display(DISP_DATA_PIN, DISP_CLK_PIN, DISP_NUM_DIGITS);     //setup the TM1640 display object.  TM1640 is the class display is the name given to the object

void setup() {                                        //setup runs once at the beginning of the program
  display.setDisplayToString("HALO");                 //output a string to the display
  delay(2000);                                        //wait 2 seconds

  display.clearDisplay();                             //clears the display
  delay(1000);                                        //wait 1 second
  
  int number = 1234;                                  //a variable who's value will be displayed

  display.setDisplayToDecNumber(number, _BV(0));      //displays an integer number and do not include the decimal
  delay(2000);                                        //wait 2 seconds

  display.setDisplayToDecNumber(number, _BV(1));      //displays an integer number but activates the decimal at location 1
  delay(1000);                                        //wait 1 second
  display.setDisplayToDecNumber(number, _BV(2));      //displays an integer number but activates the decimal at location 2
  delay(1000);                                      
  display.setDisplayToDecNumber(number, _BV(3));      //displays an integer number but activates the decimal at location 3
  delay(1000);
  display.setDisplayToDecNumber(number, _BV(4));      //displays an integer number but activates the decimal at location 4
  delay(2000);

  display.clearDisplayDigit(2);                       //clears digit 2.  The digits are numbered 0 to 3 left to right
  delay(2000);

  display.setDisplayDigit(9, 2);                      //sets digit 2 to 9
  delay(2000);
}

void loop() {
  // nothing in loop()
}

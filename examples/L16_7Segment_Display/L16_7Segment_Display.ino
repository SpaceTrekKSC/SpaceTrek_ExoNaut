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
#include <TM1640.h>

//PORT 6
#define DISP_DATA_PIN     33
#define DISP_CLK_PIN      25

//PORT8 8
// #define DISP_DATA_PIN     26
// #define DISP_CLK_PIN      27

//The display has four digits
#define DISP_NUM_DIGITS   4

TM1640 display(DISP_DATA_PIN, DISP_CLK_PIN, DISP_NUM_DIGITS);

void setup() {  
  display.setDisplayToString("HALO");                       //output a string to the disply
  delay(2000);                                              //wait 2 seconds

  display.clearDisplay();                                   //clears the display
  delay(1000);
  
  int number = 1234;

  display.setDisplayToDecNumber(number, _BV(0));            //displays an integer number and do not include the decimal
  delay(2000);

  display.setDisplayToDecNumber(number, _BV(1));            //displays an integer number but activates the decimal at location 1
  delay(1000);
  display.setDisplayToDecNumber(number, _BV(2));            //displays an integer number but activates the decimal at location 2
  delay(1000);
  display.setDisplayToDecNumber(number, _BV(3));            //displays an integer number but activates the decimal at location 3
  delay(1000);
  display.setDisplayToDecNumber(number, _BV(4));            //displays an integer number but activates the decimal at location 4
  delay(2000);

  display.clearDisplayDigit(2);                             //clears digit 2
  delay(2000);

  display.setDisplayDigit(9, 2);                            //sets digit 2 to 9
  delay(2000);

  

}

void loop() {
  // put your main code here, to run repeatedly:

}

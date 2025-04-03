/**********************************************************
 * L18_Dot_Matrix.ino
 * An example program to show how to use the 16x8 Dot Matrix display
 *
 * Author:	Andrew Gafford
 * Email:		agafford@spacetrek.com
 * Date:		May 20th, 2024
 *
 * The dot matrix display will only work if it is plugged into port 6 or port 8.
 * If it is plugged into port 6 the data pin is 33 and the clock pin is 25
 * If it is plugged into port 8 the data pin is 26 and the clock pin is 27
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
**********************************************************/
#include <TM1640.h>                     //include the TM1640 display driver library
#include <TM16xxMatrix.h>               //include the TM16xx dot matrix library

//PORT 6
#define DISP_DATA_PIN     33            //use these for PORT 6
#define DISP_CLK_PIN      25

//PORT8 8
// #define DISP_DATA_PIN     26           //use these for PORT 8
// #define DISP_CLK_PIN      27

#define MATRIX_NUMCOLUMNS 16            //the number of columns on the display
#define MATRIX_NUMROWS 8                //thenumber of rows on the display

TM1640 module(DISP_DATA_PIN, DISP_CLK_PIN);                         // TM1640 module(data pin, clock pin)
TM16xxMatrix matrix(&module, MATRIX_NUMCOLUMNS, MATRIX_NUMROWS);    // TM16xxMatrix matrix(object, columns, rows)

bool fLoop=true;

uint8_t image[8][16] = {                        //a two dimensional array to store bit data for an image
  {1,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1},            //1 is equivilant to true and 0 is equivalent to false
  {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1},            //so each of the 1s will be turned on and each of the 0s
  {1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1},            //will be turned off.  If you edit this make sure you keep
  {1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,1},            //it at 8 rows with 16 columns.
  {1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,1},
  {1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1},
  {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
  {1,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1}
};

void setup(){
  module.clearDisplay();                        //clear the display
  delay(500);                                   //wait 1 second
  
  matrix.setAll(true);                          //turn all dots on
  delay(1000);                                  //wait 1 second

  // Columns binary
  for(int i=0; i<MATRIX_NUMCOLUMNS; i++){
    for(int j=0; j<=0xFF; j++){
      matrix.setColumn(i,j);
      delay(3);
    }
    delay(50);
  }
  matrix.setAll(false);                         //another way to clear the display
  delay(1000);

  // One pixel, column by column
  for(int i=0; i<MATRIX_NUMCOLUMNS; i++){
    for(int j=0; j<MATRIX_NUMROWS; j++){
        matrix.setPixel(i,j, true);
        delay(25);
        matrix.setPixel(i,j, false);
    }
  }

  // One pixel, row by row
  for(int i=0; i<MATRIX_NUMROWS; i++){
    for(int j=0; j<MATRIX_NUMCOLUMNS; j++){
        matrix.setPixel(j,i, true);
        delay(25);
        matrix.setPixel(j,i, false);
    }
  }
}

void loop(){
  // Fill columns
  for(int i=0; i<MATRIX_NUMROWS; i++)  {
    for(int j=0; j<MATRIX_NUMCOLUMNS; j++)    {
        matrix.setPixel(j, i, true);
        delay(25);
    }
  }
  // Unfill columns
  for(int i=0; i<MATRIX_NUMROWS; i++)  {
    for(int j=0; j<MATRIX_NUMCOLUMNS; j++)    {
        matrix.setPixel(j, i, false);
        delay(15);
    }
  }
  

  // draw image[8][16]
  for(int i=0; i<MATRIX_NUMROWS; i++){
    for(int j=0; j<MATRIX_NUMCOLUMNS; j++){
        matrix.setPixel(j,i, image[i][j]);
        delay(8);
    }
  }
  delay(5000);
  matrix.setAll(false);
  delay(1000);
}

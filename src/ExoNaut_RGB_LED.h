/*
*ExoNaut_RGB_LED.h
*
*Author:	Andrew Gafford
*Email:		agafford@spacetrek.com
*Date:		May 20th, 2024
*
*This library is for controlling the Space Trek ExoNaut Robot's RGB Ultrasonic
*distance sensor.  It provides functions to read the distance and set the RGB LEDS.
*
*The RGB LED module uses two Neo Pixels.  You can use the Adafruit NeoPixel library 
*if you would like to experiment with additional features and controls. The RGB LED
*module will work on ports 2, 6 and 8 of the ExoNout robot.  If you are using the 
*Adafruit library the following table gives the port to pin mapping.
*	__________________________
*	PORT Number |	Pin Number
*		2		|		32
*		6		|		33
*		8		|		26
*
*/

#ifndef __EXONAUT_RGBLED_h
#define __EXONAUT_RGBLED_h

#include "ExoNaut.h"
#include <Adafruit_NeoPixel.h>

class RGB{
	public:
		void begin(uint8_t port);
		void setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
		void setColorAll(uint8_t r, uint8_t g, uint8_t b);
		void show(void);
		void clear(void);
	private:
};

extern Adafruit_NeoPixel pixels2;

#endif //end __EXONAUT_RGBLED_h
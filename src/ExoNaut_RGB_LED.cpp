/*
 *ExoNaut_RGB_LED.h
 *
 *Author:    Andrew Gafford
 *Email:        agafford@spacetrek.com
 *Date:        May 20th, 2024
 *
 *This library is for controlling the Space Trek ExoNaut Robot's RGB Ultrasonic
 *distance sensor.  It provides functions to read the distance and set the RGB LEDS.
 *
 */

#include "ExoNaut_RGB_LED.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels2(2, 36, NEO_GRB + NEO_KHZ800);

void RGB::begin(uint8_t port)
{
	int8_t pin_number;
	switch (port)
	{
	case 2:
		pin_number = 32;
		break;
	case 6:
		pin_number = 33;
		break;
	case 8:
		pin_number = 26;
		break;
	default:
		pin_number = -1;
		break;
	}
	pixels2.setPin(pin_number);
	pixels2.begin();
	for (int i = 0; i < 2; i++)
	{
		pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
	}
	pixels2.show();
}

void RGB::setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	pixels2.setPixelColor(n, pixels2.Color(r, g, b));
}

void RGB::setColorAll(uint8_t r, uint8_t g, uint8_t b)
{
	for (int i = 0; i < 2; i++)
	{
		pixels2.setPixelColor(i, pixels2.Color(r, g, b));
	}
	pixels2.show();
}

void RGB::show(void)
{
	pixels2.show();
}

void RGB::clear(void)
{
	pixels2.clear();
	pixels2.show();
}

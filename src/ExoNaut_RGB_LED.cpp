/*
 *ExoNaut_RGB_LED.cpp
 *
 *Author:	Andrew Gafford
 *Email:		agafford@spacetrek.com
 *Date:		May 20th, 2024
 *
 *This library is for controlling the Space Trek ExoNaut Robot's RGB LED module.
 *It provides functions to set the RGB LEDs.
 *
 */

#include "ExoNaut_RGB_LED.h"

void RGB::begin(uint8_t port)
{
	// Determine pin based on port number
	int pin_number;

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
		// Invalid port, use a default
		pin_number = 32;
		break;
	}

	// Delete previous instance if it exists
	if (pixels_rgb != NULL)
	{
		delete pixels_rgb;
	}

	// Create a new NeoPixel instance with the appropriate pin
	pixels_rgb = new Adafruit_NeoPixel(RGB_LED_NUM_PIXELS, pin_number, NEO_GRB + NEO_KHZ800);

	// Initialize the NeoPixel library
	pixels_rgb->begin();

	// Set brightness to maximum (255)
	pixels_rgb->setBrightness(255);

	// Turn off all pixels initially
	for (int i = 0; i < RGB_LED_NUM_PIXELS; i++)
	{
		pixels_rgb->setPixelColor(i, pixels_rgb->Color(0, 0, 0));
	}

	pixels_rgb->show();
}

void RGB::setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	if (pixels_rgb != NULL && n < RGB_LED_NUM_PIXELS)
	{
		pixels_rgb->setPixelColor(n, pixels_rgb->Color(r, g, b));
	}
}

void RGB::setColorAll(uint8_t r, uint8_t g, uint8_t b)
{
	if (pixels_rgb != NULL)
	{
		for (int i = 0; i < RGB_LED_NUM_PIXELS; i++)
		{
			pixels_rgb->setPixelColor(i, pixels_rgb->Color(r, g, b));
		}
		pixels_rgb->show();
	}
}

void RGB::show(void)
{
	if (pixels_rgb != NULL)
	{
		pixels_rgb->show();
	}
}

void RGB::clear(void)
{
	if (pixels_rgb != NULL)
	{
		pixels_rgb->clear();
		pixels_rgb->show();
	}
}

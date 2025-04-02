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

RGB::RGB()
{
	// Constructor
	_initialized = false;
	pixels_rgb = NULL;
	_pin_number = -1;
}

RGB::~RGB()
{
	// Destructor - clean up resources
	if (pixels_rgb != NULL)
	{
		delete pixels_rgb;
		pixels_rgb = NULL;
	}
	_initialized = false;
}

void RGB::begin(uint8_t port)
{
	Serial.print("RGB::begin - Initializing on port: ");
	Serial.println(port);

	// Determine pin based on port number
	switch (port)
	{
	case 2:
		_pin_number = 32;
		break;
	case 6:
		_pin_number = 33;
		break;
	case 8:
		_pin_number = 26;
		break;
	default:
		// Invalid port, use a default
		Serial.println("WARNING: Invalid port specified. Using port 2 (pin 32) as default.");
		_pin_number = 32;
		break;
	}

	Serial.print("RGB::begin - Using pin number: ");
	Serial.println(_pin_number);

	// Delete previous instance if it exists
	if (pixels_rgb != NULL)
	{
		Serial.println("RGB::begin - Deleting previous NeoPixel instance");
		delete pixels_rgb;
		pixels_rgb = NULL;
	}

	// Create a new NeoPixel instance with the appropriate pin
	Serial.print("RGB::begin - Creating new NeoPixel instance with ");
	Serial.print(RGB_LED_NUM_PIXELS);
	Serial.print(" pixels on pin ");
	Serial.println(_pin_number);

	// Using NEO_RGB color order as in the working example
	pixels_rgb = new Adafruit_NeoPixel(RGB_LED_NUM_PIXELS, _pin_number, NEO_RGB + NEO_KHZ800);

	if (pixels_rgb == NULL)
	{
		Serial.println("ERROR: Failed to create NeoPixel instance!");
		_initialized = false;
		return;
	}

	// Initialize the NeoPixel library
	Serial.println("RGB::begin - Calling pixels_rgb->begin()");
	pixels_rgb->begin();

	// Set brightness to maximum (255)
	pixels_rgb->setBrightness(255);

	// Turn off all pixels initially
	Serial.println("RGB::begin - Setting all pixels to off");
	for (int i = 0; i < RGB_LED_NUM_PIXELS; i++)
	{
		pixels_rgb->setPixelColor(i, pixels_rgb->Color(0, 0, 0));
	}

	Serial.println("RGB::begin - Calling pixels_rgb->show()");
	pixels_rgb->show();

	_initialized = true;
	Serial.println("RGB::begin - Initialization complete");
}

bool RGB::isInitialized()
{
	return _initialized && (pixels_rgb != NULL);
}

void RGB::setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	if (!isInitialized())
	{
		Serial.println("ERROR: RGB::setColor - Module not initialized");
		return;
	}

	if (n >= RGB_LED_NUM_PIXELS)
	{
		Serial.print("ERROR: RGB::setColor - Pixel index out of range: ");
		Serial.println(n);
		return;
	}

	pixels_rgb->setPixelColor(n, pixels_rgb->Color(r, g, b));
}

void RGB::setColorAll(uint8_t r, uint8_t g, uint8_t b)
{
	if (!isInitialized())
	{
		Serial.println("ERROR: RGB::setColorAll - Module not initialized");
		return;
	}

	for (int i = 0; i < RGB_LED_NUM_PIXELS; i++)
	{
		pixels_rgb->setPixelColor(i, pixels_rgb->Color(r, g, b));
	}
	pixels_rgb->show();
}

void RGB::show(void)
{
	if (!isInitialized())
	{
		Serial.println("ERROR: RGB::show - Module not initialized");
		return;
	}

	pixels_rgb->show();
}

void RGB::clear(void)
{
	if (!isInitialized())
	{
		Serial.println("ERROR: RGB::clear - Module not initialized");
		return;
	}

	pixels_rgb->clear();
	pixels_rgb->show();
}

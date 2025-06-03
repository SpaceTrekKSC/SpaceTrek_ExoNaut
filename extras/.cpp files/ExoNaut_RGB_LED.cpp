#include "ExoNaut_RGB_LED.h"

// Constructor definition for RGB class
RGB::RGB(uint8_t num_leds, int8_t pin, rmt_channel_t rmt_ch, neoPixelType type) : _pixels_member(num_leds, pin, type, rmt_ch)
{
	// The _pixels_member is initialized using the member initializer list
}

void RGB::begin()
{
	_pixels_member.begin();
}

void RGB::setBrightness(uint8_t b)
{
	_pixels_member.setBrightness(b);
}

void RGB::setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	_pixels_member.setPixelColor(n, r, g, b);
}

// Implementation for the new setColor overload
void RGB::setColor(uint16_t n, uint32_t c)
{
	// Delegate to the ExoNautPixelController's overload that handles packed uint32_t color
	_pixels_member.setPixelColor(n, c);
}

void RGB::show()
{
	_pixels_member.show();
}

void RGB::clear()
{
	_pixels_member.clear();
	// Note: show() is not automatically called by this clear() method.
	// Call show() explicitly in your sketch if you want to update the LEDs immediately after clearing.
}

uint16_t RGB::numPixels()
{
	return _pixels_member.numPixels();
}
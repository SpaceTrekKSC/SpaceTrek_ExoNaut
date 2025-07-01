#include "ExoNaut_RGB_LED.h"

// Helper function to get the GPIO pin for a given port.
static int8_t getPinForRGB(uint8_t port)
{
	switch (port)
	{
	case 1:
		return 36;
	case 2:
		return 32;
	case 6:
		return 33;
	case 8:
		return 26;
	default:
		return -1;
	}
}

// Helper function to get a unique RMT channel for a given port.
static rmt_channel_t getRmtForRGB(uint8_t port)
{
	switch (port)
	{
	case 1:
		return RMT_CHANNEL_3;
	case 2:
		return RMT_CHANNEL_4;
	case 6:
		return RMT_CHANNEL_1;
	case 8:
		return RMT_CHANNEL_2;
	default:
		return RMT_CHANNEL_MAX;
	}
}

// New constructor that only takes the port number and assumes 2 LEDs.
RGB::RGB(uint8_t port) : RGB(port, 2) // Delegates to the other constructor with num_leds = 2
{
	// Body is empty.
}

// Constructor that takes port and number of LEDs.
RGB::RGB(uint8_t port, uint8_t num_leds) : RGB(num_leds, getPinForRGB(port), getRmtForRGB(port))
{
	// Body is empty.
}

// Original, detailed constructor.
RGB::RGB(uint8_t num_leds, int8_t pin, rmt_channel_t rmt_ch, neoPixelType type) : _pixels_member(num_leds, pin, type, rmt_ch)
{
	// Body is empty.
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
	_pixels_member.show();
}

void RGB::setColor(uint16_t n, uint32_t c)
{
	_pixels_member.setPixelColor(n, c);
	_pixels_member.show();
}

void RGB::show()
{
	_pixels_member.show();
}

void RGB::clear()
{
	_pixels_member.clear();
}

uint16_t RGB::numPixels()
{
	return _pixels_member.numPixels();
}

void RGB::colorWipe(uint8_t r, uint8_t g, uint8_t b, int wait)
{
	for (uint16_t i = 0; i < numPixels(); i++)
	{
		_pixels_member.setPixelColor(i, r, g, b);
		_pixels_member.show();
		delay(wait);
	}
}

void RGB::colorWipe(uint32_t c, int wait)
{
	colorWipe((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, wait);
}

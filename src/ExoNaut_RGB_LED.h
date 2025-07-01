#ifndef EXONAUT_RGB_LED_H
#define EXONAUT_RGB_LED_H

#include "ExoNaut.h"

class RGB
{
public:
	// New, simplified constructor: Takes only a port number.
	// The number of LEDs will default to 2.
	RGB(uint8_t port);

	// Overloaded constructor for cases with a different number of LEDs.
	RGB(uint8_t port, uint8_t num_leds);

	// Original, more detailed constructor
	RGB(uint8_t num_leds, int8_t pin, rmt_channel_t rmt_ch, neoPixelType type = NEO_GRB + NEO_KHZ800);

	void begin();
	void setBrightness(uint8_t b);
	void setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void setColor(uint16_t n, uint32_t c);
	void show();
	void clear();
	uint16_t numPixels();

	void colorWipe(uint8_t r, uint8_t g, uint8_t b, int wait);
	void colorWipe(uint32_t c, int wait);

private:
	ExoNautPixelController _pixels_member;
};

#endif // EXONAUT_RGB_LED_H

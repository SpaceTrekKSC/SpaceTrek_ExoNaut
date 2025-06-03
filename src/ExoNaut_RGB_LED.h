#ifndef EXONAUT_RGB_LED_H
#define EXONAUT_RGB_LED_H

#include "ExoNaut.h" // This includes ExoNautPixel.h, providing ExoNautPixelController and rmt_channel_t

class RGB
{
public:
	// Constructor: number of LEDs, pin for this external strip, RMT channel to use, and pixel type
	RGB(uint8_t num_leds, int8_t pin, rmt_channel_t rmt_ch, neoPixelType type = NEO_GRB + NEO_KHZ800);

	void begin();
	void setBrightness(uint8_t b);
	void setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void setColor(uint16_t n, uint32_t c); // << ADDED THIS OVERLOAD for packed uint32_t color
	void show();
	void clear(); // Clears buffer; call show() separately to update physical LEDs
	uint16_t numPixels();

private:
	ExoNautPixelController _pixels_member; // Encapsulated pixel controller instance
};

#endif // EXONAUT_RGB_LED_H
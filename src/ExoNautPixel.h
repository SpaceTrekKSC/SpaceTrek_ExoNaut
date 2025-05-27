#ifndef EXONAUT_PIXEL_H
#define EXONAUT_PIXEL_H

#include <Arduino.h>
#if defined(ESP32)
#include "driver/rmt.h"
#include "esp_idf_version.h"
#endif

// Color orders and speed
#define NEO_GRB ((1 << 6) | (1 << 4) | (0 << 2) | (2))
#define NEO_KHZ800 0x0000
typedef uint16_t neoPixelType;

class ExoNautPixelController
{
public:
    // Constructor now takes RMT channel
    ExoNautPixelController(uint16_t n, int16_t p_pin, neoPixelType type_val, rmt_channel_t rmt_ch_num = RMT_CHANNEL_MAX);
    ~ExoNautPixelController();

    void begin(void);
    void show(void);
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void setPixelColor(uint16_t n, uint32_t c);
    void clear(void);
    uint16_t numPixels(void) const;
    void setBrightness(uint8_t b);     // Added
    uint8_t getBrightness(void) const; // Added

    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

private:
    void updateType(neoPixelType t);
    void updateLength(uint16_t n);
    void setPin(int16_t p_pin);

    bool is800KHz;
    bool begun;
    uint16_t numLEDs;
    uint16_t numBytes;
    int16_t pin_num;
    uint8_t *pixels;
    uint8_t rOffset;
    uint8_t gOffset;
    uint8_t bOffset;
    uint8_t wOffset;
    uint32_t endTime;
    uint8_t brightness_level; // Added for brightness control

#if defined(ESP32)
    rmt_channel_t _rmt_channel; // Made non-static (per instance)
    bool _rmt_driver_installed; // Made non-static (per instance)

    static const int WS2812_T0H_NS = 400;
    static const int WS2812_T0L_NS = 850;
    static const int WS2812_T1H_NS = 800;
    static const int WS2812_T1L_NS = 450;
    static const int WS2811_T0H_NS = 500;
    static const int WS2811_T0L_NS = 2000;
    static const int WS2811_T1H_NS = 1200;
    static const int WS2811_T1L_NS = 1300;

    // Per-instance tick values for RMT
    uint32_t _t0h_ticks;
    uint32_t _t1h_ticks;
    uint32_t _t0l_ticks;
    uint32_t _t1l_ticks;

    // Static pointer for the RMT adapter to access the current instance's data
    static ExoNautPixelController *_active_instance_for_rmt;
    // Static RMT adapter function declaration
    static void IRAM_ATTR ws2812_rmt_adapter_static(const void *src, rmt_item32_t *dest, size_t src_size,
                                                    size_t wanted_num, size_t *translated_size, size_t *item_num);
#endif
};

#endif // EXONAUT_PIXEL_H
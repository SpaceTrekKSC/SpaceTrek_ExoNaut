#include "ExoNautPixel.h"

#if defined(ESP32)
// Definition for the static member
ExoNautPixelController *ExoNautPixelController::_active_instance_for_rmt = nullptr;
#endif

// Constructor definition matching the 4-argument declaration
ExoNautPixelController::ExoNautPixelController(uint16_t n, int16_t p_pin, neoPixelType type_val, rmt_channel_t rmt_ch_num) : is800KHz(true), begun(false), numLEDs(0), numBytes(0), pin_num(-1), pixels(NULL),
                                                                                                                             rOffset(0), gOffset(0), bOffset(0), wOffset(0), endTime(0), brightness_level(255) // Initialize brightness
#if defined(ESP32)
                                                                                                                             ,
                                                                                                                             _rmt_channel(rmt_ch_num), _rmt_driver_installed(false), // Initialize RMT members
                                                                                                                             _t0h_ticks(0), _t1h_ticks(0), _t0l_ticks(0), _t1l_ticks(0)
#endif
{
    updateType(type_val);
    setPin(p_pin);
    updateLength(n);
}

ExoNautPixelController::~ExoNautPixelController()
{
    free(pixels);
    pixels = NULL;
#if defined(ESP32)
    if (_rmt_driver_installed)
    {                                       // Use member variable
        rmt_driver_uninstall(_rmt_channel); // Use member variable
        _rmt_driver_installed = false;
    }
#endif
}

void ExoNautPixelController::updateType(neoPixelType t)
{
    wOffset = (t >> 6) & 0b11;
    rOffset = (t >> 4) & 0b11;
    gOffset = (t >> 2) & 0b11;
    bOffset = t & 0b11;
    is800KHz = (t < 256);
}

void ExoNautPixelController::updateLength(uint16_t n)
{
    free(pixels);
    pixels = NULL;
    numBytes = n * ((wOffset == rOffset) ? 3 : 4);
    if (numBytes > 0)
    {
        if ((pixels = (uint8_t *)malloc(numBytes)))
        {
            memset(pixels, 0, numBytes);
        }
        else
        {
            numBytes = 0;
        }
    }
    else
    {
        pixels = NULL;
    }
    numLEDs = (pixels ? n : 0);
}

void ExoNautPixelController::setPin(int16_t p_pin)
{
    if (begun && (pin_num >= 0))
    {
        // pinMode(pin_num, INPUT);
    }
    pin_num = p_pin;
    if (begun)
    {
        pinMode(pin_num, OUTPUT);
        digitalWrite(pin_num, LOW);
    }
}

void ExoNautPixelController::begin(void)
{
    if (pin_num >= 0)
    {
        pinMode(pin_num, OUTPUT);
        digitalWrite(pin_num, LOW);
    }
    begun = true;
}

void ExoNautPixelController::clear(void)
{
    if (pixels)
    {
        memset(pixels, 0, numBytes);
    }
}

uint16_t ExoNautPixelController::numPixels(void) const
{
    return numLEDs;
}

// Definition for setBrightness
void ExoNautPixelController::setBrightness(uint8_t b)
{
    this->brightness_level = b;
}

// Definition for getBrightness
uint8_t ExoNautPixelController::getBrightness(void) const
{
    return this->brightness_level;
}

void ExoNautPixelController::setPixelColor(uint16_t n, uint8_t r_in, uint8_t g_in, uint8_t b_in)
{
    if (pixels && n < numLEDs)
    {
        uint8_t r = r_in;
        uint8_t g = g_in;
        uint8_t b = b_in;

        if (this->brightness_level < 255)
        {
            if (this->brightness_level == 0)
            {
                r = 0;
                g = 0;
                b = 0;
            }
            else
            {
                r = (uint8_t)(((uint16_t)r * this->brightness_level) >> 8);
                g = (uint8_t)(((uint16_t)g * this->brightness_level) >> 8);
                b = (uint8_t)(((uint16_t)b * this->brightness_level) >> 8);
            }
        }

        uint8_t *p = &pixels[n * 3];
        p[gOffset] = g;
        p[rOffset] = r;
        p[bOffset] = b;
    }
}

void ExoNautPixelController::setPixelColor(uint16_t n, uint32_t c)
{
    setPixelColor(n, (uint8_t)(c >> 16), (uint8_t)(c >> 8), (uint8_t)c);
}

#if defined(ESP32)
// Definition for ws2812_rmt_adapter_static
void IRAM_ATTR ExoNautPixelController::ws2812_rmt_adapter_static(const void *src, rmt_item32_t *dest, size_t src_size,
                                                                 size_t wanted_num, size_t *translated_size, size_t *item_num)
{
    if (!_active_instance_for_rmt || src == NULL || dest == NULL)
    {
        *translated_size = 0;
        *item_num = 0;
        return;
    }

    const rmt_item32_t bit0 = {{{_active_instance_for_rmt->_t0h_ticks, 1, _active_instance_for_rmt->_t0l_ticks, 0}}};
    const rmt_item32_t bit1 = {{{_active_instance_for_rmt->_t1h_ticks, 1, _active_instance_for_rmt->_t1l_ticks, 0}}};
    size_t current_size = 0;
    size_t current_item_num = 0;
    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;

    while (current_size < src_size && current_item_num < wanted_num)
    {
        for (int i = 0; i < 8; i++)
        {
            if (*psrc & (1 << (7 - i)))
            {
                pdest->val = bit1.val;
            }
            else
            {
                pdest->val = bit0.val;
            }
            current_item_num++;
            pdest++;
        }
        current_size++;
        psrc++;
    }
    *translated_size = current_size;
    *item_num = current_item_num;
}

void ExoNautPixelController::show(void)
{
    if (!pixels || !numBytes || pin_num < 0 || _rmt_channel == RMT_CHANNEL_MAX)
    {
        return;
    }

    if (!_rmt_driver_installed)
    {                                                                                   // Use member variable
        rmt_config_t config = RMT_DEFAULT_CONFIG_TX((gpio_num_t)pin_num, _rmt_channel); // Use member variable
        config.clk_div = 2;

        if (rmt_config(&config) != ESP_OK)
            return;
        if (rmt_driver_install(_rmt_channel, 0, 0) != ESP_OK)
            return;                   // Use member variable
        _rmt_driver_installed = true; // Use member variable
    }

    uint32_t counter_clk_hz = 0;
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
    if (rmt_get_counter_clock(_rmt_channel, &counter_clk_hz) != ESP_OK)
    { // Use member variable
        counter_clk_hz = APB_CLK_FREQ / 2;
    }
#else
    counter_clk_hz = APB_CLK_FREQ / 2;
#endif

    if (counter_clk_hz == 0)
    {
        return;
    }

    float ratio = (float)counter_clk_hz / 1e9f;

    if (is800KHz)
    {
        _t0h_ticks = (uint32_t)(ratio * WS2812_T0H_NS); // Use member variable
        _t0l_ticks = (uint32_t)(ratio * WS2812_T0L_NS); // Use member variable
        _t1h_ticks = (uint32_t)(ratio * WS2812_T1H_NS); // Use member variable
        _t1l_ticks = (uint32_t)(ratio * WS2812_T1L_NS); // Use member variable
    }
    else
    {
        _t0h_ticks = (uint32_t)(ratio * WS2811_T0H_NS); // Use member variable
        _t0l_ticks = (uint32_t)(ratio * WS2811_T0L_NS); // Use member variable
        _t1h_ticks = (uint32_t)(ratio * WS2811_T1H_NS); // Use member variable
        _t1l_ticks = (uint32_t)(ratio * WS2811_T1L_NS); // Use member variable
    }

    _active_instance_for_rmt = this;

    esp_err_t trans_err = rmt_translator_init(_rmt_channel, ws2812_rmt_adapter_static); // Use member variable
    if (trans_err != ESP_OK)
    {
        _active_instance_for_rmt = nullptr;
        return;
    }

    esp_err_t write_err = rmt_write_sample(_rmt_channel, pixels, numBytes, true); // Use member variable
    _active_instance_for_rmt = nullptr;

    if (write_err != ESP_OK)
    {
        return;
    }
    endTime = micros();
}
#else
void ExoNautPixelController::show(void) {}			// ... as before 
#endif
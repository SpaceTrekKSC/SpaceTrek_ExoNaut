/**
 * Simple_External_RGB_Test_Integrated.ino
 * A basic test to directly control an external NeoPixel RGB LED module
 * using the integrated RGB class from the ExoNaut library.
 *
 * The RGB LED module comments from original:
 * - Port 6: data pin 33
 * - Port 8: data pin 26
 */

#include <ExoNaut_RGB_LED.h> // Use your library's RGB LED header
                             // This should include ExoNaut.h, which includes ExoNautPixel.h

// Define the pin and number of pixels for the EXTERNAL module
//#define PIN_EXTERNAL 26        // Pin for the external module (e.g., Port 8)
#define PIN_EXTERNAL 33     // Or Pin for Port 6
#define NUM_PIXELS_EXTERNAL 2  // Number of LEDs in the external module

// IMPORTANT: Define an RMT channel for this external LED strip.
// This MUST be different from the RMT channel used by the onboard LEDs (which uses RMT_CHANNEL_0).
// Let's use RMT_CHANNEL_1.
#define RMT_CHANNEL_EXTERNAL RMT_CHANNEL_1

// Create an instance of your RGB class.
// It's initialized with number of pixels, pin, RMT channel, and pixel type.
// The NEO_GRB and NEO_KHZ800 constants should be available from ExoNautPixel.h.
RGB pixels(NUM_PIXELS_EXTERNAL, PIN_EXTERNAL, RMT_CHANNEL_EXTERNAL, NEO_GRB + NEO_KHZ800);
// Note: I've kept the object name 'pixels' to minimize changes to your original sketch's loop and colorWipe.

void setup() {
  Serial.begin(115200);
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime < 3000)) {
    delay(10); // Wait for Serial Monitor
  }
  Serial.println("Simple External RGB Test (Integrated Library)");

  pixels.begin(); // Initialize the RGB LED module (calls _pixels_member.begin())
  pixels.setBrightness(255); // Set to max brightness (assumes RGB class has setBrightness)

  // Turn all pixels off initially
  pixels.clear(); // Clears the buffer in _pixels_member
  pixels.show();  // Updates the physical LEDs
  
  Serial.print("Initialized NeoPixels on pin ");
  Serial.print(PIN_EXTERNAL);
  Serial.print(" using RMT Channel ");
  Serial.println(RMT_CHANNEL_EXTERNAL);
}

void loop() {
  // Cycle through colors
  Serial.println("Setting RED");
  // Use the static ExoNautPixelController::Color to get the packed uint32_t value
  colorWipe(ExoNautPixelController::Color(255, 0, 0), 500); // Red

  Serial.println("Setting GREEN");
  colorWipe(ExoNautPixelController::Color(0, 255, 0), 500); // Green

  Serial.println("Setting BLUE");
  colorWipe(ExoNautPixelController::Color(0, 0, 255), 500); // Blue

  Serial.println("Setting WHITE");
  colorWipe(ExoNautPixelController::Color(255, 255, 255), 500); // White (full brightness)
  // For a dimmer white if brightness is set to max:
  // colorWipe(ExoNautPixelController::Color(150, 150, 150), 500); 

  Serial.println("All off");
  pixels.clear();
  pixels.show();
  delay(1000);
}

// Fill the dots one after the other with a color
// This function remains largely the same, just uses the 'pixels' object of type 'RGB'.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < pixels.numPixels(); i++) { // Uses RGB::numPixels()
    pixels.setColor(i, color); // Uses RGB::setColor(uint16_t, uint32_t)
    pixels.show();             // Uses RGB::show()
    delay(wait);
  }
}
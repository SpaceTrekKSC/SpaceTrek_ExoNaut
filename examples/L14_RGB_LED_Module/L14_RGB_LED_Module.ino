/**
 * Simple_RGB_Test.ino
 * A basic test to directly control NeoPixels on pin 26
 * without using the ExoNaut_RGB_LED library
 */

#include <Adafruit_NeoPixel.h>

// Define the pin and number of pixels
#define PIN 26
#define NUM_PIXELS 2

 Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  Serial.println("Simple RGB Test");
  
  pixels.begin();
  pixels.setBrightness(255);  // Max brightness
  
  // Turn all pixels off initially
  pixels.clear();
  pixels.show();
  
  Serial.println("Initialized NeoPixels on pin 26");
}

void loop() {
  // Cycle through colors
  Serial.println("Setting RED");
  colorWipe(pixels.Color(255, 0, 0), 500); // Red
  
  Serial.println("Setting GREEN");
  colorWipe(pixels.Color(0, 255, 0), 500); // Green
  
  Serial.println("Setting BLUE");
  colorWipe(pixels.Color(0, 0, 255), 500); // Blue
  
  Serial.println("Setting WHITE");
  colorWipe(pixels.Color(255, 255, 255), 500); // White
  
  Serial.println("All off");
  pixels.clear();
  pixels.show();
  delay(1000);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}

#include <Adafruit_NeoPixel.h>

#define PIN 22        // Data pin connected to the NeoPixels
#define NUM_PIXELS 16 // Number of NeoPixels in the ring

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
int offset = 0;
int fadeLength = 10; // Number of pixels in the fade trail
volatile float brightness = 1.0; // Default brightness
volatile bool newBrightnessAvailable = false;

void setup() {
  pixels.begin(); // Initialize NeoPixel strip
  Serial.begin(9600); // Initialize serial communication
  pixels.setBrightness(2.55); // Set initial brightness to maximum
}

void loop() {
  rainbowChaseWithFade(50); // Run rainbow chase animation with 50ms delay between updates
  
  checkSerial();
  
  if (newBrightnessAvailable) {
    pixels.setBrightness(brightness * 2.55);
    newBrightnessAvailable = false;
  }
}

void checkSerial() {
  static char buffer[10];
  static uint8_t index = 0;
  
  while (Serial.available() > 0) {
    char received = Serial.read();
    
    if (received == '\n') {
      buffer[index] = '\0'; // Null-terminate the string
      brightness = atof(buffer); // Convert string to float
      newBrightnessAvailable = true;
      index = 0; // Reset buffer index
    } else if (index < 9) {
      buffer[index++] = received;
    }
  }
}

void rainbowChaseWithFade(int wait) {
  for(int i=0; i<pixels.numPixels(); i++) {
    int hue = ((i + offset) * 65536L / pixels.numPixels()) % 65536;
    int distance = (i - offset + pixels.numPixels()) % pixels.numPixels();
    
    if (distance < fadeLength) {
      uint32_t color = pixels.gamma32(pixels.ColorHSV(hue));
      int brightness = 255 - (255 * distance / fadeLength);
      pixels.setPixelColor(i, dimColor(color, brightness));
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Set to black
    }
  }
  
  pixels.show();
  delay(wait);
  
  offset = (offset + 1) % pixels.numPixels();
}

uint32_t dimColor(uint32_t color, int brightness) {
  uint8_t r = (uint8_t)(color >> 16);
  uint8_t g = (uint8_t)(color >> 8);
  uint8_t b = (uint8_t)color;
  
  r = (r * brightness) / 255;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;
  
  return pixels.Color(r, g, b);
}


#include <FastLED.h>

#define NUM_DEVICES 2
#define DEV_1_DATA_PIN 2
#define DEV_1_NUM_LEDS 24
#define DEV_2_DATA_PIN 3
#define DEV_2_NUM_LEDS 24

#define FRAMES_PER_SECOND 12

CRGB dev1_leds[DEV_1_NUM_LEDS];
CRGB dev2_leds[DEV_2_NUM_LEDS];
int dot;
int i;
int i_mod;
int gHue;
int numLeds;

void setup() {
  FastLED.addLeds<WS2812B, DEV_1_DATA_PIN>(dev1_leds, DEV_1_NUM_LEDS);
  FastLED.addLeds<WS2812B, DEV_2_DATA_PIN>(dev2_leds, DEV_2_NUM_LEDS);
  numLeds = DEV_1_NUM_LEDS + DEV_2_NUM_LEDS;
  gHue = 0;
}

void loop() {
  // Forward chase
  for(int dot = 0; dot < DEV_1_NUM_LEDS; dot++)
  {
    dev1_leds[dot] = CRGB::Red;
    FastLED.show();
    dev1_leds[dot] = CRGB::Black;
    delay(1000/FRAMES_PER_SECOND);
  }
  for(int dot = 0; dot < DEV_2_NUM_LEDS; dot++)
  {
    dev2_leds[dot] = CRGB::Red;
    FastLED.show();
    dev2_leds[dot] = CRGB::Black;
    delay(1000/FRAMES_PER_SECOND);
  }
  // Reverse chase
  for(int dot = DEV_2_NUM_LEDS - 1; dot >= 0; dot--)
  {
    dev2_leds[dot] = CRGB::Red;
    FastLED.show();
    dev2_leds[dot] = CRGB::Black;
    delay(1000/FRAMES_PER_SECOND);
  }
  for(int dot = DEV_1_NUM_LEDS - 1; dot >= 0; dot--)
  {
    dev1_leds[dot] = CRGB::Red;
    FastLED.show();
    dev1_leds[dot] = CRGB::Black;
    delay(1000/FRAMES_PER_SECOND);
  }
}


#include <FastLED.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 2
#define NUM_LEDS 24
#define ANALOG_READ_RANGE 1023
#define COLOR_TEMP_MIN 1500
#define COLOR_TEMP_MAX 6500

CRGB leds[NUM_LEDS];
int sensorPin = A0;
int pot = 0;
int colorTemp = 0;
int red = 0;
int green = 0;
int blue = 0;

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
}

void loop() {
  pot = ANALOG_READ_RANGE - analogRead(sensorPin);
  colorTemp = map(pot, 0, ANALOG_READ_RANGE, COLOR_TEMP_MIN, COLOR_TEMP_MAX);
  red = 255;
  green = map(colorTemp, COLOR_TEMP_MIN, COLOR_TEMP_MAX, 50, 250);
  blue = map(colorTemp, COLOR_TEMP_MIN, COLOR_TEMP_MAX, 0, 255);
  FastLED.clear();
  FastLED.showColor(CRGB(red,green,blue));
}

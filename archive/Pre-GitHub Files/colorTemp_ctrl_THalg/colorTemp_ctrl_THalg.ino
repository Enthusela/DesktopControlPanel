
#include <FastLED.h>
#include <math.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 2
#define MAX_TEMP 40000
#define MIN_TEMP 1500
#define NUM_LEDS 24
#define POT_RANGE 1023
#define POT_WHITE_START 0.10 // position of pot where saturation starts dropping as a percentage of sweep range

CRGB leds[NUM_LEDS];

int dot;
int ledSel;
int sensorPin = A0;
int pot = 0;
int potWhitePoint = int(float(POT_RANGE) * POT_WHITE_START);
int temp = 0;
// HSV variables
int hue = 0;
int sat = 0;
int val = 0;
// RGB variables
int led_r = 0;
int led_g = 0;
int led_b = 0;

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  
}

void loop() {
  pot = 1023 - analogRead(sensorPin);
  temp = map(pot, 0, 1023, MIN_TEMP, MAX_TEMP) / 100;
  // Set Red
  if (temp <= 66) {
    led_r = 255;
  }
  else {
    led_r = temp - 60;
    led_r = 329.698727446 * (pow(led_r, -0.1332047592));
  }
  led_r = constrain(led_r,0,255);
  // Set green
  if (temp <= 66) {
    led_g = temp;
    led_g = 99.4708025861 * log(led_g) - 161.1195681661;
  }
  else {
    led_g = temp - 60;
    led_g = 288.1221695283 * pow(led_g, -0.0755148492);
  }
  led_g = constrain(led_g,0,255);
  // Set blue
  if (temp <= 19) {
    led_b = 0;
  }
  else {
    led_b = temp - 10;
    led_b = 138.5177312231 * log(led_b) - 305.0447927307;
  }
  led_b = constrain(led_b,0,255);
  // Update LEDs
  FastLED.clear();
  FastLED.showColor(CRGB(led_r,led_g,led_b));
  delay(100);
}

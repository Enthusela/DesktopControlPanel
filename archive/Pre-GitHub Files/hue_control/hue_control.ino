
#include <FastLED.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 2
#define NUM_LEDS 24
#define POT_RANGE 1023
#define POT_WHITE_START 0.10 // position of pot where saturation starts dropping as a percentage of sweep range

CRGB leds[NUM_LEDS];

int dot;
int ledSel;
int sensorPin = A0;
int pot = 0;
int potWhitePoint = int(float(POT_RANGE) * POT_WHITE_START);
// HSV variables
int hue = 0;
int sat = 0;
int val = 0;

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  
}

void loop() {
  pot = 1023 - analogRead(sensorPin);
  hue = map(pot, 0, 1023, 5, 30);
  sat = map(max(pot - potWhitePoint, 0), 0, POT_RANGE - potWhitePoint, 255, 0);
  val = 255; //map(pot, 0, 1023, 192, 255);
  FastLED.clear();
  FastLED.showColor(CHSV(hue,sat,val));
}

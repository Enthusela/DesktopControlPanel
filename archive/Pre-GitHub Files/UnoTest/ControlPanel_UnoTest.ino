#include <Arduino.h>
#include <FastLED.h>
#include <chipsets.h>
#define HUE_CTRL_POT_PIN A5
#define ARGB1_CHIPSET WS2812
#define ARGB1_LED_NUM 26
#define ARGB1_ORDER GRB
#define ARGB1_PIN 2
#define ARGB_LED_VAL_MAX 255
#define ARGB_LED_FPS 60                    

CRGB argb1Pixels[ARGB1_LED_NUM];
CHSV argbHSV;
uint16_t argbDisplayPeriod = 1000/ARGB_LED_FPS;
uint32_t argbDisplayTimeLast = 0;
uint32_t serialWriteTimeLast = 0;

void setup() {
    setupFaceplatePinModes();
    setupLighting();
    setupSerial();
}

void loop() {
    setARGBColorValues();
    showARGBColorIfDisplayTimeElapsed();
}

void setupLighting() {
    FastLED.addLeds<ARGB1_CHIPSET,ARGB1_PIN,ARGB1_ORDER>(argb1Pixels,ARGB1_LED_NUM);
    delay(1000);
    FastLED.clear(true);
    delay(1000);
    setARGBColorValues();
    FastLED.showColor(argbHSV, ARGB_LED_VAL_MAX);
}

void setupFaceplatePinModes() {
    pinMode(HUE_CTRL_POT_PIN, INPUT);
}

void setupSerial() {
    Serial.begin(9600);
    Serial.println("Started.");
    Serial.flush();
}

void setARGBColorValues() {
    argbHSV.hue = map(analogRead(HUE_CTRL_POT_PIN), 0, 1023, 0, 255);
    argbHSV.sat = 255;
    argbHSV.val = 255;
}

void showARGBColorIfDisplayTimeElapsed() {
    if (ARGBDisplayTimeHasElapsed()) {
        FastLED.showColor(argbHSV, ARGB_LED_VAL_MAX);
    }
}

bool ARGBDisplayTimeHasElapsed() {
    static uint32_t argbDisplayTimeLast;
    const uint8_t updatesPerSecond = 60;
    const uint16_t argbDisplayPeriod = 1000/updatesPerSecond;
    if ((millis() - argbDisplayTimeLast) > argbDisplayPeriod) {
        argbDisplayTimeLast = millis();
        return true;
    }
    return false;
}

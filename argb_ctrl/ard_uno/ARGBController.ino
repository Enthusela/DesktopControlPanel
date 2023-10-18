#include "ARGBController.h"

// ARGB
CHSV argbHSV;
CRGB fanTopRearPixels[ARGB_FAN_TOP_REAR_LED_NUM];
CRGB fanTopFrontPixels[ARGB_FAN_TOP_FRONT_LED_NUM];
CRGB fanFrontTopPixels[ARGB_FAN_FRONT_TOP_LED_NUM];
CRGB fanFrontMidPixels[ARGB_FAN_FRONT_MID_LED_NUM];
CRGB fanFrontLowPixels[ARGB_FAN_FRONT_LOW_LED_NUM];
CRGB fanRearPixels[ARGB_FAN_REAR_LED_NUM];
CRGB supportPixels[ARGB_SUPPORT_LED_NUM];

//Serial
uint32_t serialWriteTimeLast = 0;

void setup() {
    setupSerial();
    setupLighting();
}

void loop() {
    getColorValuesFromSerial();
    showARGBColorIfDisplayTimeElapsed();
}

void setupSerial() {
    Serial.begin(9600);
    Serial.flush();
}

void setupLighting() {
    FastLED.addLeds<ARGB_FAN_TOP_REAR_CHIPSET,ARGB_FAN_TOP_REAR_PIN,ARGB_FAN_TOP_REAR_ORDER>(fanTopRearPixels,ARGB_FAN_TOP_REAR_LED_NUM);
    FastLED.addLeds<ARGB_FAN_TOP_FRONT_CHIPSET,ARGB_FAN_TOP_FRONT_PIN,ARGB_FAN_TOP_FRONT_ORDER>(fanTopFrontPixels,ARGB_FAN_TOP_FRONT_LED_NUM);
    FastLED.addLeds<ARGB_FAN_FRONT_TOP_CHIPSET,ARGB_FAN_FRONT_TOP_PIN,ARGB_FAN_FRONT_TOP_ORDER>(fanFrontTopPixels,ARGB_FAN_FRONT_TOP_LED_NUM);
    FastLED.addLeds<ARGB_FAN_FRONT_MID_CHIPSET,ARGB_FAN_FRONT_MID_PIN,ARGB_FAN_FRONT_MID_ORDER>(fanFrontMidPixels,ARGB_FAN_FRONT_MID_LED_NUM);
    FastLED.addLeds<ARGB_FAN_FRONT_LOW_CHIPSET,ARGB_FAN_FRONT_LOW_PIN,ARGB_FAN_FRONT_LOW_ORDER>(fanFrontLowPixels,ARGB_FAN_FRONT_LOW_LED_NUM);
    FastLED.addLeds<ARGB_FAN_REAR_CHIPSET,ARGB_FAN_REAR_PIN,ARGB_FAN_REAR_ORDER>(fanRearPixels,ARGB_FAN_REAR_LED_NUM);
    FastLED.addLeds<ARGB_SUPPORT_CHIPSET,ARGB_SUPPORT_PIN,ARGB_SUPPORT_ORDER>(supportPixels,ARGB_SUPPORT_LED_NUM);
    delay(1000);
    FastLED.clear(true);
    delay(1000);
    // Set to half-brightness white to indicate "Started, no serial received".
    argbHSV = CHSV(0, ARGB_LED_SAT_MIN, ARGB_LED_VAL_MAX / 2);
    FastLED.showColor(argbHSV, ARGB_LED_VAL_MAX);
    delay(1000);
}

void getColorValuesFromSerial() {
    if (Serial.available() >= 4 && Serial.read() == '_') {
        uint8_t colorValues[3];
        for (int i = 0; i < 3; i++) {
            colorValues[i] = uint8_t(Serial.read());
        }
        argbHSV = CHSV(colorValues[0], colorValues[1], colorValues[2]);
        Serial.flush();
    }
}

void showARGBColorIfDisplayTimeElapsed() {
    if (ARGBDisplayTimeHasElapsed()) {
        FastLED.showColor(argbHSV, ARGB_LED_VAL_MAX);
    }
}

bool ARGBDisplayTimeHasElapsed() {
    static uint32_t argbDisplayTimeLast;
    const uint16_t argbDisplayPeriod = 1000/ARGB_LED_FPS;
    if (periodHasElapsedSinceLastTime(argbDisplayTimeLast, argbDisplayPeriod)) {
        argbDisplayTimeLast = millis();
        return true;
    }
    return false;
}

bool periodHasElapsedSinceLastTime(uint32_t period, uint32_t lastTime) {
    return ((millis() - lastTime) > period);
}

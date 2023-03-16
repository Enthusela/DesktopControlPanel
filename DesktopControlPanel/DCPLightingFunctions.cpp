#include "DCPLightingFunctions.h"

CHSV colorHueToHSV(uint8_t hue) {
    return CHSV(hue, ARGB_LED_SAT_MAX, ARGB_LED_VAL_MAX);
}

CHSV colorTemperatureToHSV(uint16_t temperature) {
    /*  Best results so far hold sat at max for a small percentage of the full Kelvin range.
        This delays the color fading to white until the hue shifts towards yellow.
    */
    float kelvRange = ARGB_LED_KELV_MAX - ARGB_LED_KELV_MIN;
    float satFullRange = 0.2; // Percentage of full Kelvin range
    uint16_t satFullKelvLimit = ARGB_LED_KELV_MIN + int(kelvRange * satFullRange);
    uint8_t hueMin = 7;   // Warmer end
    uint8_t hueMax = 42;//35;  // Cooler end
    // Get control pot values
    uint8_t hue = map(temperature, ARGB_LED_KELV_MIN, ARGB_LED_KELV_MAX, hueMin, hueMax);
    uint8_t sat = ARGB_LED_SAT_MAX;
    if (temperature > satFullKelvLimit) {
        sat = map(temperature, satFullKelvLimit, ARGB_LED_KELV_MAX, ARGB_LED_SAT_MAX, 0);
    }
    uint8_t val = scaleValBySat(ARGB_LED_VAL_MAX, sat);
    return CHSV(hue, sat, val);                                                                                                                                                           
}

// Reduces val as sat -> 0 to reduce yellowing of output light
uint8_t scaleValBySat(uint8_t val, uint8_t sat) {
    int minSatForMaxVal = 128;
    if (sat >= minSatForMaxVal) {
        return val;
    }
    else {
        // As sat decreases from valScaleSttSat -> 0, decrease val -> zeroSatValLimit
        uint8_t maxValAtZeroSat = 100;
        uint8_t max_val = map(sat, 0, minSatForMaxVal, maxValAtZeroSat, ARGB_LED_SAT_MAX);
        return min(val, max_val);
    }
}
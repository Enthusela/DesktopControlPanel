#include "SoftTakeoverLEDs.h"

SoftTakeoverLEDs::SoftTakeoverLEDs(
    uint8_t n,
    int16_t pMin,
    int16_t pMax,
    int pin0,
    ...)
    : ledNum(n),
    posMin(pMin),
    posMax(pMax)
{
    ledSpacing = (posMax - posMin) / (ledNum - 1);
    posRange = abs(posMax - posMin);
    va_list vl;
    va_start(vl, pin0);
    for (int i = 0; i < ledNumMax; i++) {
        if (i >= ledNum) {
            LEDPins[i] = 0;
        }
        else {
            if (i == 0) {
                LEDPins[i] = pin0;
            }
            else if (i < ledNum) {
                LEDPins[i] = va_arg(vl, int);
            }
            pinMode(LEDPins[i], OUTPUT);
        }
    }
}

uint8_t SoftTakeoverLEDs::getClosestLED(int16_t pos) {
    // Default to first LED
    uint8_t closestLED = 0;
    uint16_t shortestDist = abs(pos - posMin);
    // Check distances to remaining LEDs
    for (uint8_t i = 1; i < ledNum; i++) {
        int16_t ledPos = posMin + ledSpacing * i; // 240 + (-300 * 1) = -60
        uint16_t dist = abs(pos - ledPos); // abs(-7 - (-60)) = 53 < 242
        if (dist < shortestDist) {
            shortestDist = dist;
            closestLED = i;
        }
    }
    return closestLED;
}

// Turns on LED closest to required pot position and increases brightness as pot gets closer.
void SoftTakeoverLEDs::setLEDs(bool active, int16_t posCurrent=0, int16_t posTarget=0) {
    for (int i = 0; i < ledNum; i++) {
        analogWrite(LEDPins[i], 0);
    }
    if (active) { // Get position and brightness of active LED
        float distToTargetPct = min(1.0, float(abs(posTarget - posCurrent)) / float(posRange));
        uint8_t brightness = float(brightnessRange) * (1.0 - distToTargetPct) + brightnessMin;
        uint8_t activeLED = getClosestLED(posTarget);
        analogWrite(LEDPins[activeLED], brightness);
    }
}
#include "DesktopControlPanel.h"

#ifndef SOFT_TKOVR_LEDS_H
#define SOFT_TKOVR_LEDS_H

class SoftTakeoverLEDs {
    public:
        SoftTakeoverLEDs::SoftTakeoverLEDs(
            uint8_t n,
            int16_t pMin,
            int16_t pMax,
            int p0,
            ...);
        uint8_t getClosestLED(int16_t angle);
        void setLEDs(bool active, int16_t posCurrent=0, int16_t posTarget=0);
    private:
        int8_t brightnessMax = STO_LED_MAX_BRIGHTNESS;
        int8_t brightnessMin = STO_LED_MIN_BRIGHTNESS;
        int8_t brightnessRange = abs(brightnessMax - brightnessMin);
        uint8_t ledNum;
        const uint8_t ledNumMax = STO_LEDS_MAX;
        int LEDPins[STO_LEDS_MAX];
        int16_t ledSpacing; // Angular spacing between soft takeover LEDs
        int16_t posMin;     // Angular position of CCW-most soft takeover LED
        int16_t posMax;     // Angular position of CW-most soft takeover LED
        uint16_t posRange;
};

#endif
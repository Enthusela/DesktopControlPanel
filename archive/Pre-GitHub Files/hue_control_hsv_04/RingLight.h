#include "includes.h"

#ifndef RING_LIGHT_H
#define RING_LIGHT_H

class RingLight {
    public:
        RingLight (int* pins, int pos);
        void setTakeoverMode (bool b) { takeoverMode = b; }
        void setTakeoverValues (int pos, int target);
    private:
        int LED1Pos;
        int LEDSpacing;
        int LEDs[FP_RINGLIGHT_LEDS_MAX];
        int numLEDs;
        bool takeoverMode;
};

#endif
#include "RingLight.h"

RingLight::RingLight (int* pins, int pos)
    : LED1Pos(pos)
    {
        int i;
        numLEDs = sizeOfArray_int(pins);
        LEDSpacing = CIRCLE_DEGREES / numLEDs;
        for (i = 0; i < numLEDs; i++) {
            pinMode(LEDs[i], OUTPUT);
        }
    }
void RingLight::setTakeoverValues (int pos, int target) {
    //TODO
    //analogWrite(pin,value)
}
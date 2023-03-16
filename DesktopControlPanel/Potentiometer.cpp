#include "Potentiometer.h"

Potentiometer::Potentiometer(
            int p,
            uint16_t vMax,
            int16_t aMin,
            int16_t aMax,
            bool log=false)
    : pin(p),
    valueMax(vMax),
    angleMin(aMin),
    angleMax(aMax),
    logarithmic(log)
    {
        pinMode(pin, INPUT);
    }

int16_t Potentiometer::getAngle() {
    return map(value, 0, valueMax, angleMin, angleMax);
}

int16_t Potentiometer::getAngleMin() {
    return angleMin;
}

int16_t Potentiometer::getAngleMax() {
    return angleMax;
}

uint16_t Potentiometer::getValue() {
    value = getDenoisedPinReading();
    convertValueIfLogarithmicPot();
    return value;
}

uint16_t Potentiometer::getDenoisedPinReading() {
    uint16_t reading = analogRead(pin);
    if (abs(reading - value) < noiseThld) {
        return value;
    }
    return reading;
}

uint16_t Potentiometer::convertValueIfLogarithmicPot() {
    if (logarithmic) {
        value = logToLinear(value, float(valueMax));
    }
}

uint16_t Potentiometer::getValueMax() {
    return valueMax;
}

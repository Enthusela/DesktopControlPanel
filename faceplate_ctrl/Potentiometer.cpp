#include "Potentiometer.h"

Potentiometer::Potentiometer(
            int p,
            uint16_t vMax,
            int16_t aMin,
            int16_t aMax,
            bool log=false,
            Adafruit_DCMotor *mtr=NULL)
    : pin(p),
    valueMax(vMax),
    angleMin(aMin),
    angleMax(aMax),
    logarithmic(log),
    motor(mtr)
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

void Potentiometer::moveToValue(uint16_t v) {
    if (motor == NULL || abs(v - value) <= diffThld) {
        movingToValue = false;
    } else {
        targetValue = v;
        movingToValue = true;
    }
}

void Potentiometer::moveMotor() {
    if (motor == NULL) {
        return;
    }
    if (movingToValue) {
        int16_t diff = targetValue - getValue();
        if (abs(diff) <= diffThld) {
            motor->setSpeed(0);
            motor->run(RELEASE);
            movingToValue = false;
        } 
        else {
            if (diff >= 0) {
                motor->run(BACKWARD);
            } else {
                motor->run(FORWARD);
            }
            uint8_t motorSpeed = map(abs(diff), 0, valueMax, 42, 196);
            motor->setSpeed(motorSpeed);
        }
    }
}

bool Potentiometer::moving() {
    return movingToValue;
}
#include "ControlEncoder.h"

ControlEncoder::ControlEncoder(uint8_t clk, uint8_t dt, uint8_t sw)
    : clkPin(clk), dtPin(dt), swPin(sw)
    {
        pinMode(clkPin, INPUT);
        pinMode(dtPin, INPUT);
        pinMode(swPin, INPUT);
        clkPinStateLast = digitalRead(clkPin);
        swButtonStateLast = digitalRead(swPin);
        swPinDebounceTimeLast = 0;
        value = 0;
    }

uint16_t ControlEncoder::getValue() {
    return value;
}

void ControlEncoder::updateValue() {
    bool clkPinState = digitalRead(clkPin);
    bool dtPinState = digitalRead(dtPin);
    readButtonDebounced(&swButtonState, &swPin, &swPinReadLast, &swPinDebounceTimeLast);    
    if (swButtonState = LOW) {
        value = 0;
    }
    else if (clkPinState != clkPinStateLast) {
        if (dtPinState != clkPinState) {
            value++; // Encoder rotating clockwise
        }
        else {
            value--; // Encoder rotating counter-clockwise
        }
    }
    clkPinStateLast = clkPinState;
    swButtonStateLast = swButtonState;
}
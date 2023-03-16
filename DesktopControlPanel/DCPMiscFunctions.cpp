#include "DCPMiscFunctions.h"

// Returns linearised approximation of logarithmic potentiometer output
uint16_t logToLinear(uint16_t value, float valueMax) {
    return round(map(sqrt(value), 0.0, sqrt(valueMax), 0.0, valueMax));
}

uint8_t mapInt10bTo8b(uint16_t i) {
    return map(i, 0, 1023, 0, 255);
}

// Generic button debounce function for use by one or more button objects
void readButtonDebounced (bool *buttonState, uint8_t *pin, bool *pinStateLast, uint32_t *debounceTimeLast) {
	uint8_t debounceDelay = 50; // debounce time in ms
    bool pinRead = digitalRead(*pin);
    if (pinRead != *pinStateLast) {
        *debounceTimeLast = millis();
    }
    if ((millis() - *debounceTimeLast) > debounceDelay) {
        *buttonState = pinRead;
    }
    *pinStateLast = pinRead;
}

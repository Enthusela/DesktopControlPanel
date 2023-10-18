#include "ControlButtonMultiMode.h"

ControlButtonMultiMode::ControlButtonMultiMode (uint8_t p, uint8_t mN)
    : pin(p), modeNum(mN)
    {
        pinMode(pin, INPUT_PULLUP);
        updateMode();
    }

uint8_t ControlButtonMultiMode::getMode () {
    return mode;
}

void ControlButtonMultiMode::updateMode() { // Increment value when switch is pressed
    readButtonDebounced(&buttonState, &pin, &pinStateLast, &debounceTimeLast);
    if (buttonState != buttonStateLast && buttonState == LOW) {
        if (mode == modeNum - 1) {
            mode = 0;
        }
        else {
            mode++;
        }
    }
    buttonStateLast = buttonState;
}
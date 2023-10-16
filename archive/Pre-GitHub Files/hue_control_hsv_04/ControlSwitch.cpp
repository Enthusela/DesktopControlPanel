#include "ControlSwitch.h"

ControlSwitch::ControlSwitch (int positions, int pin)
    : numPositions(positions), inputPin(pin)
    {
        int i;
        for (i = 0; i < FP_SWITCH_KNOBS_MAX; i++) {
            controlKnobs[i] = nullptr;
        }
    }

void ControlSwitch::attachControlKnob (ControlKnob* ck) {
    int i = 0;
    bool attached = false;
    while (attached == false && i < FP_SWITCH_KNOBS_MAX) {
        if (controlKnobs[i] != nullptr) {
            i++;
        }
        else {
            controlKnobs[i] = ck;
            attached = true;
        }
    }
}

void ControlSwitch::updateControlKnobModes () {
    int i = 0;
    ControlKnob* ck = controlKnobs[0];
    while (ck != nullptr && i < FP_SWITCH_KNOBS_MAX) {
        controlKnobs[i]->setMode(position);
        i++;
    }
}

void ControlSwitch::removeControlKnob(ControlKnob* ck = nullptr, int ckIndex = 0) {
    int i;
    if (ck != nullptr) {
        
    }
}

TactileSwitch::TactileSwitch (int positions, int pin, int dbncDly)
    : ControlSwitch(positions, pin), debounceDelay(dbncDly)
    {
        pinMode(inputPin, INPUT_PULLUP); // Tactile switch pressed = LOW output
        switchState = HIGH;
        lastDebounceTime = 0;
    }

void TactileSwitch::updatePosition () {
    bool currentSwitchState;
    bool lastSwitchState;
    lastSwitchState = switchState;
    currentSwitchState = digitalRead(inputPin);
    if (currentSwitchState != lastSwitchState) {
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        switchState = currentSwitchState;
        if (switchState == LOW) {
            position++;
            if (position == numPositions) {
                position = 0;
            }
            updateControlKnobModes();
        }
    }

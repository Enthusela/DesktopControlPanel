# include "ToggleSwitch.h"

ToggleSwitch::ToggleSwitch(
    int tPin,
    int opOnPin,
    int opOffPin)
    : togglePin(tPin),
    optoOnPin(opOnPin),
    optoOffPin(opOffPin)
{
    pinMode(togglePin, INPUT_PULLUP);
    pinMode(optoOnPin, OUTPUT);
    pinMode(optoOffPin, OUTPUT);
    toggleStateLast = !digitalRead(togglePin);
    digitalWrite(optoOnPin, LOW);
    digitalWrite(optoOffPin, LOW);
    toggleMillis = millis();
    optoPulseLength = OPTO_PULSE_LENGTH;
}


void ToggleSwitch::set() {
    bool toggleState = !digitalRead(togglePin);
    if (toggleState != toggleStateLast) {
        toggleMillis = millis();
        if (toggleState == HIGH) {
            digitalWrite(optoOnPin, HIGH);
        }
        else {
            digitalWrite(optoOffPin, HIGH);
        }
    }
    if ((toggleMillis - millis()) > optoPulseLength) {
        digitalWrite(optoOnPin, LOW);
        digitalWrite(optoOffPin, LOW);
    }
    toggleStateLast = toggleState;
}

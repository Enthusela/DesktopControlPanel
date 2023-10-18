#include "ControlPot.h"

ControlPot::ControlPot(
    Potentiometer *p,
    uint16_t vMin,
    uint16_t vMax,
    SoftTakeoverLEDs *stol=NULL)
    : pot(p),
    valueMin(vMin),
    valueMax(vMax),
    LEDs(stol)
{    
}

bool ControlPot::getActive() {
    return active;
}

bool ControlPot::setActive(bool a) {
    if (!active && a) {
        sendPotPosition();
    }
    active = a;
    return active;
}

void ControlPot::sendPotPosition() {
    uint16_t targetValue = map(value, valueMin, valueMax, 0, pot->getValueMax());
    pot->moveToValue(targetValue);
}

int16_t ControlPot::getValue() {
    if (active && !pot->moving()) {
        setValue();
    }
    return value;
}

void ControlPot::setValue() {
    value = map(pot->getValue(), 0, pot->getValueMax(), valueMin, valueMax);
}

// Returns current or previous value when passed false or true, respectively.
// TODO: integrate ControlSwitch class into ControlPot to eliminate the need for ctrlModeReactivated
int16_t ControlPot::getValueSTO(bool ctrlModeReactivated=false) {
    int16_t valueNew = map(pot->getValue(), 0, pot->getValueMax(), valueMin, valueMax);
    softTakeoverLatched = ((ctrlModeReactivated && (valueNew != value)) || softTakeoverLatched);
    // TODO: make this function Do One Thing
    if (softTakeoverLatched) {
        if (abs(valueNew - value) <= softTakeoverDisableThld) {
            softTakeoverLatched = false;
        }
        if (LEDs != NULL) {
            int16_t potAngleTarget = map(value, valueMin, valueMax, pot->getAngleMin(), pot->getAngleMax());
            int16_t potAngleCurrent = pot->getAngle();
            LEDs->setLEDs(true, potAngleCurrent, potAngleTarget);
        }
    }
    if (!softTakeoverLatched) {
        value = valueNew;
        // TODO: Consolidate double if-leds-null checks
        if (LEDs != NULL) {
            LEDs->setLEDs(false);
        }
    }
    return value;
}

int16_t ControlPot::getValueMax() {
    return valueMax;
}

float ControlPot::getValuePercent() {
    return float(value) / float(valueMax);
}
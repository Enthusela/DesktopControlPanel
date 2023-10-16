#include "ControlKnob.h"

ControlKnob::ControlKnob (int pin, int range = 0, int ref = 0, RingLight* rL = nullptr, ControlSwitch* sw = nullptr) 
    : active(true), inputPin(pin), mode(0), modeSwitch(sw), numModes(1), posRange(range), posRef(ref), ringLight(rL)
    {
        int i;
        numModes = sizeof(modeValues) / sizeof(modeValues[0]);
        initArrayToZero_int(modeValues);
        pinMode(inputPin, INPUT);
        value = analogRead(inputPin);
        if (modeSwitch != nullptr) {
            attachControlSwitch(sw);
        }
    }

void ControlKnob::attachControlSwitch (ControlSwitch* sw) {
    int i;
    modeSwitch = sw;
    mode = modeSwitch->getPosition();
    numModes = modeSwitch->getNumPositions();
    initArrayToZero_int(modeValues);
    modeValues[mode] = value;
    modeSwitch->attachControlKnob(this);
}

void ControlKnob::setMode (int modeSel) { // to be called by an instance of Switch
    int modePrev;
    if (modeSel < 0 || modeSel > numModes) {
        active = false;
    }
    else {
        active = true;
        modePrev = mode;
        mode = modeSel;
        if (mode != modePrev && value != modeValues[mode]){
            setTakeover(true);            
        }
        else {
            setTakeover(false);
        }
    }
}

void ControlKnob::setTakeover(bool b) {
    bool prev;
    prev = takeoverActive;
    takeoverActive = b;
    if (ringLight != nullptr) {
        ringLight->setTakeoverMode(b);
    }
    if (takeoverActive != prev && takeoverActive == true) {
        updateValue();
    }
}

void ControlKnob::updateTakeoverValues () {
    int pos;
    int posTarget;
    int valueTarget;
    valueTarget = modeValues[mode];
    if (value == valueTarget) {
        setTakeover(false);
    }
    else if (ringLight != nullptr) {
        pos = (pos / ANALOG_RANGE * posRange) + posRef;
        posTarget = (posTarget / ANALOG_RANGE * posRange) + posRef;
        ringLight->setTakeoverValues(pos, posTarget);
    }
}

void ControlKnob::updateValue () {
    int prev;
    if(active) { // Do not update if last setMode input was out of range
        prev = value;
        value = analogRead(inputPin);
        if (value != prev) {
            if (takeoverActive) {
                updateTakeoverValues();
            }
            if (!takeoverActive) { // The above IF block may set takeoverActive false
                modeValues[mode] = value;
            }        
        }
    }
}
#include "includes.h"

#ifndef CONTROL_SWITCH_H
#define CONTROL_SWITCH_H

class ControlSwitch {
    public:
        ControlSwitch (int positions, int pin);
        void attachControlKnob (ControlKnob* ck);//TODO: define array-handling fns { controlKnobs.push_back(ck); }
        int getNumPositions() { return numPositions; }
        int getPosition () { return position; }
        void removeControlKnob(ControlKnob* ck = nullptr, int ckIndex = 0);
    protected:
        ControlKnob* controlKnobs[FP_SWITCH_KNOBS_MAX];
        int inputPin;
        int numPositions;
        int position;
        virtual void updatePosition () = 0;
        void updateControlKnobModes ();
};

class TactileSwitch : public ControlSwitch {
    // Tactile momentary switch class that can cycle through multiple modes
    // Inherited "position" member indicates active mode, not physical switch position
    public:
        TactileSwitch (int positions, int pin, int dbncDly);
        void updatePosition ();
    private:
        bool switchState;
        int lastDebounceTime;
        int debounceDelay;
};

#endif
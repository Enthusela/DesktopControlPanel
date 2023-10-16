#include "includes.h"

#ifndef CONTROL_KNOB_H
#define CONTROL_KNOB_H

class ControlKnob {
    public:
        ControlKnob (int p, int pRng = 0, int pRef = 0, RingLight* rl = nullptr, ControlSwitch* sw = nullptr);
        void attachControlSwitch (ControlSwitch* sw) { modeSwitch = sw; }
        int getMode () { return mode; }
        int getModeValue(int modeSel) { return modeValues[modeSel]; }
        void setMode (int modeSel);
        void updateValue();
    private:  
        bool active;
        int inputPin;
        int mode;
        ControlSwitch* modeSwitch;
        int modeValues[FP_KNOB_MODES_MAX];
        int numModes;
        int posRange;
        int posRef;
        RingLight *ringLight;
        bool takeoverActive;
        int value;
        // int valueLast;
        void setTakeover(bool state);
        void updateModeValues ();
        void updateTakeoverValues ();
};

#endif
#include "DesktopControlPanel.h"

#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

class ToggleSwitch {
    public:
        ToggleSwitch::ToggleSwitch(
            int tPin,
            int opOnPin,
            int opOffPin);
        void set();
    private:
        int togglePin;
        int optoOnPin;
        int optoOffPin;
        bool toggleStateLast;
        uint64_t toggleMillis;
        uint64_t optoPulseLength;
};

#endif

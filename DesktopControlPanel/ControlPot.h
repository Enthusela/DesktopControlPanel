#include "DesktopControlPanel.h"

#ifndef CONTROLPOT_H
#define CONTROLPOT_H

class ControlPot {
    public:
        ControlPot::ControlPot(
            Potentiometer *p,
            uint16_t vMin,
            uint16_t vMax,
            SoftTakeoverLEDs *stol=NULL);
        int16_t getValue(bool ctrlModeReactivated=false);
        int16_t getValueMax();
        float getValuePercent();
    private:
        Potentiometer *pot;
        SoftTakeoverLEDs *LEDs;
        bool softTakeoverLatched = false;
        int16_t ledAngleMax;     // Angular position of CW-most soft takeover LED
        int16_t ledAngleMin;     // Angular position of CCW-most soft takeover LED
        int16_t ledAngleSpacing; // Angular spacing between soft takeover LEDs
        uint8_t ledNum;
        int inputPin;
        uint8_t softTakeoverDisableThld = STO_DISABLE_THRESHOLD;
        int16_t value;
        int16_t valueMax;
        int16_t valueMin;
        int16_t valuePrev;
};

#endif
#include "DesktopControlPanel.h"

#ifndef CONTROLENCODER_H
#define CONTROLENCODER_H

class ControlEncoder {
    public:
        ControlEncoder(uint8_t clk, uint8_t dt, uint8_t sw);
        uint16_t getValue();
        void updateValue();
    private:
        uint8_t clkPin;
        bool clkPinStateLast;
        uint8_t dtPin;
        bool swButtonState;
        bool swButtonStateLast;
        uint8_t swPin;
        uint32_t swPinDebounceTimeLast;
        bool swPinReadLast;
        uint16_t value;
};

#endif
#include "DesktopControlPanel.h"

#ifndef CONTROLBUTTONMULTIMODE_H
#define CONTROLBUTTONMULTIMODE_H

class ControlButtonMultiMode {
    public:
        ControlButtonMultiMode (uint8_t p, uint8_t s);
        uint8_t getMode();
        void updateMode();
    private:
        void readPin();        
        bool buttonState;
        bool buttonStateLast;
        uint32_t debounceTimeLast;
        uint8_t modeNum;
        uint8_t mode;
        uint8_t pin;
        bool pinStateLast;
};

#endif
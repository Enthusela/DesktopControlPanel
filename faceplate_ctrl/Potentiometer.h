#include "DesktopControlPanel.h"

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

class Potentiometer {
    public:
        Potentiometer::Potentiometer(
            int p,
            uint16_t valueMax,
            int16_t angleMin,
            int16_t angleMax,
            bool log=false,
            Adafruit_DCMotor *mtr=NULL);        
        int16_t getAngle();
        int16_t getAngleMin();
        int16_t getAngleMax();
        uint16_t getValue();
        uint16_t getValueMax();
        void moveToValue(uint16_t v);
        bool moving();
        void moveMotor();
    private:
        uint16_t getDenoisedPinReading();
        uint16_t convertValueIfLogarithmicPot();
        // All angular values in degrees, measured CCW from 0 at right-hand horizontal
        int pin;
        int16_t angleMin;    // Angular position of pot at zero
        int16_t angleMax;    // Angular position of pot at max
        const uint8_t noiseThld = 2;
        uint16_t value;
        uint16_t valueMax;
        bool logarithmic;
        Adafruit_DCMotor *motor;
        bool softTakeoverLatched = false;
        bool movingToValue = false;
        uint16_t targetValue = 0;
        uint16_t diffThld = 5;

};

#endif
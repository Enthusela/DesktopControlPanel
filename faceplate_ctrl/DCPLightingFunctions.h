#ifndef DCP_LIGHTING_FUNCTIONS_H
#define DCP_LIGHTING_FUNCTIONS_H

#include "DesktopControlPanel.h"

CHSV colorHueToHSV(uint8_t hue);
CHSV colorTemperatureToHSV(uint16_t temperature);
uint8_t scaleValBySat(uint8_t val, uint8_t sat);

#endif
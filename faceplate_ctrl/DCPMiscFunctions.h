#include "DesktopControlPanel.h"

#ifndef MISCFUNCTIONS_H
#define MISCFUNCTIONS_H

uint16_t logToLinear(uint16_t value, float valueMax);
uint8_t mapInt10bTo8b(uint16_t i);
void readButtonDebounced (bool *buttonState, uint8_t *pin, bool *pinStateLast, uint32_t *debounceTimeLast);

#endif
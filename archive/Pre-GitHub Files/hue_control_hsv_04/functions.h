#include "includes.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

CHSV colorTemperature (int potPos);

void initArrayToZero_int(int* arr);

CHSV kelvinToHSV(int kelv);

void lcdShowColorTemp (LiquidCrystal lcd, int k, CHSV hsv, float vScale);

bool readButtonDebounced (int pin);

int readPot (int pin);

int sizeOfArray_int (int* arr);

int valueToKelvin (int v);

int valueToHSVscale (int v);

#endif
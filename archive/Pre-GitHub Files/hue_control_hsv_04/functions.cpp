#include "includes.h"

CHSV colorTemperature (int potPos) {
  CHSV HSV;
  int hueMin = 7;   // Warmer end
  int hueMax = 35;  // Cooler end
  int kelv;
  int kelvMax = 6500;
  int kelvMin = 1500;
  int kelvRange = kelvMax - kelvMin;
  int maxHSV = 255;
  float satFullRange = 0.1; // Percentage of full Kelvin range
  int satFullKelvLimit = kelvMin + int(kelvRange * satFullRange); 
  int satDecrKelvRange = kelvRange - satFullKelvLimit;


  kelv = map(potPos, 0, ANALOG_RANGE, kelvMin, kelvMax);
  HSV.hue = map(kelv, kelvMin, kelvMax, hueMin, hueMax);
  HSV.sat = map(max(kelv,satFullKelvLimit), satFullKelvLimit, kelvMax, maxHSV, 0);
  HSV.val = maxHSV;
  return HSV;
}

void initArrayToZero_int (int* arr) {
    int i;
    for (i = 0; i < (sizeof(arr)/sizeof(arr[0])); i++) {
      arr[i] = 0;
    }
}

CHSV kelvinToHSV (int kelv) {
  // Function to convert color temperature in Kelvin to an HSV pixel value.
  // Best results so far hold sat at max for a small percentage of the full Kelvin range.
  // This delays the color fading to white until the hue shifts towards yellow.
  CHSV HSV;
  float kelvRange = ARGB_LED_KELV_MAX - ARGB_LED_KELV_MIN;
  float satFullRange = 0.1; // Percentage of full Kelvin range
  int   satFullKelvLimit = ARGB_LED_KELV_MIN + int(kelvRange * satFullRange); 
  int   satDecrKelvRange = kelvRange - satFullKelvLimit;
  int   hueMin = 7;   // Warmer end
  int   hueMax = 35;  // Cooler end
  
  // HSV.hue = map(kelv, ARGB_LED_KELV_MIN, ARGB_LED_KELV_MAX, hueMin, hueMax);
  // HSV.sat = map(max(kelv,satFullKelvLimit), satFullKelvLimit, ARGB_LED_KELV_MAX, ARGB_LED_SAT_MAX, 0);
  // HSV.val = ARGB_LED_VAL_MAX;
  return HSV;
}

void lcdShowColorTemp (LiquidCrystal lcd, int k, CHSV hsv, float vScale) {
  static int vScaleLast;
  static char vScaleStr[4];
  char  lcdText[LCD_ROW_NUM][LCD_COL_NUM];
  int i;
  // Only perform the vScale conversion if value has changed
  if (vScale != vScaleLast) {
    sprintf(vScaleStr, "VAL x%d.%d%d",int(vScale),int(vScale*10)%10,int(vScale*100)%10);
  }
  sprintf(lcdText[0],"%4iK ",k);
  sprintf(lcdText[6], vScaleStr);
  sprintf(lcdText[1],"h%3i s%3i v%3i",hsv.h,hsv.s,hsv.v);
  for (i = 0; i < LCD_ROW_NUM; i++) {
    lcd.setCursor(0,i);
    lcd.print(lcdText[i]);
  }
  vScaleLast = vScale;
}

int readPot (int pin) {
  return analogRead(pin);
}

int sizeOfArray_int (int* arr) {
  return sizeof(arr) / sizeof(arr[0]);
}

int valueToHSVscale (int v) {
  return map(v, 0, ANALOG_RANGE, 0, HSV_SCALE_MAX);
}

int valueToKelvin (int v) {
  return map(v, 0, ANALOG_RANGE, ARGB_LED_KELV_MIN, ARGB_LED_KELV_MAX);
}

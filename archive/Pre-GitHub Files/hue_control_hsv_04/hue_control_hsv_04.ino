#include "includes.h"
#include "defines.h"

#define FILE_NAME "Control Panel"
#define FILE_VERSION "Version 0.4"

int devMillisLast = 0;
int devRefreshTime = 1000/ARGB_LED_FPS;
int lcdMillisLast = 0;
int lcdRefreshTime = 1000/LCD_FPS;
char lcdText[LCD_ROW_NUM][LCD_COL_NUM];
int kelv = 0;
int kelvPin = A0;
float valScale = 0;
int valScalePin = A1;

// LCD display ini
LiquidCrystal lcd(12,11,5,4,3,2);
// RGB device ini
//TODO: locate or define a Device class to reduce number of #defines
//TODO: rename Device classes to "rgbDev[n]". Names remain "dev[n]LEDs" while CRGB (led) class is in use.
CRGB argb1LED[ARGB1_LED_NUM];
CRGB argb2LED[ARGB2_LED_NUM];
CRGB argb3LED[ARGB3_LED_NUM];
CHSV argbHSV;

TactileSwitch swHueBright(2, 24);
ControlKnob ckHueBright(A0, ARDUINO_POT_POSRNG, ARDUNIO_POT_POSREF, nullptr, &swHueBright);

void setup() {
  lcd.begin(LCD_COL_NUM,LCD_ROW_NUM);
  lcd.clear();
  delay(3000);
  //FastLED.addLeds<ARGB1_LED_TYPE,ARGB1_PIN_DATA,ARGB1_LED_COLOR_ORDER>(argb1LED,ARGB1_LED_NUM);
  lcd.print(FILE_NAME);
  lcd.setCursor(0,1);
  lcd.print(FILE_VERSION);
  delay(1000);
}

void loop() {
  swHueBright.updatePosition();
  ckHueBright.updateValue();
  // Set argbHSV value based on Hue/Brightness control knob values
  /*/ TODO: replace hard-coded getModeValue calls with meaningful 
            defines or variables
  */
  kelv = valueToKelvin(ckHueBright.getModeValue(0));
  valScale = valueToHSVscale(ckHueBright.getModeValue(1));
  argbHSV = kelvinToHSV(kelv);
  argbHSV.val = int(float(argbHSV.val) * valScale);
  // Refresh ARGB devices
  // if (millis() - devMillisLast > devRefreshTime) {
  //   argbHSV = kelvinToHSV(kelv);
  //   argbHSV.val = int(float(argbHSV.val) * valScale);
  //   FastLED.clear();
  //   FastLED.showColor(argbHSV);
  //   devMillisLast = millis();
  // }
  // Update LCD display
  if (millis() - lcdMillisLast > lcdRefreshTime) {
    lcdShowColorTemp (lcd, kelv, argbHSV, valScale);
    lcdMillisLast = millis();
  }
}





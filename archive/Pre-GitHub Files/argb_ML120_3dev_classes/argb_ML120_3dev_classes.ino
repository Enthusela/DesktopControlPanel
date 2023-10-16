
#include <FastLED.h>
#include <chipsets.h>
#include <LiquidCrystal.h>

#define FILE_NAME "argb_ML240_3dvc"
#define FILE_VERSION "Version 0.1"
#define TRUE  1
#define FALSE 0
// Default device #defines
#define ANALOG_RANGE 1023
#define DEV_LED_COLOR_ORDER GRB
#define DEV_LED_FPS 60
#define DEV_LED_HUE_MAX 255
#define DEV_LED_KELV_MIN 1500 // "kelv" used for temperature to avoid confusion at "temp" values
#define DEV_LED_KELV_MAX 6500 //Kelvin
#define DEV_LED_SAT_MAX 255
#define DEV_LED_TYPE WS2812B
#define DEV_LED_VAL_MAX 255
//Individual device #defines
// Fan 1
#define FAN1_LED_COLOR_ORDER DEV_LED_COLOR_ORDER
#define FAN1_LED_NUM 24
#define FAN1_LED_TYPE DEV_LED_TYPE
#define FAN1_PIN_DATA 22
// Fan 2
#define FAN2_LED_COLOR_ORDER DEV_LED_COLOR_ORDER
#define FAN2_LED_NUM 24
#define FAN2_LED_TYPE DEV_LED_TYPE
#define FAN2_PIN_DATA 23
// Pump
#define PUMP_LED_COLOR_ORDER DEV_LED_COLOR_ORDER
#define PUMP_LED_NUM 12
#define PUMP_LED_TYPE DEV_LED_TYPE
#define PUMP_PIN_DATA 24
// LEDs
#define DOT_FPS 13
#define HSV_MAX_VAL 255
// LCD display
#define LCD_COL_NUM 16
#define LCD_ROW_NUM 2
#define LCD_FPS 10

class ARGB_Device {
  private:
    CLEDController &controller;
    int numLeds;
    CRGB *leds;
  public:
    ARGB_Device (ESPIChipsets chipset, uint8_t dataPin, EOrder rgbOrder, int n) {
      numLeds = n;
      CRGB led_array[numLeds];
      leds = led_array;
      FastLED.addLeds<PUMP_LED_TYPE,PUMP_PIN_DATA,PUMP_LED_COLOR_ORDER>(leds,n);
      controller = FastLED[FastLED.count()];
    }

};

int   devMillisLast = 0;
int   devRefreshTime = 1000/DEV_LED_FPS;
int   dot = 0;
bool  dotIncreasing = TRUE;
int   dotMillisLast = 0;
int   dotRefreshTime = 1000/DOT_FPS;
int   i;
char  lcdText[LCD_ROW_NUM][LCD_COL_NUM];
int   lcdMillisLast = 0;
int   lcdRefreshTime = 1000/LCD_FPS;
int   kelv = 0;
int   kelvPin = A0;
float valScale = 0;
int   valScalePin = A1;

// LCD display ini
LiquidCrystal lcd(12,11,5,4,3,2);
// RGB device ini
//TODO: locate or define a Device class to reduce number of #defines
//TODO: rename Device classes to "rgbDev[n]". Names remain "dev[n]LEDs" while CRGB (led) class is in use.
CRGB fan1LED[FAN1_LED_NUM];
CRGB fan2LED[FAN2_LED_NUM];
CRGB pumpLED[PUMP_LED_NUM];
ARGB_Device pump <PUMP_LED_TYPE,PUMP_PIN_DATA,PUMP_LED_COLOR_ORDER>(pumpLED,PUMP_LED_TYPE,PUMP_PIN_DATA,PUMP_LED_COLOR_ORDER);
CHSV devHSV;

void setup() {
  lcd.clear();
  delay(3000);
//  FastLED.addLeds<PUMP_LED_TYPE,PUMP_PIN_DATA,PUMP_LED_COLOR_ORDER>(pumpLED,PUMP_LED_NUM);
//  pump.setController(FastLED[FastLED.count()]);
//  pump.setLEDs(pumpLED);
//  FastLED.addLeds<FAN1_LED_TYPE,FAN1_PIN_DATA,FAN1_LED_COLOR_ORDER>(fan1LED,FAN1_LED_NUM);
//  FastLED.addLeds<FAN2_LED_TYPE,FAN2_PIN_DATA,FAN2_LED_COLOR_ORDER>(fan2LED,FAN2_LED_NUM);

  lcd.begin(LCD_COL_NUM,LCD_ROW_NUM);
  lcd.print(FILE_NAME);
  lcd.setCursor(0,1);
  lcd.print(FILE_VERSION);
  delay(1000);
}

void loop() {
  // Update dot position
  if (millis() - dotMillisLast > dotRefreshTime) {
    if (dotIncreasing) {
      if (dot < FAN1_LED_NUM - 1) { dot++; }
      else { dotIncreasing = FALSE; }
    }
    else {
      if (dot > 0) { dot--; }
      else { dotIncreasing = TRUE; }
    }
    dotMillisLast = millis();
  }
  // Update LED temperature/brightness
  kelv = map(analogRead(kelvPin),0,ANALOG_RANGE,DEV_LED_KELV_MIN,DEV_LED_KELV_MAX);
  valScale = float(map(analogRead(valScalePin),0,ANALOG_RANGE,0,100)) / 100.0;
  // Update LED devices
  if (millis() - devMillisLast > devRefreshTime) {
    devHSV = kelvToHSV(kelv);
    devHSV.val = int(float(devHSV.val) * valScale);
    FastLED.clear();
    fan1LED[dot] = devHSV;
    FastLED.show();
    devMillisLast = millis();
  }
  // Update display
  if (millis() - lcdMillisLast > lcdRefreshTime) {
    sprintf(lcdText[0],"%4iK VAL x%d.%d%d",kelv,int(valScale),int(valScale*10)%10,int(valScale*100)%10);
    sprintf(lcdText[1],"h%3i s%3i v%3i",devHSV.h,devHSV.s,devHSV.v);
    for (i = 0; i < LCD_ROW_NUM; i++) {
      lcd.setCursor(0,i);
      lcd.print(lcdText[i]);
    }
    lcdMillisLast = millis();
  }
}

CHSV kelvToHSV(int kelv) {
  // Function to convert color temperature in Kelvin to an HSV pixel value.
  // Best results so far hold sat at max for a small percentage of the full Kelvin range.
  // This delays the color fading to white until the hue shifts towards yellow.
  CHSV HSV;
  float kelvRange = DEV_LED_KELV_MAX - DEV_LED_KELV_MIN;
  float satFullRange = 0.1; // Percentage of full Kelvin range
  int   satFullKelvLimit = DEV_LED_KELV_MIN + int(kelvRange * satFullRange); 
  int   satDecrKelvRange = kelvRange - satFullKelvLimit;
  int   hueMin = 7;   // Warmer end
  int   hueMax = 35;  // Cooler end
  
  HSV.hue = map(kelv, DEV_LED_KELV_MIN, DEV_LED_KELV_MAX, hueMin, hueMax);
  HSV.sat = map(max(kelv,satFullKelvLimit), satFullKelvLimit, DEV_LED_KELV_MAX, DEV_LED_SAT_MAX, 0);
  HSV.val = DEV_LED_VAL_MAX;
  return HSV;
}

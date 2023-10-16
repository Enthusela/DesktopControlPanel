
#include <FastLED.h>
#include <LiquidCrystal.h>

#define FILE_NAME "hue_control_hsv"
#define FILE_VERSION "Version 0.2"
// Default device #defines
#define ANALOG_RANGE 1023
#define DEV_LED_COLOR_ORDER GRB
#define DEV_LED_FPS 120
#define DEV_LED_HUE_MAX 255
#define DEV_LED_KELV_MIN 1500 // "kelv" used for temperature to avoid confusion with "temp" values
#define DEV_LED_KELV_MAX 6500 //Kelvin
#define DEV_LED_SAT_MAX 255
#define DEV_LED_TYPE WS2812B
#define DEV_LED_VAL_MAX 255
//Individual device #defines
// Device 1
#define DEV1_LED_COLOR_ORDER DEV_LED_COLOR_ORDER
#define DEV1_LED_NUM 12
#define DEV1_LED_TYPE DEV_LED_TYPE
#define DEV1_PIN_DATA 22
// Device 2
#define DEV2_LED_COLOR_ORDER DEV_LED_COLOR_ORDER
#define DEV2_LED_NUM 24
#define DEV2_LED_TYPE DEV_LED_TYPE
#define DEV2_PIN_DATA 23
//// Device 3
//#define DEV3_LED_COLOR_ORDER DEV_LED_COLOR_ORDER
//#define DEV3_LED_NUM 24
//#define DEV3_LED_TYPE DEV_LED_TYPE
//#define DEV3_PIN_DATA 24

// HSV value limits
#define HSV_MAX_VAL 255
// LCD display
#define LCD_COL_NUM 16
#define LCD_ROW_NUM 2
#define LCD_FPS 10

int   devMillisLast = 0;
int   devRefreshTime = 1000/DEV_LED_FPS;
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
CRGB dev1LEDs[DEV1_LED_NUM];
CRGB dev2LEDs[DEV2_LED_NUM];
CHSV devHSV;

void setup() {
  FastLED.clear();
  lcd.clear();
  delay(1000);
  FastLED.addLeds<DEV1_LED_TYPE,DEV1_PIN_DATA,DEV1_LED_COLOR_ORDER>(dev1LEDs,DEV1_LED_NUM);
  FastLED.addLeds<DEV2_LED_TYPE,DEV2_PIN_DATA,DEV2_LED_COLOR_ORDER>(dev2LEDs,DEV2_LED_NUM);
  lcd.begin(LCD_COL_NUM,LCD_ROW_NUM);
  lcd.print(FILE_NAME);
  lcd.setCursor(0,1);
  lcd.print(FILE_VERSION);
  delay(1000);
}

void loop() {
  // Set temperature and brightness control values
  kelv = map(analogRead(kelvPin),0,ANALOG_RANGE,DEV_LED_KELV_MIN,DEV_LED_KELV_MAX);
  valScale = float(map(analogRead(valScalePin),0,ANALOG_RANGE,0,100)) / 100.0;
  // Update LED devices
  if (millis() - devMillisLast > devRefreshTime) {
    devHSV = kelvToHSV(kelv);
    devHSV.val = int(float(devHSV.val) * valScale);
    FastLED.clear();
    FastLED.showColor(devHSV);
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

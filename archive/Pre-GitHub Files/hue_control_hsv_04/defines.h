#ifndef DEFINES_H
#define DEFINES_H
// Logic/math
# define CIRCLE_DEGREES 360
// Faceplate
#define FP_KNOB_NUM 4
#define FP_SWITCH_NUM 4
#define FP_KNOB_MODES_MAX FP_SWITCH_NUM
#define FP_SWITCH_KNOBS_MAX FP_KNOB_NUM
#define FP_RINGLIGHT_LEDS_MAX 8
// Arduino component parameters
#define ARDUNIO_POT_POSREF 210
#define ARDUINO_POT_POSRNG 240
// Default ARGBdevice parameters
#define ANALOG_RANGE 1023
#define ARGB_LED_COLOR_ORDER GRB
#define ARGB_LED_FPS 60
#define ARGB_LED_HUE_MAX 255
#define ARGB_LED_KELV_MIN 1500 // "kelv" used for temperature to avoid confusion at "temp" values
#define ARGB_LED_KELV_MAX 6500 //Kelvin
#define ARGB_LED_SAT_MAX 255
#define ARGB_LED_TYPE WS2812B
#define ARGB_LED_VAL_MAX 255
//Individual ARGB device parameters
// ARGB 1
#define ARGB1_LED_COLOR_ORDER ARGB_LED_COLOR_ORDER
#define ARGB1_LED_NUM 24
#define ARGB1_LED_TYPE ARGB_LED_TYPE
#define ARGB1_PIN_DATA 22
// ARGB 2
#define ARGB2_LED_COLOR_ORDER ARGB_LED_COLOR_ORDER
#define ARGB2_LED_NUM 24
#define ARGB2_LED_TYPE ARGB_LED_TYPE
#define ARGB2_PIN_DATA 23
// ARGB 3
#define ARGB3_LED_COLOR_ORDER ARGB_LED_COLOR_ORDER
#define ARGB3_LED_NUM 12
#define ARGB3_LED_TYPE ARGB_LED_TYPE
#define ARGB3_PIN_DATA 24
// LEDs
#define DOT_FPS 13
#define HSV_SCALE_MAX 255
// LCD display
#define LCD_COL_NUM 16
#define LCD_ROW_NUM 2
#define LCD_FPS 10

#endif
#ifndef DESKTOP_CONTROL_PANEL_H
#define DESKTOP_CONTROL_PANEL_H

#include <Arduino.h>
#include <FastLED.h>
#include <chipsets.h>

// ARGB defaults
#define ARGB_DEFAULT_CHIPSET WS2812B
#define ARGB_DEFAULT_COLOR_ORDER GRB
#define ARGB_LED_FPS 60
#define ARGB_LED_HUE_MIN 0
#define ARGB_LED_HUE_MAX 255
#define ARGB_LED_KELV_MIN 1500 // "kelv" used for temperature to avoid confusion at "temp" values
#define ARGB_LED_KELV_MAX 6500
#define ARGB_FAN_LED_NUM 24
#define ARGB_LED_SAT_MIN 0
#define ARGB_LED_SAT_MAX 255
#define ARGB_LED_VAL_MIN 0
#define ARGB_LED_VAL_MAX 170
#define ARGB_HUE_BLUE 120
#define ARGB_HUE_GREEN 240
#define ARGB_HUE_RED 0
#define ARGB_DEVICE_NUMBER 7
// ARGB 1 - Top Fans - Rear
#define ARGB_FAN_TOP_REAR_CHIPSET ARGB_DEFAULT_CHIPSET
#define ARGB_FAN_TOP_REAR_LED_NUM ARGB_FAN_LED_NUM
#define ARGB_FAN_TOP_REAR_ORDER ARGB_DEFAULT_COLOR_ORDER
#define ARGB_FAN_TOP_REAR_PIN 6
// ARGB 2 - Top Fans - Front
// flickers on hue = 0 - 84, 203 - 254
#define ARGB_FAN_TOP_FRONT_CHIPSET ARGB_DEFAULT_CHIPSET
#define ARGB_FAN_TOP_FRONT_LED_NUM ARGB_FAN_LED_NUM
#define ARGB_FAN_TOP_FRONT_ORDER ARGB_DEFAULT_COLOR_ORDER
#define ARGB_FAN_TOP_FRONT_PIN 7
// ARGB 3 - Front Fans - Top
#define ARGB_FAN_FRONT_TOP_CHIPSET ARGB_DEFAULT_CHIPSET
#define ARGB_FAN_FRONT_TOP_LED_NUM ARGB_FAN_LED_NUM
#define ARGB_FAN_FRONT_TOP_ORDER ARGB_DEFAULT_COLOR_ORDER
#define ARGB_FAN_FRONT_TOP_PIN 8
// ARGB 4 - Rear Fan
#define ARGB_FAN_REAR_CHIPSET ARGB_DEFAULT_CHIPSET
#define ARGB_FAN_REAR_LED_NUM ARGB_FAN_LED_NUM
#define ARGB_FAN_REAR_ORDER ARGB_DEFAULT_COLOR_ORDER
#define ARGB_FAN_REAR_PIN 9
// ARGB 5 - Front Fans - Middle
#define ARGB_FAN_FRONT_MID_CHIPSET ARGB_DEFAULT_CHIPSET
#define ARGB_FAN_FRONT_MID_LED_NUM ARGB_FAN_LED_NUM
#define ARGB_FAN_FRONT_MID_ORDER ARGB_DEFAULT_COLOR_ORDER
#define ARGB_FAN_FRONT_MID_PIN 10
// ARGB 6 - Front Fans - Bottom
#define ARGB_FAN_FRONT_LOW_CHIPSET ARGB_DEFAULT_CHIPSET
#define ARGB_FAN_FRONT_LOW_LED_NUM ARGB_FAN_LED_NUM
#define ARGB_FAN_FRONT_LOW_ORDER ARGB_DEFAULT_COLOR_ORDER
#define ARGB_FAN_FRONT_LOW_PIN 11
// ARGB 7 - GPU Support
#define ARGB_SUPPORT_CHIPSET ARGB_DEFAULT_CHIPSET
#define ARGB_SUPPORT_LED_NUM 26
#define ARGB_SUPPORT_ORDER ARGB_DEFAULT_COLOR_ORDER
#define ARGB_SUPPORT_PIN 12

#endif
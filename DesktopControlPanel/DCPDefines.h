#ifndef DCP_DEFINES_H
#define DCP_DEFINES_H

// System parameters
#define ANALOG_MAX 1023
#define UPDATE true

// Serial
#define SERIAL_WRITE_PERIOD 10

// ARGB
#define ARGB_LED_HUE_MIN 0
#define ARGB_LED_HUE_MAX 255
#define ARGB_LED_KELV_MIN 1500 // "kelv" used for temperature to avoid confusion at "temp" values
#define ARGB_LED_KELV_MAX 6500
#define ARGB_LED_SAT_MIN 0
#define ARGB_LED_SAT_MAX 255
#define ARGB_LED_VAL_MIN 0
#define ARGB_LED_VAL_MAX 170

#define ARGBW_G_PIN 10

// Potentiometers
#define POT1_PIN A1
#define POT2_PIN A2
#define POT3_PIN A3
#define POT4_PIN A0
#define POT_DIFF_MIN 2
#define POT_LINEAR false
#define POT_LOGARITHMIC true
#define POT_ANGLE_MIN 240
#define POT_ANGLE_MAX -60
#define POT_STO_LED_NUM 2

// Rocker Switches
#define SW_R_1_PIN 22
#define SW_R_2_PIN 23
#define SW_R_3_PIN 24
#define SW_R_4_PIN 25

// Toggle Switches
#define SW_T_1_PIN 26
#define SW_T_2_PIN 27
#define SW_T_3_PIN 28
// Toggle switches 4 and 5 are hard-wired

// Push Switches
#define SW_P_1_PIN 31
#define SW_P_2_PIN 32
#define SW_P_3_PIN 33

// Optocouplers
#define OPTO_1_ON_PIN   42
#define OPTO_1_OFF_PIN  44
#define OPTO_2_ON_PIN   46
#define OPTO_2_OFF_PIN  48
#define OPTO_3_ON_PIN   50
#define OPTO_3_OFF_PIN  52
#define OPTO_PULSE_LENGTH 1000

// Control mode
#define CTRL_MODE_VOLUME 1
#define CTRL_MODE_ARGB 0

// Audio Control
#define VOLUME_MAX ANALOG_MAX
#define VOLUME_MIN_DIFF 2
#define AUDIO_SOUNDCARD 0
#define AUDIO_HEADPHONES 1

// ControlPot parameters
#define STO_DISABLE_THRESHOLD 5
#define STO1_LED1_PIN 6 //6
#define STO1_LED2_PIN 4 //
#define STO2_LED1_PIN 5 //
#define STO2_LED2_PIN 8
#define STO3_LED1_PIN 7 //
#define STO3_LED2_PIN 3 // 2
#define STO_LEDS_MAX 2
#define STO_LED_MAX_BRIGHTNESS 255
#define STO_LED_MIN_BRIGHTNESS 10
#define CURRENT 0
#define PREVIOUS 1

// IR Transmitter
#define IR_RX_PIN 50
#define IR_TX_PIN 9
#define IR_AIRCON_ON 
#define IR_AIRCON_OFF 

// RF Remote
#define RF_A_ON_PIN 48
#define RF_A_OFF_PIN 50

#endif

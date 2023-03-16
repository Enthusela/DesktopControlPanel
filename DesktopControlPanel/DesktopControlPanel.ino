#include "DesktopControlPanel.h"
#define DECODE_NEC
#define DEBUG
#define RAW_BUFFER_LENGTH 132
// #include <IRremote.hpp>

// ARGB
CHSV argbHSV;

// Faceplate
bool pcAudioMode = false;
bool pcAudioModePrev = false;
bool ctrlMode;
bool ctrlModeChanged;
bool ctrlModePrev;
bool lightTemperatureMode = false;
Potentiometer pot1 = Potentiometer(POT1_PIN, ANALOG_MAX, POT_ANGLE_MIN, POT_ANGLE_MAX, POT_LINEAR);
Potentiometer pot2 = Potentiometer(POT2_PIN, ANALOG_MAX, POT_ANGLE_MIN, POT_ANGLE_MAX, POT_LINEAR);
Potentiometer pot3 = Potentiometer(POT3_PIN, ANALOG_MAX, POT_ANGLE_MIN, POT_ANGLE_MAX, POT_LINEAR);
Potentiometer pot4 = Potentiometer(POT4_PIN, ANALOG_MAX, POT_ANGLE_MIN, POT_ANGLE_MAX, POT_LOGARITHMIC);
SoftTakeoverLEDs pot1LEDs = SoftTakeoverLEDs(2, POT_ANGLE_MIN, POT_ANGLE_MAX, STO1_LED1_PIN, STO1_LED2_PIN);
SoftTakeoverLEDs pot2LEDs = SoftTakeoverLEDs(2, POT_ANGLE_MIN, POT_ANGLE_MAX, STO2_LED1_PIN, STO2_LED2_PIN);
SoftTakeoverLEDs pot3LEDs = SoftTakeoverLEDs(2, POT_ANGLE_MIN, POT_ANGLE_MAX, STO3_LED1_PIN, STO3_LED2_PIN);
ControlPot ctrlPotKelv = ControlPot(&pot1, ARGB_LED_KELV_MIN, ARGB_LED_KELV_MAX, &pot1LEDs);
ControlPot ctrlPotHue  = ControlPot(&pot1, ARGB_LED_HUE_MIN, ARGB_LED_HUE_MAX, &pot1LEDs);
ControlPot ctrlPotSat  = ControlPot(&pot2, ARGB_LED_SAT_MIN, ARGB_LED_SAT_MAX, &pot2LEDs);
ControlPot ctrlPotVal  = ControlPot(&pot3, ARGB_LED_VAL_MIN, ARGB_LED_VAL_MAX, &pot3LEDs);
ControlPot ctrlPotPCVolApp1 = ControlPot(&pot1, 0, ANALOG_MAX, &pot1LEDs);
ControlPot ctrlPotPCVolApp2 = ControlPot(&pot2, 0, ANALOG_MAX, &pot2LEDs);
ControlPot ctrlPotPCVolApp3 = ControlPot(&pot3, 0, ANALOG_MAX, &pot3LEDs);
ControlPot ctrlPotPCVolMain = ControlPot(&pot4, 0, ANALOG_MAX);
ToggleSwitch tglAudio =     ToggleSwitch(SW_T_1_PIN, OPTO_1_ON_PIN, OPTO_1_OFF_PIN);
// Opto pair 3 controls only one outlet only due to wiring issues: this is the behaviour I want for toggle 2
ToggleSwitch tglMonitorMain =  ToggleSwitch(SW_T_2_PIN, OPTO_3_ON_PIN, OPTO_3_OFF_PIN);
// Opto pair 2 controls only two outlets due to wiring issues: this is the behaviour I want for toggle 3
ToggleSwitch tglMonitorBoth =  ToggleSwitch(SW_T_3_PIN, OPTO_2_ON_PIN, OPTO_2_OFF_PIN);
uint16_t pcVolumeApp1;
uint16_t pcVolumeApp2;
uint16_t pcVolumeApp3;
uint16_t pcVolumeMain;

uint64_t stoLast;

//Serial
uint32_t serialWriteTimeLast = 0;

void setup() {
    setupSerial();
    setupPinModes();    
    setupInitialValues();
    stoLast = millis();
}

void loop() {
    readFaceplateInputs();
    pcVolumeMain = ctrlPotPCVolMain.getValue();
    setValuesForActiveCtrlMode();
    setARGBStripColorValues();
    sendCommandsToPCIfWriteTimeElapsed();
    // printDiagnostics();
}

void setupSerial() {
    Serial.begin(9600);
    // Serial.println(F("START " __FILE__ " at " __TIMESTAMP__));
    Serial.flush();
}

void setupPinModes() {
    pinMode(SW_R_1_PIN, INPUT_PULLUP);
    pinMode(SW_R_2_PIN, INPUT_PULLUP);
    pinMode(SW_R_3_PIN, INPUT_PULLUP);
    pinMode(SW_R_4_PIN, INPUT_PULLUP);
    pinMode(SW_P_1_PIN, INPUT_PULLUP);
    pinMode(SW_P_2_PIN, INPUT_PULLUP);
    pinMode(SW_P_3_PIN, INPUT_PULLUP);
    pinMode(ARGBW_G_PIN, OUTPUT);
}

void setupInitialValues() {
    pcAudioMode = digitalRead(SW_R_4_PIN);
    ctrlMode = digitalRead(SW_R_3_PIN);
    ctrlModePrev = false;
    ctrlModeChanged = false;
    lightTemperatureMode = digitalRead(SW_T_2_PIN);
    setARGBColorValues();
    pcVolumeApp1 = ctrlPotPCVolApp1.getValue();
    pcVolumeApp2 = ctrlPotPCVolApp2.getValue();
    pcVolumeApp3 = ctrlPotPCVolApp3.getValue();
    pcVolumeMain = ctrlPotPCVolMain.getValue();
}

void readFaceplateInputs() {
    pcAudioModePrev = pcAudioMode;
    pcAudioMode = digitalRead(SW_R_4_PIN);
    ctrlModePrev = ctrlMode;
    ctrlMode = digitalRead(SW_R_3_PIN);
    ctrlModeChanged = (ctrlMode != ctrlModePrev);
    lightTemperatureMode = digitalRead(SW_R_2_PIN);
    tglAudio.set();
    tglMonitorMain.set();
    tglMonitorBoth.set();
}

void setValuesForActiveCtrlMode() {
    if (ctrlMode == CTRL_MODE_ARGB) {
        setARGBColorValues();
    }
    else if (ctrlMode == CTRL_MODE_VOLUME) {
        setPCAppVolumeValues();
    }
}

void setARGBColorValues() {
    if (lightTemperatureMode) {
        argbHSV = colorTemperatureToHSV(ctrlPotKelv.getValue(ctrlModeChanged));
    }
    else {
        argbHSV = colorHueToHSV(ctrlPotHue.getValue(ctrlModeChanged));
    }
    ctrlPotSat.getValue();
    ctrlPotVal.getValue();
    argbHSV.sat *= ctrlPotSat.getValuePercent();
    argbHSV.val *= ctrlPotVal.getValuePercent();
}

void setARGBStripColorValues() {
    int val = int(float(analogRead(POT1_PIN)) / 4.0);
    analogWrite(ARGBW_G_PIN, val);
}

void setPCAppVolumeValues() {
    pcVolumeApp1 = ctrlPotPCVolApp1.getValue(ctrlModeChanged);
    pcVolumeApp2 = ctrlPotPCVolApp2.getValue(ctrlModeChanged);
    pcVolumeApp3 = ctrlPotPCVolApp3.getValue(ctrlModeChanged);
}

void sendCommandsToPCIfWriteTimeElapsed() {
    if (commandWriteTimeHasElapsed()) {
        String commands;
        commands = getCommands();
        Serial.println(commands);
        Serial.flush();
    }
}

bool commandWriteTimeHasElapsed() {
    static uint32_t lastWriteTime;
    const uint8_t updatesPerSecond = 100;
    const uint16_t writePeriod = 1000/updatesPerSecond;
    if (periodHasElapsedSinceLastTime(writePeriod, lastWriteTime)) {
        lastWriteTime = millis();
        return true;
    }
    return false;
}

bool periodHasElapsedSinceLastTime(uint32_t period, uint32_t lastTime) {
    return ((millis() - lastTime) > period);
}

int16_t scaleValueByControlPot(int value, ControlPot *ctrlPot) {
    return value *= float(ctrlPot->getValue()) / float(ctrlPot->getValueMax());
}

String getCommands() {
    String commands = "_";
    commands += ",A " + String(pcAudioMode);
    commands += ",V1 " + String(pcVolumeApp1);
    commands += ",V2 " + String(pcVolumeApp2);
    commands += ",V3 " + String(pcVolumeApp3);
    commands += ",VM " + String(pcVolumeMain);
    commands += ",H " + String(argbHSV.hue);
    commands += ",S " + String(argbHSV.sat);
    commands += ",V " + String(argbHSV.val);
    return commands;
}

void printDiagnostics() {
    char text[150];
    int rsw1 = digitalRead(SW_R_1_PIN);
    int rsw2 = digitalRead(SW_R_2_PIN);
    int rsw3 = digitalRead(SW_R_3_PIN);
    int rsw4 = digitalRead(SW_R_4_PIN);
    int tsw1 = !digitalRead(SW_T_1_PIN);
    int tsw2 = !digitalRead(SW_T_2_PIN);
    int tsw3 = !digitalRead(SW_T_3_PIN);
    int psw1 = digitalRead(SW_P_1_PIN);
    int psw2 = digitalRead(SW_P_2_PIN);
    int psw3 = digitalRead(SW_P_3_PIN);
    int pot1 = analogRead(POT1_PIN);
    int pot2 = analogRead(POT2_PIN);
    int pot3 = analogRead(POT3_PIN);
    int pot4 = analogRead(POT4_PIN);
    sprintf(text, "rsw1 %04i rsw2 %04i rsw3 %04i rsw4 %04i || tsw1 %04i tsw2 %04i tsw3  %04i || psw1 %04i psw2 %04i psw3 %04i || pot1 %04i pot2 %04i pot3 %04i pot4 %04i",
    rsw1, rsw2, rsw3, rsw4, tsw1, tsw2, tsw3, psw1, psw2, psw3, pot1, pot2, pot3, pot4);
    Serial.println(text);
    // char hsv_values[12];
    // uint16_t temp = ctrlPotKelv.getValue();
    // sprintf(hsv_values, "%04i %03i %03i %03i", temp, argbHSV.h, argbHSV.s, argbHSV.v);
    // Serial.println(hsv_values);
}

void showSTOLeds() {
    const int ledNum = 6;
    int pins[ledNum] = {STO1_LED1_PIN, STO1_LED2_PIN, STO2_LED1_PIN, STO2_LED2_PIN, STO3_LED1_PIN, STO3_LED2_PIN};
    static int pin = 0;
    for (int i = 0; i < ledNum; i++) {
        analogWrite(pins[i], (i = pin) ? 255 : 0);
    }
    pin = (pin < ledNum) ? pin + 1 : 0;
}

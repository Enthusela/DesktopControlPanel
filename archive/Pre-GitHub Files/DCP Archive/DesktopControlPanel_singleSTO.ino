#include "DesktopControlPanel.h"
#include "DCPGlobals.h"
void setup() {
    // LED setup
    FastLED.addLeds<ARGB1_CHIPSET,ARGB1_PIN,ARGB1_ORDER>(argb1Pixels,ARGB1_LED_NUM);
    FastLED.addLeds<ARGB2_CHIPSET,ARGB2_PIN,ARGB2_ORDER>(argb2Pixels,ARGB2_LED_NUM);
    FastLED.addLeds<ARGB3_CHIPSET,ARGB3_PIN,ARGB3_ORDER>(argb3Pixels,ARGB3_LED_NUM);
    FastLED.addLeds<ARGB4_CHIPSET,ARGB4_PIN,ARGB4_ORDER>(argb4Pixels,ARGB4_LED_NUM);
    FastLED.addLeds<ARGB5_CHIPSET,ARGB5_PIN,ARGB5_ORDER>(argb5Pixels,ARGB5_LED_NUM);
    FastLED.addLeds<ARGB6_CHIPSET,ARGB6_PIN,ARGB6_ORDER>(argb6Pixels,ARGB6_LED_NUM);
    FastLED.addLeds<ARGB7_CHIPSET,ARGB7_PIN,ARGB7_ORDER>(argb7Pixels,ARGB7_LED_NUM);
    FastLED.addLeds<ARGB8_CHIPSET,ARGB8_PIN,ARGB8_ORDER>(argb8Pixels,ARGB8_LED_NUM);
    delay(1000);
    FastLED.clear(true);
    delay(1000);
    // Faceplate setup
    pinMode(POT1_PIN, INPUT);
    pinMode(POT2_PIN, INPUT);
    pinMode(POT3_PIN, INPUT);
    pinMode(POT4_PIN, INPUT);
    pinMode(RGBW_RED_PIN, OUTPUT);
    pinMode(RGBW_GREEN_PIN, OUTPUT);
    pinMode(RGBW_BLUE_PIN, OUTPUT);
    pinMode(STO_POT1_LED1_PIN, OUTPUT);
    pinMode(STO_POT1_LED2_PIN, OUTPUT);
    pinMode(STO_POT2_LED1_PIN, OUTPUT);
    pinMode(STO_POT2_LED2_PIN, OUTPUT);
    pinMode(STO_POT3_LED1_PIN, OUTPUT);
    pinMode(STO_POT3_LED2_PIN, OUTPUT);
    pinMode(SWITCH_AUDIO_DEVICE_PIN, INPUT_PULLUP);
    pinMode(SWITCH_CTRL_MODE_PIN, INPUT_PULLUP);
    // Initial values
    // TODO: integrate into object initialise functions
    argbHSV.hue = mapInt10bTo8b(analogRead(POT1_PIN));
    argbHSV.sat = mapInt10bTo8b(analogRead(POT2_PIN));
    argbHSV.val = reduceValBySat(mapInt10bTo8b(analogRead(POT3_PIN)), argbHSV.sat);
    volume1 = analogRead(POT1_PIN);
    volume2 = analogRead(POT2_PIN);
    volume3 = analogRead(POT3_PIN);
    volume4 = logToLinear(analogRead(POT4_PIN), float(ANALOG_RANGE));
    // Prevent [variable] != [variable]Prev conditions triggering on first scan
    argbHSVPrev.hue = argbHSV.hue;
    argbHSVPrev.sat = argbHSV.sat;
    argbHSVPrev.val = argbHSV.val;
    volume1Prev = volume1;
    volume2Prev = volume2;
    volume3Prev = volume3;
    volume4Prev = volume4;
    swAudioModeSelPrev = digitalRead(SWITCH_AUDIO_DEVICE_PIN);
    swCtrlModeSelPrev = digitalRead(SWITCH_CTRL_MODE_PIN);
    // Soft takeover setup
    hueSTOLEDs[0][PIN] = STO_POT1_LED1_PIN;
    hueSTOLEDs[1][PIN] = STO_POT1_LED2_PIN;
    for (int i = 0; i < argbSTOLEDNum; i++) {
        hueSTOLEDs[i][ANGLE] = argbSTOLEDAngleMin + argbSTOLEDSpacing * i;
        hueSTOLEDs[i][BRIGHTNESS] = 0;
    }
    // Serial setup
    Serial.begin(9600);
    Serial.println("Started.");
    Serial.flush();
}

void loop() {
    // Update faceplate IO states
    bool swAudioModeSel = digitalRead(SWITCH_AUDIO_DEVICE_PIN);
    bool swCtrlModeSel = digitalRead(SWITCH_CTRL_MODE_PIN);
    uint16_t pot1 = analogRead(POT1_PIN);
    uint16_t pot2 = analogRead(POT2_PIN);
    uint16_t pot3 = analogRead(POT3_PIN);
    uint16_t pot4 = logToLinear(analogRead(POT4_PIN), float(ANALOG_RANGE));
    // Check soft-takeover modes
    bool swCtrlModeChanged = (swCtrlModeSel != swCtrlModeSelPrev);
    if (swCtrlModeSel == CTRL_MODE_VOLUME) {
        // No soft-takeover: volumes will change instantly when mode enabled
        // Set volumes; filter pot noise by ignoring small changes
        volume1 = potChanged(pot1, pot1Prev) ? pot1 : volume1Prev;
        volume2 = potChanged(pot2, pot2Prev) ? pot2 : volume2Prev;
        volume3 = potChanged(pot3, pot3Prev) ? pot3 : volume3Prev;
        volume4 = potChanged(pot4, pot4Prev) ? pot4 : volume4Prev;
        // Set previous values
        volume1Prev = volume1;
        volume2Prev = volume2;
        volume3Prev = volume3;
        volume4Prev = volume4;
    }
    else if (swCtrlModeSel == CTRL_MODE_ARGB) {        
        // Set hue
        uint8_t hueNew = mapInt10bTo8b(pot1);
        bool hueChanged = (hueNew != argbHSV.hue);
        argbHueSTOActive = ((swCtrlModeChanged && hueChanged) || argbHueSTOActive); // STO mode latches on
        if (!argbHueSTOActive) {
            argbHSV.hue = hueNew;
            argbHueSTOAngleInitial = STO_START_ANGLE_NULL; // arbitrary invalid value used to trigger update to argbHueSTOStartAngle when entering STO mode
            // Test pattern to identify STO-disabled mode
            for (int i = 0; i < argbSTOLEDNum; i++) {
                analogWrite(hueSTOLEDs[i][PIN], 0);
            }
        }
        else {
            int argbSTOValCurr = hueNew;
            int argbSTOValTgt = argbHSV.hue;
            if (abs(argbSTOValCurr - argbSTOValTgt) <= STO_THLD) {
                argbHueSTOActive = false;
            }
            else { // soft takeover lighting on
                // Get current angular position
                int argbSTOValMin = ARGB_LED_HUE_MIN;
                int argbSTOValMax = ARGB_LED_HUE_MAX;
                int argbSTOAngleCurr = map(argbSTOValCurr, argbSTOValMin, argbSTOValMax, potAngleMin, potAngleMax);
                // If STO mode just started, capture initial anglular position
                if (argbHueSTOAngleInitial == STO_START_ANGLE_NULL) {
                    argbHueSTOAngleInitial = argbSTOAngleCurr;
                }
                // Calculate target angular position
                int argbSTOAngleTgt = map(argbSTOValTgt, argbSTOValMin, argbSTOValMax, potAngleMin, potAngleMax);
                // Find closest LED to target angular positon
                uint8_t closestLED = 0;
                uint16_t shortestAngularDistance = STO_ANGLE_DIST_NULL; // arbitrary starting value larger than largest valid value
                for (int i = 0; i < argbSTOLEDNum; i++) {
                    uint16_t ledAngularDistance = abs(argbSTOAngleTgt - hueSTOLEDs[i][ANGLE]);
                    if (ledAngularDistance < shortestAngularDistance) {
                        shortestAngularDistance = ledAngularDistance;
                        closestLED = i;
                    }
                }
                // Calculate current distance to target as % of initial distance to target
                float argbSTORltvDistToTgt = float(abs(argbSTOAngleTgt - argbSTOAngleCurr)) / float(abs(argbSTOAngleTgt - argbHueSTOAngleInitial));
                argbSTORltvDistToTgt = min(max(argbSTORltvDistToTgt, 0.0), 1.0);
                // Calculate and write LED brightness values
                int argbSTOBrightnessFromDist = float(STO_LED_MAX_BRIGHTNESS) * (1.0 - argbSTORltvDistToTgt);
                int argbSTOBrightness = max(STO_LED_MIN_BRIGHTNESS, argbSTOBrightnessFromDist);
                for (int i = 0; i < argbSTOLEDNum; i++) {
                    int brightness = (i == closestLED) ? argbSTOBrightness : 0;
                    analogWrite(hueSTOLEDs[i][PIN], brightness);
                }
            }
        }
        // Set sat
        uint8_t satNew = potChanged(pot2, pot2Prev) ? mapInt10bTo8b(pot2) : argbHSV.sat;
        argbHSV.sat = satNew;
        // Set val
        uint8_t valNew = potChanged(pot3, pot3Prev) ? mapInt10bTo8b(pot3) : argbHSV.val;
        argbHSV.val = reduceValBySat(valNew, argbHSV.sat);
        // Set previous values
        argbHSVPrev.hue = argbHSV.hue;
        argbHSVPrev.sat = argbHSV.sat;
        argbHSVPrev.val = argbHSV.val;
        // argbHSVPrev = argbHSV;
    }
    // Update PC RGB LEDs
    if ((millis() - argbDisplayTimeLast) > argbDisplayPeriod) {
        if(false) { // swKelvColorMode) { // Temperature-based color control
            colorKelv = map(pot1, 0, ANALOG_RANGE, ARGB_LED_KELV_MIN, ARGB_LED_KELV_MAX);
            argbHSV = kelvinToHSV(colorKelv);
        }
        FastLED.showColor(argbHSV, 255);
        argbDisplayTimeLast = millis();
    }
    // Update RGBW strip LEDs 
    // TODO: Determine color curves to allow control of the RGBW strip
    analogWrite(RGBW_RED_PIN, 255);
    analogWrite(RGBW_GREEN_PIN, 1);
    analogWrite(RGBW_BLUE_PIN, 0);
    // Update Python serial commands
    if ((millis() - serialWriteTimeLast) > SERIAL_WRITE_PERIOD) {
        // Generate command string and print to serial output
        serialStrCmd = "START";
        strAudioDev = swAudioModeSel ? ",AUDIO HEADPHONES" : ",AUDIO SOUNDCARD";
        strPot1 = ",POT1 " + String(volume1);
        strPot2 = ",POT2 " + String(volume2);
        strPot3 = ",POT3 " + String(volume3);
        strPot4 = ",POT4 " + String(volume4);
        serialStrCmd += strAudioDev + strPot1 + strPot2 + strPot3 + strPot4;
        // Serial.println(serialStrCmd);
        // Serial.flush();
        serialWriteTimeLast = millis();
    }
    // Update faceplate IO previous-states
    swAudioModeSelPrev = swAudioModeSel;
    swCtrlModeSelPrev = swCtrlModeSel;
    pot1Prev = pot1;
    pot2Prev = pot2;
    pot3Prev = pot3;
    pot4Prev = pot4;
}
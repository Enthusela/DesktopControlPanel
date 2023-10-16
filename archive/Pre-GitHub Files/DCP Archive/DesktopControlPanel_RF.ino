#include "DesktopControlPanel.h"
// ARGB
CRGB argb1Pixels[ARGB1_LED_NUM];
CRGB argb2Pixels[ARGB2_LED_NUM];
CRGB argb3Pixels[ARGB3_LED_NUM];
CRGB argb4Pixels[ARGB4_LED_NUM];
CRGB argb5Pixels[ARGB5_LED_NUM];
CRGB argb6Pixels[ARGB6_LED_NUM];
CRGB argb7Pixels[ARGB7_LED_NUM];
CRGB argb8Pixels[ARGB8_LED_NUM];
uint8_t argbCtrlNum = ARGB_DEVICE_NUMBER;
uint16_t argbDisplayPeriod = 1000/ARGB_LED_FPS;
uint32_t argbDisplayTimeLast = 0;
CHSV argbHSV;
CHSV argbHSVLast;
CRGB argbRGB;
uint16_t colorKelv = 0;
uint16_t colorKelvLast = 0;
uint8_t rgbwKelv = 127;
// Faceplate
ControlPot colorKelvCtrl(POT2_PIN);
ControlPot colorValCtrl(POT1_PIN);
uint16_t pot1 = 0;
uint16_t pot2 = 0;
uint16_t pot3 = 0;
uint16_t pot4 = 0;
uint16_t pot1Prev = pot1;
uint16_t pot2Prev = pot2;
uint16_t pot3Prev = pot3;
uint16_t pot4Prev = pot4;
uint16_t volumeDebounceTime = 300;
uint32_t volumeUpdateTimeLast = 0;
bool swKelvColorMode = false;
bool swAudioDevSel = false;
bool swAudioDevSelPrev = false;
bool swVolumeModeSel = false;
bool swVolumeModeSelPrev = false;
// Serial
String serialStrCmd;
String strAudioDev;
String strVolumeMode;
String strPot1;
String strPot2;
String strPot3;
String strPot4;
uint32_t serialWriteTimeLast = 0;
// ControlEncoder deviceSelect(ENC_DEV_SEL_CLK_PIN,ENC_DEV_SEL_DT_PIN,ENC_DEV_SEL_SW_PIN);

// RF
#define PACKET_NONE		0
#define PACKET_OK		1
#define PACKET_INVALID	2
#define PAYLOAD_SIZE NRF905_MAX_PAYLOAD

nRF905 transceiver;

static volatile uint8_t packetStatus;

void nRF905_int_dr() {
    transceiver.interrupt_dr();
}

void nRF905_onRxComplete(nRF905* device) {
    packetStatus = PACKET_OK;
}

void nRF905_onRxInvalid(nRF905* device) {
    packetStatus = PACKET_INVALID;
}

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
    pinMode(SWITCH_AUDIO_DEVICE_PIN, INPUT_PULLUP);
    pinMode(SWITCH_LIGHTING_MODE_PIN, INPUT_PULLUP);
    pinMode(RGBW_RED_PIN, OUTPUT);
    pinMode(RGBW_GREEN_PIN, OUTPUT);
    pinMode(RGBW_BLUE_PIN, OUTPUT);
    // Prevent [variable] != [variable]Prev conditions triggering on first scan
    swAudioDevSel = 0; //digitalRead(SWITCH_AUDIO_DEVICE_PIN);
    swAudioDevSelPrev = swAudioDevSel;
    swVolumeModeSel = digitalRead(SWITCH_AUDIO_DEVICE_PIN);
    swVolumeModeSelPrev = swVolumeModeSel;
    // RF setup
    SPI.begin();
    transceiver.begin(
        SPI,
        16000000,
        RF_CSN_PIN,
        RF_CE_PIN,
        RF_TRX_PIN,
        RF_PWR_PIN,
        NRF905_PIN_UNUSED, // CD pin unused
        RF_DR_PIN,
        NRF905_PIN_UNUSED, // AM unused
        nRF905_int_dr, // Interrupt function for DR interrupt pin
        NULL    // AM pin not used: no interrupt function
    );
    transceiver.events(
        nRF905_onRxComplete,
        nRF905_onRxInvalid,
        NULL,   // Recieve-only
        NULL    // Recieve-only
    );
    transceiver.setLowRxPower(true);
    // Serial setup
    Serial.begin(9600);
    Serial.println(F("Started."));
}

void loop() {
    // Update faceplate IO states
    colorKelvCtrl.updateValue();
    swKelvColorMode = 0; // digitalRead(SWITCH_LIGHTING_MODE_PIN);
    swVolumeModeSel = digitalRead(SWITCH_LIGHTING_MODE_PIN);
    swAudioDevSel = digitalRead(SWITCH_AUDIO_DEVICE_PIN);
    pot1 = analogRead(POT1_PIN);
    pot2 = analogRead(POT2_PIN);
    pot3 = analogRead(POT3_PIN);
    pot4 = round(map(sqrt(analogRead(POT4_PIN)), 0.0, 31.9, 0.0, 1023.0));
    // Update PC RGB LEDs
    if ((millis() - argbDisplayTimeLast) > argbDisplayPeriod) {
        if(false) { // swKelvColorMode) { // Temperature-based color control
            colorKelv = map(colorKelvCtrl.getValue(), 0, ANALOG_RANGE, ARGB_LED_KELV_MIN, ARGB_LED_KELV_MAX);
            argbHSV = kelvinToHSV(colorKelv);    
        }
        else { // Manual HSV-based color control
            argbHSV.hue = map(analogRead(POT1_PIN),0,1023,0,255);
            argbHSV.sat = map(analogRead(POT2_PIN),0,1023,0,255);
            argbHSV.val = reduceValBySat(argbHSV.sat);
        }
        FastLED.showColor(argbHSV, 255);
        argbDisplayTimeLast = millis();
    }
    // Update RGBW strip LEDs 
    // TODO: Determine color curves to allow control of the RGBW strip
    analogWrite(RGBW_RED_PIN, 255);
    analogWrite(RGBW_GREEN_PIN, 1); // + rgbwKelv / 255); // * colorVal);
    analogWrite(RGBW_BLUE_PIN, 0); // + rgbwKelv / 255); // * colorVal);
    // Generate commands for Python script to serial
    if ((millis() - serialWriteTimeLast) > SERIAL_WRITE_PERIOD) {
        // Avoid sending frequent volume updates by ignoring small changes (the pots are noisy)
        String strPot1Val = abs(pot1 - pot1Prev) > POT_MIN_DIFF ? String(pot1) : String(pot1Prev);
        String strPot2Val = abs(pot2 - pot2Prev) > POT_MIN_DIFF ? String(pot2) : String(pot2Prev);
        String strPot3Val = abs(pot3 - pot3Prev) > POT_MIN_DIFF ? String(pot3) : String(pot3Prev);
        String strPot4Val = abs(pot4 - pot4Prev) > POT_MIN_DIFF ? String(pot4) : String(pot4Prev);
        // Generate command string and print to serial output
        serialStrCmd = "START";
        strAudioDev = swAudioDevSel ? ",AUDIO HEADPHONES" : ",AUDIO SOUNDCARD";
        strPot1 = ",CTRL1 " + strPot1Val;
        strPot2 = ",CTRL2 " + strPot2Val;
        strPot3 = ",CTRL3 " + strPot3Val;
        strPot4 = ",CTRL4 " + strPot4Val;
        serialStrCmd += strAudioDev + strPot1 + strPot2 + strPot3 + strPot4;
        Serial.println(serialStrCmd);
        Serial.flush();
        serialWriteTimeLast = millis();
    }
    // Update faceplate IO previous-states
    swAudioDevSelPrev = swAudioDevSel;
    pot1Prev = pot1;
    pot2Prev = pot2;
    pot3Prev = pot3;
    pot4Prev = pot4;
}
// #include "C:\Users\natha\OneDrive\Documents\Arduino\hardware\esp8266com\esp8266\libraries\ESP8266WiFi\src\ESP8266Wifi.h"

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define LED_BUILTIN_HIGH LOW
#define LED_BUILTIN_LOW HIGH
#define RED 0
#define GREEN 1
#define BLUE 2
#define WHITE 3
#define RGBW_MIN_VALUE 0
#define RGBW_MAX_VALUE 255
#define RED_PIN D1
#define GREEN_PIN D2
#define BLUE_PIN D6
#define WHITE_PIN D5

uint8_t rgbw[4] = {255, 255, 255, 255};

const char* ssid = "CoffeeHouse";
const char* pwd = "N4thansD0main";
WiFiClient client;

IPAddress server(192, 168, 50, 202);
const int port = 5000;
String rawJSON = "";

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("Booting...");
    setupPinModes();
    connectToWiFi();
    connectToServer();
    // Serial.println("Listening for messages from the server...");
}

void loop() {
    if (!client.available()) {
        // client.println("ESP8266 is listening.");
    }
    else {
        rawJSON = client.readStringUntil('\n');
        client.flush();
        parseRGBWJSON();
        serialPrintRGBW();
    }
    setRGBW();
}


void setupPinModes() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(WHITE_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LED_BUILTIN_HIGH);
}

void connectToWiFi() {
    Serial.print("Connecting to Wifi...");
    WiFi.begin(ssid, pwd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }    
    Serial.println();
    Serial.print("Connected to ");
    Serial.println(WiFi.localIP());
}

void connectToServer() {
    Serial.print("Connecting to Raspberry Pi at ");
    Serial.print(server);
    Serial.print(":");
    Serial.print(port);
    Serial.print("...");
    uint64_t start = millis();
    while (!client.connected() && millis() - start < 10000) {
        if (!client.connect(server, port)) {
            Serial.print(".");
        } else {
            Serial.println("Connected.");
            client.println("Hello from ESP8266");
            return;
        }
        delay(500);
    }
    Serial.println();
    Serial.println("Failed to connect to server.");
}

void parseRGBWJSON() {
    DynamicJsonDocument rgbwDoc(1024);
    DeserializationError error = deserializeJson(rgbwDoc, rawJSON);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.println("Continuing to listen for messages from the server...");
        client.flush();
    }
    rgbw[RED]   = error ? 0 : min(max(int(rgbwDoc["r"]), RGBW_MIN_VALUE), RGBW_MAX_VALUE);
    rgbw[GREEN] = error ? 0 : min(max(int(rgbwDoc["g"]), RGBW_MIN_VALUE), RGBW_MAX_VALUE);
    rgbw[BLUE]  = error ? 0 : min(max(int(rgbwDoc["b"]), RGBW_MIN_VALUE), RGBW_MAX_VALUE);
    rgbw[WHITE] = error ? 0 : min(max(int(rgbwDoc["w"]), RGBW_MIN_VALUE), RGBW_MAX_VALUE);
}

void setRGBW() {
    analogWrite(RED_PIN, rgbw[RED]);
    analogWrite(GREEN_PIN, rgbw[GREEN]);
    analogWrite(BLUE_PIN, rgbw[BLUE]);
    analogWrite(WHITE_PIN, rgbw[WHITE]);
}

void serialPrintRGBW() {
    Serial.print("rgbw: ");
    Serial.print(rgbw[RED]);
    Serial.print(", ");
    Serial.print(rgbw[GREEN]);
    Serial.print(", ");
    Serial.print(rgbw[BLUE]);
    Serial.print(", ");
    Serial.println(rgbw[WHITE]);
}
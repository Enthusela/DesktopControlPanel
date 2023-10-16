#include <Arduino.h>
#define SENSOR_PIN A0

int32_t amps = 0;
uint16_t ampsValue = 0;
uint16_t ampsZeroAdj = 0;
uint32_t millisLast = 0;
uint8_t serialDelay = 250;
double Vcc = 0;
char strBffr[32];

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  for (uint8_t i = 1; i <= 10; i++) {
    uint16_t error = 512 - analogRead(SENSOR_PIN);
    ampsZeroAdj = ((ampsZeroAdj * (i-1)) + error) / i;
  }
}

void loop() {
  ampsValue = analogRead(SENSOR_PIN) + ampsZeroAdj;
  Vcc = readVcc()/1000.0;
  sprintf(strBffr, "readVcc = %d", Vcc);//"ampsZeroAdj: %d amps: %d",ampsZeroAdj,ampsValue);
  if (millis() - millisLast >= serialDelay) {
    Serial.println(strBffr);
    millisLast = millis();
  }
}

#include <Arduino.h>
#line 1 "C:\\Users\\natha\\OneDrive\\Documents\\Coding\\GitHub\\RF_Test\\RF_Test.ino"
/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch rxSwitch = RCSwitch();
RCSwitch txSwitch = RCSwitch();

#line 12 "C:\\Users\\natha\\OneDrive\\Documents\\Coding\\GitHub\\RF_Test\\RF_Test.ino"
void setup();
#line 19 "C:\\Users\\natha\\OneDrive\\Documents\\Coding\\GitHub\\RF_Test\\RF_Test.ino"
void loop();
#line 12 "C:\\Users\\natha\\OneDrive\\Documents\\Coding\\GitHub\\RF_Test\\RF_Test.ino"
void setup() {
  Serial.begin(9600);
  rxSwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  txSwitch.enableTransmit(50);
  Serial.println("setup");
}

void loop() {

  if (rxSwitch.available()) {
    
    Serial.print("Received ");
    Serial.print( rxSwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( rxSwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( rxSwitch.getReceivedProtocol() );

    rxSwitch.resetAvailable();
  }
  else {
    rxSwitch.send(5393, 24);
    delay(1000);
  }
}


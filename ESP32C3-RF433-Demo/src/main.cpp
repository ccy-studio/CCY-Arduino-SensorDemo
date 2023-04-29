#include "Arduino.h"


#include "RCSwitch.h"

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(GPIO_NUM_2);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );

    mySwitch.resetAvailable();
  }
}
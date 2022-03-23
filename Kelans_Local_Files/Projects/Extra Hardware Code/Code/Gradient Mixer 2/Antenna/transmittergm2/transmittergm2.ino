#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  // PIN THAT GIVES POWER TO THE ANTENNA
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  // PIN THAT SENDS THE BINARY ARRAY TO THE Transmitter, WHICH IS Arduino Pin #10  
  mySwitch.enableTransmit(10);

  // Optional set pulse length. dont touch
  mySwitch.setPulseLength(718);
  
  // Optional set protocol (default is 1, will work for most outlets) dont touch
  mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
}

void loop() {
  // Binary code - button 3
  mySwitch.send("10100110000010000101111000000000");
  delay(1000);  
  mySwitch.send("10101110000010000101111000000000");
  delay(1000);
}

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);

  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  mySwitch.setPulseLength(714);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
}

void loop() {
  // Binary code - button 3
  mySwitch.send("10001110000010000101111000000000");
  delay(1000);  
  mySwitch.send("10000001000010000101111000000000");
  delay(1000);
}

void setup() {
  // put your setup code here, to run onc
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(10, HIGH);
  delay(5000);
  digitalWrite(10, LOW);
  delay(5000);
}

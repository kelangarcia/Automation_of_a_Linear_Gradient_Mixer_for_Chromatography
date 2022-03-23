int valve = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(valve, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(valve,HIGH);
  delay(2000);
  digitalWrite(valve,LOW);
  delay(2000);
}

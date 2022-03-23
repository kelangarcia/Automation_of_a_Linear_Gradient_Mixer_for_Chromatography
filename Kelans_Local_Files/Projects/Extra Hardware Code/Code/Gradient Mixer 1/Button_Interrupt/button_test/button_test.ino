#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

// Pins for Stop Buttons                                 
const int StopLED = 19;
const int StopButton = 21;
const int PlayButton = 14;

const int VALVE = 8;

// variables will change:
volatile int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);

  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH); //POWER ANTENNA
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  mySwitch.setPulseLength(718);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(2);

  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(2);

  //pinMode(12, OUTPUT);
  //
  digitalWrite(12, LOW); 

  pinMode(VALVE, OUTPUT);
  
  
  pinMode(StopLED, OUTPUT);
  
  pinMode(StopButton,INPUT);
  pinMode(PlayButton, INPUT);
  
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(StopButton), PauseProgram,  RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(VALVE, LOW);
  mySwitch.send("10101110000010000101111000000000");
  digitalWrite(StopLED, LOW);
  delay(2000);
  
  digitalWrite(VALVE, HIGH);
  mySwitch.send("10100110000010000101111000000000");
  digitalWrite(StopLED, HIGH);
  delay(2000);
}

void PauseProgram(){
  //if (currentPage == '5'){
    int decider = 1;

//    pump = 0;
    mySwitch.send("10101110000010000101111000000000");
    digitalWrite(StopLED, HIGH);
    
    //Wait until Play Button is pressed
    while(decider == 1){
      buttonState = digitalRead(PlayButton);
      if (buttonState == HIGH){
        decider = 0;
        //Turns Pump ON
        //  pump = 1;
        mySwitch.send("10100110000010000101111000000000");
      digitalWrite(StopLED, LOW);
  
      }
    }
  //}
}

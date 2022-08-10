/*
 * With this code buttons, LED and transmitter can be tested. 
 */

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

// Pins for Stop Buttons                                 
const int StopLED = 18;
const int ButtonsVcc =19;
const int StopButton = 20;
const int PlayButton = 21;

const int VALVE = 8;
int pump = 0;

//transmitter codes
const char* antennaOn = "10100110000010000101111000000000";
const char* antennaOff = "10101110000010000101111000000000";
unsigned long lastInterrupt;

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
   pinMode(ButtonsVcc, OUTPUT);
  digitalWrite(ButtonsVcc, HIGH);

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
  digitalWrite(StopLED, LOW);
  delay(5);
  digitalWrite(VALVE, HIGH);
  digitalWrite(StopLED, HIGH);
  delay(5);
}

void PauseProgram(){
  if(millis() - lastInterrupt > 10) // we set a 10ms no-interrupts window
    {    

      int decider = 1;
  
      pump = 0;
      //You can delete the codeline below if you dont want to turn off the pump when the program is paused
      mySwitch.send(antennaOff);//Change Binary Code to your own binary code for turning the pump off
      digitalWrite(StopLED, HIGH);
      
      //Wait until Play Button is pressed
      while(decider == 1){
        buttonState = digitalRead(PlayButton);
        if (buttonState > 0.1){
          decider = 0;
        }
      }
      
      //Turns Pump ON
      pump = 1;
      //You can delete the codeline below if you dont want to turn nf the pump when the program goes back to work
      mySwitch.send(antennaOn);//Change Binary Code to your own binary code for turning the pump on
      digitalWrite(StopLED, LOW);

      lastInterrupt = millis();
    }
}

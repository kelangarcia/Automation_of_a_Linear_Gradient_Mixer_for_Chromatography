// Pins for externals tools
const int LED = 18;
const int VCCbutton1 = 19;
const int VCCbutton2 = 14;
const int StopButton = 20;
const int PlayButton = 21;

// variables will change:
volatile int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  pinMode(VCCbutton1, OUTPUT);
  pinMode(VCCbutton2, OUTPUT);
  digitalWrite(VCCbutton1, HIGH);
  digitalWrite(VCCbutton2, HIGH);
  
  pinMode(LED, OUTPUT);
  pinMode(StopButton,INPUT);
  pinMode(PlayButton, INPUT);
  
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(StopButton), PauseProgram,  RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, LOW);
  
}

void PauseProgram(){
  int decider = 1;
  digitalWrite(LED, HIGH);
  while(decider == 1){
    buttonState = digitalRead(PlayButton);
    if (buttonState > 0){
      decider = 0;
    }
  }
}

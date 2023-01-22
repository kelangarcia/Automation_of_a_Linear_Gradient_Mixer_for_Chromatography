// Libraries needed for the Touch LCD and RF transmitter
#include <UTFT.h>
#include <URTouch.h>
#include <RCSwitch.h>
#include <math.h>
#include <stdlib.h>

// Initializing the LCD, Touch Screen and RF Transmitter
UTFT    myGLCD(ILI9341_16,38,39,40,41); //LCD models and pins
URTouch  myTouch(6, 5, 4, 3, 2); // Touch pins
RCSwitch mySwitch = RCSwitch(); //RF Transmitter

// Pins for externals tools
const int VALVE = 8;
const int VCC = 9;
const int Antenna = 10;
const int Compensate_delay = 1;

//Needed variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

int x, y; //x and y position of the user input via the touch screen
char currentPage, selectedUnit; //Page Position
int pump = 0;
int valve = 0;

int RT1 = 0;
int RT2 = 0;
int RA = 0;
int RB = 0;
int RP = 0;


char T1[4] = "";
char A[4] = "";
char T2[4] = "";
char B[4] = "";
char P[2] = "";

int stCurrentLen1=0;
int stCurrentLen2=0;
int stCurrentLen3=0;
int stCurrentLen4=0;
int stCurrentLen5=0;

int time1 = 0;
int percentageA = 0;
int time2 = 0;
int percentageB = 0;
int period = 0;
int once = 0;
char buffer [33];

//Antena Communication Code

const char* antennaOn = "10100110000010000101111000000000";
const char* antennaOff = "10101110000010000101111000000000";

const int pulse_length = 718;
const int protocol = 2;
const int repeat_transmit = 2;

// Pins for Stop Buttons
const int StopLED = 18;
const int ButtonsVcc =19;
const int StopButton = 20;
const int PlayButton = 21;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

unsigned long LastInterrupt; //Checking time passed in ISR vector 

void setup() {
  // Initial setup
  Serial.begin(9600);

  // Given 5V to the VCC(power) pin
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
  
  // Declaring that the valve pin is an output
  pinMode(VALVE, OUTPUT);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(Antenna);

  // Optional set pulse length.
  mySwitch.setPulseLength(pulse_length);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(protocol);

  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(repeat_transmit);
  
  //turning the valve off
  digitalWrite(VALVE, LOW);
  //Change Binary Code to your own binary code for turning the pump off
  mySwitch.send(antennaOff);

  // Initializing and clearing the screen
  myGLCD.InitLCD();
  myGLCD.clrScr();

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_EXTREME);

  //Font and Backcolor
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);

  // StopButton PINS USED
  pinMode(ButtonsVcc, OUTPUT);
  digitalWrite(ButtonsVcc, HIGH);
  pinMode(StopLED, OUTPUT);
  pinMode(StopButton,INPUT);
  pinMode(PlayButton, INPUT);
  
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(StopButton), PauseProgram,  RISING);

  Mainmenu();
  currentPage = '0'; 
}

void loop(){
  //if (myTouch.dataAvailable()){
  //  myTouch.read();
  //  x=myTouch.getX();
  //  y=myTouch.getY();
      
    if (currentPage == '0'){ // Main Page
      if (myTouch.dataAvailable()){
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
        if ((y >= 105) && (y <= 145) && (x >= 35) && (x <= 285)){
          waitForIt(35, 95, 285, 135);
          myGLCD.clrScr();
          startPage();
          currentPage = '1';
        }
        if ((y >= 30) && (y <= 70)){  // Manual 
          if ((x>=22) && (x<=153)){  // Button: Solenoid Valve
            waitForIt(22, 170, 153, 210);
            myGLCD.clrScr();
            solenoidPage();
            currentPage = '2';
          }
          if ((x>=167) && (x<=298)){  // Button: PUMP
            waitForIt(167, 170, 298, 210);
            myGLCD.clrScr();
            pumpPage();
            currentPage = '3';
          }
        }
      }
    }
    
    if (currentPage == '1'){ // Start Page
      if (myTouch.dataAvailable()){
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
            
        //Back to Main Menu
        if ((y >= 203) && (y <= 235) && (x >= 5) && (x <=55)){
          waitForIt(5, 5, 55, 37);

          // Erasing inserted values
          T1[0] = '\0';
          stCurrentLen1 = 0;
          A[0] = '\0';
          stCurrentLen2 = 0;
          T2[0] = '\0';
          stCurrentLen3 = 0;
          B[0] = '\0';
          stCurrentLen4 = 0;
          P[0] = '\0';
          stCurrentLen5 = 0;

          // Drawing Main Menu Page
          myGLCD.clrScr();
          Mainmenu();
          currentPage = '0';
        }
        if((y >= 160) && (y <= 200)){
          if((x >= 160) && (x <= 200)){//1
            waitForIt(160, 40, 200, 80);
            if (RT1 == 1){
              updateStr('1',80,63,1);
            }
            else if(RA == 1){
              updateStr('1',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('1',80,133,3);
            }
            else if(RB == 1){
              updateStr('1',80,165,4);
            }
            else if(RP == 1){
              updateStr('1',80,200,5);
            }
          }
          if((x >= 210) && (x <= 250)){//2
            waitForIt(210, 40, 250, 80);
            if (RT1 == 1){
              updateStr('2',80,63,1);
            }
            else if(RA == 1){
              updateStr('2',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('2',80,133,3);
            }
            else if(RB == 1){
              updateStr('2',80,165,4);
            }
            else if(RP == 1){
              updateStr('2',80,200,5);
            }
          }
          if((x >= 260) && (x <= 300)){//3
            waitForIt(260, 40, 300, 80);
            if (RT1 == 1){
              updateStr('3',80,63,1);
            }
            else if(RA == 1){
              updateStr('3',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('3',80,133,3);
            }
            else if(RB == 1){
              updateStr('3',80,165,4);
            }
            else if(RP == 1){
              updateStr('3',80,200,5);
            }
          }
        }
        if((y >= 110) && (y <= 150)){
          if((x >= 160) && (x <= 200)){//4
            waitForIt(160, 90, 200, 130);
            if (RT1 == 1){
              updateStr('4',80,63,1);
            }
            else if(RA == 1){
              updateStr('4',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('4',80,133,3);
            }
            else if(RB == 1){
              updateStr('4',80,165,4);
            }
            else if(RP == 1){
              updateStr('4',80,200,5);
            }
          }
          if((x >= 210) && (x <= 250)){//5
            waitForIt(210, 90, 250, 130);
            if (RT1 == 1){
              updateStr('5',80,63,1);
            }
            else if(RA == 1){
              updateStr('5',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('5',80,133,3);
            }
            else if(RB == 1){
              updateStr('5',80,165,4);
            }
            else if(RP == 1){
              updateStr('5',80,200,5);
            }
          }
          if((x >= 260) && (x <= 300)){//6
            waitForIt(260, 90, 300, 130);
            if (RT1 == 1){
              updateStr('6',80,63,1);
            }
            else if(RA == 1){
              updateStr('6',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('6',80,133,3);
            }
            else if(RB == 1){
              updateStr('6',80,165,4);
            }
            else if(RP == 1){
              updateStr('6',80,200,5);
            }
          }
        }
        if((y >= 60) && (y <= 100)){
          if((x >= 160) && (x <= 200)){//7
            waitForIt(160, 140, 200, 180);
            if (RT1 == 1){
              updateStr('7',80,63,1);
            }
            else if(RA == 1){
              updateStr('7',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('7',80,133,3);
            }
            else if(RB == 1){
              updateStr('7',80,165,4);
            }
            else if(RP == 1){
              updateStr('7',80,200,5);
            }
          }
          if((x >= 210) && (x <= 250)){//8
            waitForIt(210, 140, 250, 180);
            if (RT1 == 1){
              updateStr('8',80,63,1);
            }
            else if(RA == 1){
              updateStr('8',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('8',80,133,3);
            }
            else if(RB == 1){
              updateStr('8',80,165,4);
            }
            else if(RP == 1){
              updateStr('8',80,200,5);
            }
          }
          if((x >= 260) && (x <= 300)){//9
            waitForIt(260, 140, 300, 180);
            if (RT1 == 1){
              updateStr('9',80,63,1);
            }
            else if(RA == 1){
              updateStr('9',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('9',80,133,3);
            }
            else if(RB == 1){
              updateStr('9',80,165,4);
            }
            else if(RP == 1){
              updateStr('9',80,200,5);
            }
          }
        }
        if((y >= 10) && (y <= 50)){
          if((x >= 160) && (x <= 200)){//C
            waitForIt(160, 190, 200, 230);
            if (RT1 == 1){
              T1[0] = '\0';
              stCurrentLen1 = 0;
              myGLCD.setColor(0, 0, 0);
              myGLCD.fillRect(80, 63, 150, 93);
            }
            else if(RA == 1){
              A[0] = '\0';
              stCurrentLen2 = 0;
              myGLCD.setColor(0, 0, 0);
              myGLCD.fillRect(80, 98, 150, 128);
            }
            else if(RT2 == 1){
              T2[0] = '\0';
              stCurrentLen3 = 0;
              myGLCD.setColor(0, 0, 0);
              myGLCD.fillRect(80, 133, 150, 163);
            }
            else if(RB == 1){
              B[0] = '\0';
              stCurrentLen4 = 0;
              myGLCD.setColor(0, 0, 0);
              myGLCD.fillRect(80, 165, 150, 195);
            }
            else if(RP == 1){
              P[0] = '\0';
              stCurrentLen5 = 0;
              myGLCD.setColor(0, 0, 0);
              myGLCD.fillRect(80, 200, 150, 235);
            } 
          }
          if((x >= 210) && (x <= 250)){//0
            waitForIt(210, 190, 250, 230);
            if (RT1 == 1){
              updateStr('0',80,63,1);
            }
            else if(RA == 1){
              updateStr('0',80,98,2);
            }
            else if(RT2 == 1){
              updateStr('0',80,133,3);
            }
            else if(RB == 1){
              updateStr('0',80,165,4);
            }
            else if(RP == 1){
              updateStr('0',80,200,5);
            }
          }
          if((x >= 260) && (x <= 300)){//->
            waitForIt(260, 190, 300, 230);
            if ((stCurrentLen1 != 0) && (stCurrentLen2 != 0) && (stCurrentLen3 != 0) && (stCurrentLen4 != 0 && (stCurrentLen5 != 0))){
              
              // Converting Strings to INT    
              time1 = convertarraytoint(T1,stCurrentLen1);
              percentageA = convertarraytoint(A,stCurrentLen2);
              time2 = convertarraytoint(T2,stCurrentLen3);
              percentageB = convertarraytoint(B,stCurrentLen4);
              period = convertarraytoint(P,stCurrentLen5);
              
              /*
               You can change the restrictions in here (in the if statement)!!
               To Change time limit, change the inequality value for "time2 <= 360"
               meaning that 360 its the maximum allowed minutes time.
              */
              if ((time1 >= 0) && (time2 > time1) && (time2 <= 360) && (percentageA >= 1) && (percentageB > percentageA) && (percentageB <= 100)  && (period >= 1)  && (period <= 6)){ // Checks if the inputs are correct
                myGLCD.clrScr();
                EnterPage();
                currentPage = '4';
              }
              else{

                myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
                myGLCD.setColor(255, 0, 0); // Sets color to white
                myGLCD.setFont(BigFont); // Sets font to big
                myGLCD.print("Incorrect Input!", 60, 5);

                delay(500);

                myGLCD.setBackColor(0,0,0);
                myGLCD.setColor(0,0,0); // Sets the background color of the area where the text will be printed to black
                myGLCD.fillRoundRect(60, 0, 319, 25);

                delay(500);
                
                myGLCD.setColor(255, 0, 0); // Sets color to white
                myGLCD.print("Incorrect Input!", 60, 5);
                
                delay(1000);

                myGLCD.setBackColor(0,0,0);
                myGLCD.setColor(0,0,0); // Sets the background color of the area where the text will be printed to black
                myGLCD.fillRoundRect(60, 0, 319, 25);

                delay(500);
                
                myGLCD.setColor(255, 255, 255); // Sets color to white
                myGLCD.setFont(BigFont); // Sets font to big
                myGLCD.print("Start Page", 100, 5);
              }
            }
             else{

                myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
                myGLCD.setColor(255, 0, 0); // Sets color to white
                myGLCD.setFont(BigFont); // Sets font to big
                myGLCD.print("Insert Value!", 80, 5);

                delay(500);

                myGLCD.setBackColor(0,0,0);
                myGLCD.setColor(0,0,0); // Sets the background color of the area where the text will be printed to black
                myGLCD.fillRoundRect(60, 0, 319, 25);

                delay(500);
                
                myGLCD.setColor(255, 0, 0); // Sets color to white
                myGLCD.print("Insert Value!", 80, 5);
                
                delay(1000);

                myGLCD.setBackColor(0,0,0);
                myGLCD.setColor(0,0,0); // Sets the background color of the area where the text will be printed to black
                myGLCD.fillRoundRect(60, 0, 319, 25);

                delay(500);
                
                myGLCD.setColor(255, 255, 255); // Sets color to white
                myGLCD.setFont(BigFont); // Sets font to big
                myGLCD.print("Start Page", 100, 5);
              }           
          }
        }
        if((x >= 5) && (x <= 70)){
          if((y >= 155) && (y <= 185)){
            waitForIt(5, 55, 70, 85);
            RT1= 1;
            RA = 0;
            RT2 = 0;
            RB = 0;
            RP = 0;
            values();
          }
          if((y >= 120) && (y <= 150)){
            waitForIt(5, 90, 70, 120);
            RT1= 0;
            RA = 1;
            RT2 = 0;
            RB = 0;
            RP = 0;
            values();
          }
          if((y >= 85) && (y <= 115)){
            waitForIt(5, 125, 70, 155);
            RT1= 0;
            RA = 0;
            RT2 = 1;
            RB = 0;
            RP = 0;
            values();
          }
          if((y >= 50) && (y <= 80)){
            waitForIt(5, 160, 70, 190);
            RT1= 0;
            RA = 0;
            RT2 = 0;
            RB = 1;
            RP = 0;
            values();
          }
          if((y >= 15) && (y <= 45)){
            waitForIt(5, 195, 70, 225);
            RT1= 0;
            RA = 0;
            RT2 = 0;
            RB = 0;
            RP = 1;
            values();
          }
        } 
      }
    }
    
    if (currentPage == '2'){ // Solenoid Valve Page
      if (myTouch.dataAvailable()){
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();

        //Back to Main Menu
        if ((y >= 203) && (y <= 235) && (x >= 5) && (x <=55)){
          waitForIt(5, 5, 55, 37);
          myGLCD.clrScr();
          Mainmenu();
          currentPage = '0';
        }

        // 3V and 12V botton Y position
        if ((y >= 30) && (y <= 70)){ // Buttons
          if ((x >= 22) && (x <= 153)){  // If 3V button is pressed
            waitForIt(22, 170, 153, 210);
            digitalWrite(VALVE, LOW);
            valve = 0;
            myGLCD.setColor(0, 0, 0);// Sets Black color
            myGLCD.fillRoundRect (100,70, 220, 130);// Draws filled rounded rectangle
            myGLCD.setColor(255, 255, 255);// Sets white color
            myGLCD.drawRoundRect (100,70, 220, 130);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame

          }
          if ((x>=167) && (x<=298)){  // Button: 12V
            waitForIt(167, 170, 298, 210);
            digitalWrite(VALVE, HIGH);
            valve = 1;
            myGLCD.setColor(255, 255, 0);// Sets Yellow color
            myGLCD.fillRoundRect (100,70, 220, 130);// Draws filled rounded rectangle
            myGLCD.setColor(255, 255, 255);// Sets white color
            myGLCD.drawRoundRect (100,70, 220, 130);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
          }
        }            
      }        
    }
    
    if (currentPage == '3'){ // Pump Page
      if (myTouch.dataAvailable()){
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
            
        //Back to Main Menu
        if ((y >= 203) && (y <= 235) && (x >= 5) && (x <=55)){
          waitForIt(5, 5, 55, 37);
          myGLCD.clrScr();
          Mainmenu();
          currentPage = '0';
        }
    
        if ((y >= 30) && (y <= 70)){  // OFF & ON position in x
          if ((x>=22) && (x<=153)){  // Button:  OFF
            waitForIt(22, 170, 153, 210);
            myGLCD.setColor(0, 0, 0);// Sets Black color
            myGLCD.fillRoundRect (100,70, 220, 130);// Draws filled rounded rectangle
            myGLCD.setColor(255, 255, 255);// Sets white color
            myGLCD.drawRoundRect (100,70, 220, 130);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
            pump = 0;
            mySwitch.send(antennaOff);//Change Binary Code to your own binary code for turning the pump off

          }
          if ((x>=167) && (x<=298)){  // Button: ON
            waitForIt(167, 170, 298, 210);
            myGLCD.setColor(255, 255, 0);// Sets Yellow color
            myGLCD.fillRoundRect (100,70, 220, 130);// Draws filled rounded rectangle
            myGLCD.setColor(255, 255, 255);// Sets white color
            myGLCD.drawRoundRect (100,70, 220, 130);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
            pump = 1;
            mySwitch.send(antennaOn);//Change Binary Code to your own binary code for turning the pump on

          }
        }
      }
    }
    if (currentPage == '4'){ // Enter Page
      if (myTouch.dataAvailable()){
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
            
        //Back to Main Menu
        if ((y >= 203) && (y <= 235) && (x >= 5) && (x <=55)){
          waitForIt(5, 5, 55, 37);
          myGLCD.clrScr();

          startPage();
          
          // Printing current values
          myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
          myGLCD.setColor(255, 255, 255); // Sets color to white
          myGLCD.print(T1, 80, 63);
          myGLCD.print(A, 80, 98);
          myGLCD.print(T2, 80, 133);
          myGLCD.print(B, 80, 165);
          myGLCD.print(P, 80, 200);

          currentPage = '1';
        }
        if ((y >= 5) && (y <= 37) && (x >= 265) && (x <= 315)){
          waitForIt(265, 203, 315, 235);
          myGLCD.clrScr();
          ProcessPage();
          currentPage = '5';
          once = 0;
        }
      }
    }
    if (currentPage == '5'){ // Process
      if (myTouch.dataAvailable()){
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
            
        //Back to Main Menu
        if ((y >= 203) && (y <= 235) && (x >= 5) && (x <=55)){
          waitForIt(5, 5, 55, 37);
          myGLCD.clrScr();
          EnterPage();
          currentPage = '4';
        }     
      }
      if (once == 0){
        /////////////////////////////////////////////
               int n1,n2;
        int Bandwidth = time2 - time1;
        n1 = time1 * 10;
        
        n2 = Bandwidth * 10;
        double delta = (double)(percentageB - percentageA) / n2;
        double intantpercentageA; 
        unsigned long int timeA = 0;
        unsigned long int timeB = 0;
    
                //Turns Pump ON
        pump = 1;
        mySwitch.send(antennaOn);//Change Binary Code to your own binary code for turning the pump on

        //Prints the current stage
        myGLCD.setColor(255, 255, 255); // Sets color to white
        myGLCD.print("Step: (0, t1] ", 35, 150); // Prints the string

        timeA = percentageA * 60;
        timeB = 6000 - timeA;


        myGLCD.print("Cycle:", 175, 150); // Prints the string
        itoa (n1,buffer,10);
        myGLCD.print(buffer,255, 150);
        myGLCD.print("/", 247, 150); // Prints the string

        itoa (1,buffer,10);
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(255, 255, 255);// Sets Black color
        myGLCD.print(buffer,228, 150);
            
        valve = 1;
        myGLCD.setColor(255, 255, 0);// Sets Yellow color
        myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle
        myGLCD.setColor(0, 0, 0);// Sets Black color
        myGLCD.fillRoundRect (175,70, 285, 130);
        myGLCD.setBackColor(255, 255, 0);
        itoa ((timeA - (15 * Compensate_delay) ),buffer,10);
        myGLCD.print(buffer,(75), 93);
        digitalWrite(VALVE, HIGH);
        delay(timeA - (15 * Compensate_delay));

        valve = 0;
        myGLCD.setColor(255, 255, 0);
        myGLCD.fillRoundRect (175,70, 285, 130);
        myGLCD.setColor(0, 0, 0);// Sets Black color
        myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle
        itoa ((timeB + (5 * Compensate_delay)),buffer,10);
        myGLCD.print(buffer,(210), 93);
        digitalWrite(VALVE, LOW);
        delay(timeB + (5 * Compensate_delay));
        
        for (int x = 1; x < n1; x++){//Step 1
      
            itoa ((x + 1),buffer,10);
            myGLCD.setBackColor(0, 0, 0);
            myGLCD.setColor(255, 255, 255);// Sets Black color
            myGLCD.print(buffer,228, 150);
            
            valve = 1;
            myGLCD.setColor(255, 255, 0);// Sets Yellow color
            myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle
            myGLCD.setColor(0, 0, 0);// Sets Black color
            myGLCD.fillRoundRect (175,70, 285, 130);
            myGLCD.setBackColor(255, 255, 0);
            itoa ((timeA - (5 * Compensate_delay)),buffer,10);
            myGLCD.print(buffer,(75), 93);
            digitalWrite(VALVE, HIGH);
            delay(timeA - (5 * Compensate_delay));

            valve = 0;
            myGLCD.setColor(255, 255, 0);
            myGLCD.fillRoundRect (175,70, 285, 130);
            myGLCD.setColor(0, 0, 0);// Sets Black color
            myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle
            itoa ((timeB + (5 * Compensate_delay)),buffer,10);
            myGLCD.print(buffer,(210), 93);
            digitalWrite(VALVE, LOW);
            delay(timeB + (5 * Compensate_delay));
           
        }

        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(0, 0, 0);// Sets Black color
        myGLCD.fillRoundRect (35,150, 200, 170);
        myGLCD.setColor(255, 255, 255); // Sets color to white
        myGLCD.print("DONE!", 35, 150); // Prints the string
        
        //Step 2
        myGLCD.setColor(255, 255, 255); // Sets color to white
        myGLCD.print("Step: (t1, t2] ", 35, 170); // Prints the string

        timeA = 0;
        timeB = 0;

        myGLCD.print("Cycle:", 175, 150); // Prints the string
        itoa (n2,buffer,10);
        myGLCD.print(buffer,255, 170);
        myGLCD.print("/", 247, 170); // Prints the string
        
        for (int x = 0; x <= n2; x++){//Step 2

            itoa (x,buffer,10);
            myGLCD.setBackColor(0, 0, 0);
            myGLCD.setColor(255, 255, 255);// Sets Black color
            myGLCD.print(buffer,200, 170);
          
            intantpercentageA = percentageA + delta * x;
            timeA = (unsigned long int)floor((double) intantpercentageA * 60);
            timeB = 6000 - timeA;
    
            valve = 1;
            myGLCD.setColor(255, 255, 0);// Sets Yellow color
            myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle
            myGLCD.setColor(0, 0, 0);// Sets Black color
            myGLCD.fillRoundRect (175,70, 285, 130);
            myGLCD.setBackColor(255, 255, 0);
            itoa ((timeA - (5 * Compensate_delay)),buffer,10);
            myGLCD.print(buffer,(75), 93);
            digitalWrite(VALVE, HIGH);
            delay(timeA - (5 * Compensate_delay));
            
            valve = 0;
            myGLCD.setColor(255, 255, 0);
            myGLCD.fillRoundRect (175,70, 285, 130);
            myGLCD.setColor(0, 0, 0);// Sets Black color
            myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle
            itoa ((timeB + (5 * Compensate_delay)),buffer,10);
            myGLCD.print(buffer,(210), 93);
            digitalWrite(VALVE, LOW);
            delay(timeB + (5 * Compensate_delay));
        }

        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(0, 0, 0);// Sets Black color
        myGLCD.fillRoundRect (35,170, 200, 190);
        myGLCD.setColor(255, 255, 255); // Sets color to white
        myGLCD.print("T2 Done", 35, 170); // Prints the string

        pump = 0;
        mySwitch.send(antennaOff);//Change Binary Code to your own binary code for turning the pump off
          
        valve = 0;
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(0, 0, 0);// Sets Black color
        myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle 
        myGLCD.setColor(255, 255, 255);// Sets white color
        myGLCD.drawRoundRect (34,69, 146, 131);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRoundRect (175,70, 285, 130);
        myGLCD.setColor(255, 255, 255);
        myGLCD.drawRoundRect (174,69, 286, 131);
        digitalWrite(VALVE, LOW);

        // Button - Back
        myGLCD.setColor(16, 167, 103);// Sets green color
        myGLCD.fillRoundRect (5,5, 55, 37);// Draws filled rounded rectangle
          
        myGLCD.setColor(255, 255, 255);// Sets white color
        myGLCD.drawRoundRect (5,5, 55, 37);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
          
        myGLCD.setFont(BigFont);
        myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
        myGLCD.print("<-",15,15);

        /////////////////////////////////////////////
        
            // Button - Back
          myGLCD.setColor(16, 167, 103);// Sets green color
          myGLCD.fillRoundRect (5,5, 55, 37);// Draws filled rounded rectangle
          
          myGLCD.setColor(255, 255, 255);// Sets white color
          myGLCD.drawRoundRect (5,5, 55, 37);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
          
          myGLCD.setFont(BigFont);
          myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
          myGLCD.print("<-",15,15);
  
          once = 1;
      }
    }
  //}
}

void PauseProgram(){
  if(millis() - LastInterrupt > 10) {// we set a 10ms no-interrupts window  
   
    if (currentPage == '5'){
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
    }
    LastInterrupt = millis();
  }
}

void Mainmenu(){// Draw the Main Menu of the app
  // Title
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Linear Gradient", CENTER, 10); // Prints the string on the screen
  myGLCD.print("Mixer", CENTER, 30);

  //Red Line
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,50,319,50); // Draws the red line
  
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("by Kelan Garcia", CENTER, 55); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Automatic", CENTER, 75);
  myGLCD.print("Manual", CENTER, 142);

  // Button - Start
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 95, 285, 135); // Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 95, 285, 135); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("START", CENTER, 106); // Prints the string
  
  
  // Button - Solenoid Valve
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (22, 170, 153, 210);// Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (22, 170, 153, 210);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("SOLENOID",25,175);
  myGLCD.print("VALVE", 50,190);
  
  // Button - Pump
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (167, 170, 298, 210);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (167, 170, 298, 210);
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("PUMP", 200, 183);
}
// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}

void values(){
       // Values
  // T1
  if (RT1 == 0) {
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(0, 0, 0);
  }
  else if (RT1 == 1){
    myGLCD.setColor(255, 0, 0);
    myGLCD.setBackColor(255, 0, 0);
  }
  myGLCD.fillRoundRect (5, 55, 70, 85);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (5, 55, 70, 85);
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("T1:", 15, 63);

  // A
  if (RA == 0) {
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(0, 0, 0);
  }
  else if (RA == 1){
    myGLCD.setColor(255, 0, 0);
    myGLCD.setBackColor(255, 0, 0);
  }
  myGLCD.fillRoundRect (5, 90, 70, 120);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (5, 90, 70, 120);
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("%T1", 10, 98);

  // T2
  if (RT2 == 0) {
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(0, 0, 0);
  }
  else if (RT2 == 1){
    myGLCD.setColor(255, 0, 0);
    myGLCD.setBackColor(255, 0, 0);
  }
  myGLCD.fillRoundRect (5, 125, 70, 155);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (5, 125, 70, 155);
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("T2:", 15, 133);

  // B
  if (RB == 0) {
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(0, 0, 0);
  }
  else if (RB == 1){
    myGLCD.setColor(255, 0, 0);
    myGLCD.setBackColor(255, 0, 0);
  }
  myGLCD.fillRoundRect (5, 160, 70, 190);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (5, 160, 70, 190);
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("%T2", 10, 165);

  // P
  if (RP == 0) {
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(0, 0, 0);
  }
  else if (RP == 1){
    myGLCD.setColor(255, 0, 0);
    myGLCD.setBackColor(255, 0, 0);
  }
  myGLCD.fillRoundRect (5, 195, 70, 225);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (5, 195, 70, 225);
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("P", 10, 200);
}

void startPage(){
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Start Page", 100, 5);

  // Button - Back
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (5,5, 55, 37);// Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (5,5, 55, 37);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("<-",15,15);

                  // Numpad
  // 1 - 3 buttons
  for (x=0; x<3; x++){
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRoundRect (160+(x*50), 40, 200+(x*50), 80);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (160+(x*50), 40, 200+(x*50), 80);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(x+1, 173+(x*50), 53);
  }
  // 4 - 6 buttons
  for (x=0; x<3; x++){
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRoundRect (160+(x*50), 90, 200+(x*50), 130);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (160+(x*50), 90, 200+(x*50), 130);
    if (x<4)
      myGLCD.printNumI(x+4, 173+(x*50), 103);
  }
  // 7 - 9 buttons
  for (x=0; x<3; x++){
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRoundRect (160+(x*50), 140, 200+(x*50), 180);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (160+(x*50), 140, 200+(x*50), 180);
    if (x<4)
      myGLCD.printNumI(x+7, 173+(x*50), 153);
  }
  // C - 0 - -> buttons
  for (x=0; x<3; x++){
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRoundRect (160+(x*50), 190, 200+(x*50), 230);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (160+(x*50), 190, 200+(x*50), 230);
  }
  myGLCD.print("C", 173, 203);
  myGLCD.printNumI(0, 223, 203);
  myGLCD.print("->", 265, 203);
  
  RT1= 0;
  RA = 0;
  RT2 = 0;
  RB = 0;
  RP = 0;
  values();
}

void solenoidPage(){
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Solenoid Page", 90, 15);

  // Button - Back
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (5,5, 55, 37);// Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (5,5, 55, 37);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("<-",15,15);

  // Indicator:  
   
  if (valve == 0){
    myGLCD.setColor(0, 0, 0);// Sets green color
  }
  else if(valve == 1){
    myGLCD.setColor(255, 255, 0);// Sets Yellow color
  }
  myGLCD.fillRoundRect (100,70, 220, 130);// Draws filled rounded rectangle

  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (100,70, 220, 130);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  // Button - 3V
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (22, 170, 153, 210);// Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (22, 170, 153, 210);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("3V",25,175);

  
  // Button - 12V
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (167, 170, 298, 210);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (167, 170, 298, 210);
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("12V", 200, 183);
}

void pumpPage(){
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Pump Page", CENTER, 10);

  // Button - Back
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (5,5, 55, 37);// Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (5,5, 55, 37);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("<-",15,15);

  // Indicator:    
  if (pump == 0){
    myGLCD.setColor(0, 0, 0);// Sets Black color
  }
  else if(pump == 1){
    myGLCD.setColor(255, 255, 0);// Sets Yellow color
  }
  myGLCD.fillRoundRect (100,70, 220, 130);// Draws filled rounded rectangle

  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (100,70, 220, 130);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame

  
    // Button - OFF
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (22, 170, 153, 210);// Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (22, 170, 153, 210);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("OFF",25,175);
  
  // Button - ON
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (167, 170, 298, 210);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (167, 170, 298, 210);
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("ON", 200, 183);
}

void updateStr(int val, int x, int y, int decider){
  if (decider == 1){
    if (stCurrentLen1 < 3){
      T1[stCurrentLen1] = val;
      T1[stCurrentLen1+1] = '\0';
  
      stCurrentLen1++;
      myGLCD.setColor(255, 255, 250);
      myGLCD.print(T1, x, y);
    }
  }

    if (decider == 2){
      if (stCurrentLen2 < 3){
        A[stCurrentLen2] = val;
        A[stCurrentLen2+1] = '\0';
  
        stCurrentLen2++;
        myGLCD.setColor(255, 255, 250);
        myGLCD.print(A, x, y);
      }
    }

  if (decider == 3){
    if (stCurrentLen3 < 3){
        T2[stCurrentLen3] = val;
        T2[stCurrentLen3+1] = '\0';
    
        stCurrentLen3++;
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(255, 255, 250);
        myGLCD.print(T2, x, y);
    }
  }

    if (decider == 4){
      if (stCurrentLen4 < 3){
        B[stCurrentLen4] = val;
        B[stCurrentLen4+1] = '\0';
    
        stCurrentLen4++;
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(255, 255, 250);
        myGLCD.print(B, x, y);
      }
    }
    if (decider == 5){
      if (stCurrentLen5 < 1){
        P[stCurrentLen5] = val;
        P[stCurrentLen5+1] = '\0';
    
        stCurrentLen5++;
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setColor(255, 255, 250);
        myGLCD.print(P, x, y);
      }
    }
}


void EnterPage(){
  myGLCD.fillScr(255, 255, 255);
  myGLCD.setBackColor(255,255,255); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(0, 0, 0); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Enter Page", 100, 5);

  // Button - Back
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (5,5, 55, 37);// Draws filled rounded rectangle
  
  myGLCD.setColor(0, 0, 0);// Sets white color
  myGLCD.drawRoundRect (5,5, 55, 37);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("<-",15,14);


   // Button - Continue
  myGLCD.setColor(16, 167, 103);// Sets green color
  myGLCD.fillRoundRect (265, 203, 315, 235);// Draws filled rounded rectangle
  
  myGLCD.setColor(0, 0, 0);// Sets Black color
  myGLCD.drawRoundRect (265, 203, 315, 235);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);// Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("->",275,212);

  // Draw crosshairs
  myGLCD.setBackColor(255, 255, 255);
  
  if(time2 <= 300){
    
    myGLCD.setColor(0, 0, 255);
    myGLCD.setBackColor(255, 255, 255);
      // X axis
    myGLCD.drawLine(5, 195, 315, 195);
    
    myGLCD.setColor(255, 0, 0);
    
    myGLCD.drawLine(15, (195 - percentageA * 1.5), (15 + time1), (195 - percentageA * 1.5));
    myGLCD.setFont(SmallFont); // Sets the font to small
    myGLCD.print(T1,(15 + time1), 200);
   
    myGLCD.print(A,1, (195 - percentageA * 1.5));
    
    myGLCD.drawLine((15 + time1) , (195 - percentageA * 1.5), (15 + time2), (195 - percentageB * 1.5));
    myGLCD.print(T2,(15 + time2), 200);
    
    myGLCD.print(B,1, (195 - percentageB * 1.5));
    
    myGLCD.setColor(0, 0, 255);
    myGLCD.setBackColor(255, 255, 255);
    // Y axis
    myGLCD.drawLine(15, 45, 15, 210);
  }
  else{
    myGLCD.setColor(0, 0, 255);
    myGLCD.setBackColor(255, 255, 255);
      // X axis
    myGLCD.drawLine(5, 195, 315, 195);
    
    myGLCD.setColor(255, 0, 0);
    
    myGLCD.drawLine(15, (195 - percentageA * 1.5), (15 + time1/1.2), (195 - percentageA * 1.5));
    myGLCD.setFont(SmallFont); // Sets the font to small
    myGLCD.print(T1,(15 + time1/1.2), 200);
   
    myGLCD.print(A,1, (195 - percentageA * 1.5));
    
    myGLCD.drawLine((15 + time1/1.2) , (195 - percentageA * 1.5), (15 + time2/1.2), (195 - percentageB * 1.5));
    myGLCD.print(T2,(15 + time2/1.2), 200);
    
    myGLCD.print(B,1, (195 - percentageB * 1.5));
    
    myGLCD.setColor(0, 0, 255);
    myGLCD.setBackColor(255, 255, 255);
    // Y axis
    myGLCD.drawLine(15, 45, 15, 210);
  }
}


int convertarraytoint(char text[], int len){
  int total = 0;
  char helper;
  int var;
  for (int x = 0; x < len; x++){
    helper = text[len - 1 - x];
    if (x != 2){
      var = ((int) helper - 48) * pow(10,x);
    }
    else if (x == 2){
      var = ((int) helper - 48) * pow(10,2);
    }
    total = total + var ;
    var = 0;
  }
  if (len >= 4){
    total += 1;
  }
  return total;
}

void ProcessPage(){
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Process Page", CENTER, 10);

    // Button - OFF
  myGLCD.setColor(0, 0, 0);// Sets Black color
  myGLCD.fillRoundRect (35,70, 145, 130);// Draws filled rounded rectangle
  
  myGLCD.setColor(255, 255, 255);// Sets white color
  myGLCD.drawRoundRect (34,69, 146, 131);// Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  
  
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.print("POLAR",75,55);
  
  // Button - ON
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRoundRect (175,70, 285, 130);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (174,69, 286, 131);

  
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.print("NON POLAR", 200, 55);
}

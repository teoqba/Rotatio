#include <MicroView.h>    // include MicroView library

#define MAXROT 30         // maximum number of allowed rotations
#define N_SLOW 4          // number of last slower cycles
#define CYCLE_DELAY 2000  // delay between cycles in ms
#define N_MAG 3           // number of magnets around the spindle

int pinREED      = 2;  // uView pin 11
int pinMosfet    = 6;  // uView pin 14
int pinPot       = A3; // uView pin 4
int counter      = 0;
int smallCounter = 0;
int reedVal;
int rotMax;        //max number of rotations
  
int tStart;
boolean timerStart = true;
boolean halt = false;

void uViewDisplay(int rotMax, int counter) {
  // Controls information to be displayed on uView screen
  // Define an offset to move the digits if the rotMax of counter 
  // are single digit numbers. This number depends on the Font width.
  int offset  = 0;  
  uView.clear(PAGE);
  if (counter < 10) offset = 10;
  uView.setCursor(0+offset,0);
  uView.setFontType(3);
  uView.print(counter);
  offset = 0;
  if (rotMax < 10) offset = 10;
  uView.setCursor(43+offset,0);
  uView.setFontType(2);
  uView.print(rotMax);
  //uView.clear(PAGE);
  uView.display();
}
 
void checkTimer(int tStart, int *counter, int *smallCounter, boolean *halt, boolean *timerStart)
// Check if the time after the drill stopped passed.
// If yes :
//    ___RESET___ the counter !! 
//    close the MOSFET and set the motor ready for new set or rotations
// 
{
  int currentTime = millis();
        if (currentTime - tStart > CYCLE_DELAY){
           *counter = 0;
           *smallCounter = 0;
           *halt = false;
           digitalWrite(pinMosfet,1);
           *timerStart = true;
        }
}
  
int readRotMax(int pinPot)
{
  // Sets number of rotations from potentiometer readout
  int tmp = analogRead(pinPot);
  int out = map(tmp, 0, 1023, 0, MAXROT);
  return out;
}

void setup() 
{
  Serial.begin(115200);
  pinMode(pinMosfet,OUTPUT);
  digitalWrite(pinMosfet,HIGH);
  pinMode(pinREED,INPUT);
  uView.begin();
  uView.clear(PAGE);
  uView.display();

}
  
void loop() 
{
  // Read value of potentiomter and update the LCD
  rotMax = readRotMax(pinPot);
  uViewDisplay(rotMax, counter);
  
  reedVal = digitalRead(pinREED);
  delayMicroseconds(1);
  if (reedVal)
  {
    
    while (reedVal) { 
      // Wait until magnet passes the swtich before incrementing the counter.
      reedVal = digitalRead(pinREED);
      
      // Allow for control while REED swtich is closed
      rotMax = readRotMax(pinPot);
      uViewDisplay(rotMax, counter);
      
      if (halt){
        // Keep checking the timer while  REED switch is closed 
        checkTimer(tStart, &counter, &smallCounter, &halt, &timerStart);
      }
    }
    // Increment the counter and update uView display
    smallCounter ++;
    if (smallCounter >= N_MAG) {
      counter ++;
      smallCounter = 0;
    }
    Serial.println(counter);
    uViewDisplay(rotMax, counter);
    
    // Slow down for last two rotations
    if (counter == rotMax - N_SLOW){
      analogWrite(pinMosfet, 100);
    }      
    if (counter >= rotMax){
      halt = true;
    }
    
  }
  if (halt){
    if (timerStart){
      tStart = millis();
      timerStart = false;
      digitalWrite(pinMosfet,0);
    }
    checkTimer(tStart, &counter, &smallCounter, &halt, &timerStart);
  }
}

    
    
    
    
      

#include<LiquidCrystal.h>
// LCD pinout
// lcdRS - 12, lcdEN - 11, lcd7 - 2, lcd6 - 3, lcd5 - 4, lcd4 - 5

#define MAXROT 60         // maximum number of allowed rotations
#define N_SLOW 4          // number of last slower cycles
#define CYCLE_DELAY 2000  // delay between cycles in ms

LiquidCrystal lcd(12,11,5,4,3,2);
int pinREED   = 8;
int pinMosfet = 9;
int pinPot    = A0; //potentiometer pin
int counter   = 0;
int reedVal;
int rotMax;        //max number of rotations
  
int tStart;
boolean timerStart = true;
boolean halt = false;

void lcdDisplay(int rotMax, int counter) {
  // Controls information to be displayed on LCD screen
  lcd.setCursor(0,0);
  // When rotMax has fewer than 2 digits, display string with one 
  // white space more
  if (rotMax < 10){
    lcd.print("Max. rot.:  ");
   }
  else {
    lcd.print("Max. rot.: ");
  }
  lcd.print(rotMax);
  lcd.setCursor(0,1);
  if (counter < 10) {
    lcd.print("Counter:  ");
  }
  else{
    lcd.print("Counter: ");
  }
  lcd.print(counter);
}
 
void checkTimer(int tStart, int *counter, boolean *halt, boolean *timerStart)
// Check if the time after the drill stopped passed.
// If yes :
//    ___RESET___ the counter !! 
//    clos the MOSFET and set the motor ready for new set or rotations
// 
{
  int currentTime = millis();
        if (currentTime - tStart > CYCLE_DELAY){
           *counter = 0;
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
  lcd.begin(16,2);
}
  
void loop() 
{
  // Read value of potentiomter and update the LCD
  rotMax = readRotMax(pinPot);
  lcdDisplay(rotMax, counter);
  
  reedVal = digitalRead(pinREED);
  delayMicroseconds(1);
  if (!reedVal)
  {
    
    while (!reedVal) { 
      // Wait until magnet passes the swtich before incrementing the counter.
      reedVal = digitalRead(pinREED);
      
      // Allow for control while REED swtich is closed
      rotMax = readRotMax(pinPot);
      lcdDisplay(rotMax, counter);
      
      if (halt){
        // Keep checking the timer while  REED switch is closed 
        checkTimer(tStart, &counter, &halt, &timerStart);
      }
    }
    // Increment the counter and update LCD display
    counter ++;
    Serial.println(counter);
    lcdDisplay(rotMax, counter);
    
    // Slow down for last two rotations
    if (counter == rotMax - N_SLOW){
      analogWrite(pinMosfet,100);
    }      
    if (counter == rotMax){
      halt = true;
    }
    
  }
  if (halt){
    if (timerStart){
      tStart = millis();
      timerStart = false;
      digitalWrite(pinMosfet,0);
    }
    checkTimer(tStart, &counter, &halt, &timerStart);
  }
}

    
    
    
    
      

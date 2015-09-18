#include<LiquidCrystal.h>
//lcdRS - 12
//lcdEN - 11
//lcd7 - 2
//lcd6 - 3
//lcd5 - 4
//lcd4 - 5

#define MAXROT 60 //maximum number of allowed rotations
#define N_SLOW 4 //number of last slower cycles

LiquidCrystal lcd(12,11,5,4,3,2);
int pinREED = 8;
int pinMosfet = 9;
int pinPot = A0; //potentiometer pin
int reedVal = 1;
int counter = 0;
int rotMax; //max number of rotations

int tStart;
int myTimer;
boolean timerStart = true;
boolean halt = false;



void setup() {
  Serial.begin(115200);
  pinMode(pinMosfet,OUTPUT);
  pinMode(pinREED,INPUT);
  digitalWrite(pinMosfet,HIGH);
  lcd.begin(16,2);
  rotMax = 40; //default value for maximum # of rotations
}


oid lcdDisplay(int rotMax, int counter) {
  // Controls information to be displayed on LCD screen
  lcd.setCursor(0,0);
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
 
  
int readRotMax(int pinPot){
  // Sets number of rotations from potentiometer readout
  int tmp = analogRead(pinPot);
  int out = map(tmp,0,1023,0,MAXROT);
  return out;
}
  
  
void loop() {
  rotMax = readRotMax(pinPot);
  lcdDisplay(rotMax, counter);
  reedVal = digitalRead(pinREED);
  delayMicroseconds(1);
  if (!reedVal)
  {
    
    while (!reedVal) { 
      // Wait until magnet passes the swtich before incrementing the counter.
      reedVal = digitalRead(pinREED);
      
      // If it happens that the drill stopped with magnet facing read switch
      // we still want to be able to change to # of max allowed rotations 
      
      rotMax = readRotMax(pinPot);
      lcdDisplay(rotMax, counter);
      
      if (halt){
        myTimer = millis();      
        if (myTimer-tStart > 2000){
           counter = 0;
           halt = false;
           digitalWrite(pinMosfet,1);
           timerStart = true;
        }
      }
    }
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
    myTimer = millis();      
    if (myTimer-tStart > 2000){
       counter = 0;
       halt = false;
       digitalWrite(pinMosfet,1);
       timerStart = true;
    }
  }
}

    
    
    
    
      

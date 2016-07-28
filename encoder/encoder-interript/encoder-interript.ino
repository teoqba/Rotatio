int pinA = 2;
int pinB = 3;
int pinC = 4;
int encoderPos = 0;
int revs = 0;

void doEncoderA() {

  if (digitalRead(pinA) == HIGH) { //low to high on A
    if (digitalRead(pinB) == HIGH) {
      encoderPos++; //CW
    } else {
      encoderPos--; //CCW
    }
  } else { // high to low on A
    if (digitalRead(pinB) == LOW) { 
      encoderPos++; //CW
    } else {
      encoderPos--;
    }
  }

}

void doEncoderB(){
  if (digitalRead(pinB)== HIGH){ // low to high
    if (digitalRead(pinA) == LOW) {
      encoderPos++;
    } else {
      encoderPos--;
    }
  } else {
    if(digitalRead(pinA) == HIGH) { //high-to-low
      encoderPos++;
    } else {
      encoderPos--;
    }
  }
}

void setup() {
  
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
//  digitalWrite(pinA, HIGH);
//  digitalWrite(pinB, HIGH);
  Serial.begin(9600);
  attachInterrupt(0, doEncoderA, CHANGE); //0 is interrupt on pin 2
  attachInterrupt(1, doEncoderB, CHANGE); //1 is interrupt on pin 3
}

void loop() {
  if (encoderPos/4 == 2048) {
    revs++;
    encoderPos = 0;
    Serial.println(revs);
    }
//  Serial.println(encoderPos);
}


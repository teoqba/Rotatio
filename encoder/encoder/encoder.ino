int pinA = 2;
int pinB = 3;
int pinC = 4;
int state = LOW;
int lastState = LOW;
int encoderPos = 0;
int fullCircle = LOW;
int lastFullCircle = LOW; 
int rotCount = 0;

void setup() {
  
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  Serial.begin(9600);

}

void loop() {
  state = digitalRead(pinA);
  fullCircle = digitalRead(pinC);
//  if ((fullCircle) && (lastFullCircle == LOW)){
//    Serial.print("Pelne kolo ");
//    Serial.println(encoderPos);}
  if ((state == HIGH) && (lastState == LOW)) {
    if (digitalRead(pinB) == HIGH) {
      encoderPos++;  
    }
    else {
      encoderPos--;
    }
  }
  lastState = state;
  lastFullCircle = fullCircle;
  Serial.println(encoderPos);
  
}


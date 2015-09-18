//H-Bridge pin 3 - Motor Plus
//H-Bridge pin 6 - Motor minus


int pinPlus   = 2;  // H-Bridge pin 7
int pinMinus  = 3;   // H-Bridge pin 2
int pinEnable = 9;  // H-Bridge pin 1
int pinButton = 10;

 
void setup(){
  Serial.begin(115200);
  pinMode(pinEnable,OUTPUT);
  pinMode(pinPlus,OUTPUT);
  pinMode(pinMinus,OUTPUT);
  pinMode(pinButton,INPUT);
  digitalWrite(pinEnable,HIGH);
  digitalWrite(pinPlus,HIGH);
  digitalWrite(pinMinus,LOW);
}

void loop(){
  int bStatus = digitalRead(pinButton);
  Serial.println(bStatus);
  if (bStatus) digitalWrite(pinEnable,HIGH);
  if (!bStatus) digitalWrite(pinEnable,LOW);
  delay(5000);
  digitalWrite(pinPlus,LOW);
  digitalWrite(pinMinus,HIGH);
  delay(5000);
  digitalWrite(pinMinus,LOW);
  digitalWrite(pinPlus,HIGH);
}
  

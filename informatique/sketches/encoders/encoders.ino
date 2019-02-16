#define motorRightC1 2
#define motorRightC2 6

#define motorLeftC1 3
#define motorLeftC2 7

volatile unsigned int motorRightPos = 0;
volatile unsigned int motorLeftPos = 0;

unsigned int tmpR = 0;
unsigned int tmpL = 0;

unsigned int oldRC1 = 0;
unsigned int newRC1 = 0;
unsigned int RC2 = 0;

unsigned int oldLC1 = 0;
unsigned int newLC1 = 0;
unsigned int LC2 = 0;

void setup() {
  pinMode(motorRightC1, INPUT);
  pinMode(motorLeftC1, INPUT);
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderRight, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderLeft, CHANGE);
  // set up the Serial Connection
  Serial.begin (115200);
}

void loop() {
  //Check each changes in position
  if (tmpR != motorRightPos) {
    Serial.println(motorRightPos, DEC);
    tmpR = motorRightPos;
  }
   //Check each changes in position
  if (tmpL != motorLeftPos) {
    Serial.println(motorLeftPos, DEC);
    tmpL = motorLeftPos;
  }
  delay(500);
}

// Interrupt on Right changing state
void doEncoderRight() {
  delay(1);
  newRC1 = digitalRead(motorRightC1);
  RC2 = digitalRead(motorLeftC2);
  if (oldRC1 == LOW && newRC1 == HIGH && RC2 == LOW){
    motorRightPos++;
  }
  else if (oldRC1 == HIGH && newRC1 == LOW && RC2 == HIGH){
    motorRightPos++;
  }
  else
  {
    motorRightPos--;
  }
  oldRC1 = newRC1;
}

// Interrupt on Left changing state
void doEncoderLeft() {
  delay(1);
  newLC1 = digitalRead(motorLeftC1);
  LC2 = digitalRead(motorLeftC2);
  if (oldLC1 == LOW && newLC1 == HIGH && LC2 == HIGH){
    motorLeftPos++;
  }
  else if (oldLC1 == HIGH && newLC1 == LOW && LC2 == LOW){
    motorRightPos++;
  }
  else
  {
    motorLeftPos--;
  }
  oldLC1 = newLC1;
  
  //LC1new = digitalRead(encoder0PinB);
  //LC1new^LC2old ? encoder0Pos++ : encoder0Pos--;
}

#include <VL53L0X.h>
#include <Wire.h>

VL53L0X TOF1;
VL53L0X TOF2;

//pin numbers
//input
const int buttonPin = A0;    
const int groundSensor1 = A6;
const int groundSensor2 = A7;
const int encoderRA = 2;
const int encoderRB = 7;
const int encoderLA = 3;
const int encoderLB = 8;
//output
//const int ledPin =  13;
const int rgbRed =  A1;
const int rgbGreen =  A2;
const int rgbBlue =  A3;
const int xshut1 = 12;
const int xshut2 = 13;
const int IN1 = 6;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

// variables 
int buttonState = 0;
int valGroundSensor1 = 0;
int valGroundSensor2 = 0;
int valTOF1 = 0;
int valTOF2 = 0;
int timeoutTOF1 = 0 ;
int timeoutTOF2 = 0 ;
long valueLeft = 0;
long valueRight = 0;
int speedMotor = 20;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(10);
    
  Serial.println("begin init");

  // output
  //pinMode(ledPin, OUTPUT);
  pinMode(rgbRed, OUTPUT);
  pinMode(rgbGreen, OUTPUT);
  pinMode(rgbBlue, OUTPUT);
  pinMode(xshut1,OUTPUT);
  pinMode(xshut2,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  delay(10);

  // input
  pinMode(buttonPin, INPUT);
  pinMode(groundSensor1, INPUT);
  pinMode(groundSensor2, INPUT);
  delay(10);

  // ToF
  pinMode(xshut2, INPUT);
  delay(10);
  
  TOF2.setAddress(42);
  pinMode(xshut1, INPUT);
  delay(10);
  
  TOF1.init();
  TOF2.init();
  
  TOF1.setTimeout(500);
  TOF2.setTimeout(500);
  
  TOF1.startContinuous();
  TOF2.startContinuous();

  // interuption
  attachInterrupt(digitalPinToInterrupt(encoderLA), encoderLeft, CHANGE );
  attachInterrupt(digitalPinToInterrupt(encoderRA), encoderRight, CHANGE );
  
  Serial.println("init done");  
}

void loop() {
  // read sensors
  buttonState = digitalRead(buttonPin);

  delay(1);
  valGroundSensor1 = analogRead(groundSensor1);
  valGroundSensor2 = analogRead(groundSensor2);

  valTOF1 = TOF1.readRangeContinuousMillimeters();
  if (TOF1.timeoutOccurred()) { timeoutTOF1 = 1; } else {timeoutTOF1 = 0;}
  
  valTOF2 = TOF2.readRangeContinuousMillimeters();
  if (TOF2.timeoutOccurred()) { timeoutTOF2 = 1; } else {timeoutTOF2 = 0;}

  // Actions
  if (buttonState == 1) {
    // turn LED on:
    //digitalWrite(ledPin, HIGH);
    digitalWrite(rgbRed, HIGH);
    digitalWrite(rgbGreen, LOW);
    digitalWrite(rgbBlue, LOW);
    analogWrite(IN1,0);
    analogWrite(IN2,speedMotor);
    analogWrite(IN3,0);
    analogWrite(IN4,speedMotor);
  }
  else
  {
    // turn LED off:
    //digitalWrite(ledPin, LOW);
    digitalWrite(rgbRed, LOW);
    digitalWrite(rgbGreen, HIGH);
    digitalWrite(rgbBlue, LOW);
    analogWrite(IN1,speedMotor);
    analogWrite(IN2,0);
    analogWrite(IN3,speedMotor);
    analogWrite(IN4,0);
  }
  printStatus();
}


void printStatus()
{
  Serial.print("button ");
  Serial.print(buttonState);
  
  Serial.print("\tground 1 ");
  Serial.print(valGroundSensor1);
  Serial.print("\tground 2 ");
  Serial.print(valGroundSensor2);
  
  Serial.print("\tTOF1 ");
  if(timeoutTOF1) Serial.print("TIMEOUT"); else Serial.print(valTOF1);
  Serial.print("\tTOF2 ");
  if(timeoutTOF2) Serial.print("TIMEOUT"); else Serial.print(valTOF2);
  
  Serial.print("\tEncoder Left ");
  Serial.print(valueLeft);
  Serial.print("\tEncoder Right ");
  Serial.print(valueRight);
  
  Serial.println("");
}

// count encoder
void encoderLeft()
{
  if(digitalRead(encoderLA) == 1)
  {
    if(digitalRead(encoderLB) == 1)
    valueLeft--;
    else
    valueLeft++;
  }
  else // LA = 0
  {
    if(digitalRead(encoderLB) == 1)
    valueLeft++;
    else
    valueLeft--;
  }
}

// count encoder
void encoderRight()
{
  if(digitalRead(encoderRA) == 1)
  {
    if(digitalRead(encoderRB) == 1)
    valueRight--;
    else
    valueRight++;
  }
  else // RA = 0
  {
    if(digitalRead(encoderRB) == 1)
    valueRight++;
    else
    valueRight--;
  }
}

/*
  Blink rgb with a state machine strategy
  Push button
  Led rgb
  Motor driving
  Line detectors
*/
#include <Wire.h>

#define LED_RED A1
#define LED_GREEN A2
#define LED_BLUE A3
#define PUSH_BUTTON 4
#define OFF 0
#define RED 1
#define GREEN 2
#define BLUE 3

//Motor A
const int motorPin1 = 6;
const int motorPin2 = 9;

//Motor B
const int motorPin3 = 10;
const int motorPin4 = 11;

boolean RGB_CHANGE = false;
int RGB_STATE = 0;
int last_btn = 0;
int btn = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  setup_serial();
  setup_btn();
  setup_rgb();
  setup_motor();
  setup_qtr();
}

// the loop function runs over and over again forever
void loop() {
  // read sensors
  //read_btn();
  RGB_CHANGE = true;
  // apply state
  if(RGB_CHANGE){
    write_rgb();
  }
  // wait for a second
  delay(3000);
}

// the loop function runs over and over again forever
void read_btn() {
  btn = digitalRead(PUSH_BUTTON);
  Serial.print(last_btn);
  Serial.print("   ");
  Serial.println(btn);
  if(!last_btn && !btn){ // because mode pullup
    RGB_CHANGE = true;
    last_btn = 1;
  }
  else if(last_btn && btn){
    last_btn = 0;
  }
}

// the loop function runs over and over again forever
void write_rgb() {
  switch(RGB_STATE){
    case OFF :
      digitalWrite(LED_RED, HIGH);  // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_BLUE, LOW);  // turn the LED off by making the voltage LOW
      digitalWrite(LED_GREEN, LOW); // turn the LED off by making the voltage LOW
      Serial.println("RED");
      RGB_STATE = RED;
      motors_reverse();
      break;

    case RED :
      digitalWrite(LED_RED, LOW);   // turn the LED off by making the voltage LOW
      digitalWrite(LED_BLUE, LOW);  // turn the LED off by making the voltage LOW
      digitalWrite(LED_GREEN, HIGH);// turn the LED on (HIGH is the voltage level)
      Serial.println("GREEN");
      RGB_STATE = GREEN;
      motors_forward();
      break;
      
    case GREEN :
      digitalWrite(LED_RED, LOW);   // turn the LED off by making the voltage LOW
      digitalWrite(LED_BLUE, HIGH); // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_GREEN, LOW); // turn the LED off by making the voltage LOW
      Serial.println("BLUE");
      RGB_STATE = BLUE;
      motors_rigth();
      break;
      
    default :  
      digitalWrite(LED_RED, LOW);  // turn the LED off by making the voltage LOW
      digitalWrite(LED_BLUE, LOW); // turn the LED off by making the voltage LOW
      digitalWrite(LED_GREEN, LOW);// turn the LED on (HIGH is the voltage level)
      Serial.println("OFF");
      RGB_STATE = OFF;
      motors_stop();
      break;     
  }
  RGB_CHANGE = false;
}

void motors_forward(){
  analogWrite(motorPin1, 250);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 250);
  Serial.println("Motors go forward");
}

void motors_rigth(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 250);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 250);
  Serial.println("Motors turning right");
}

void motors_reverse() {
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 250);
  analogWrite(motorPin3, 250);
  analogWrite(motorPin4, 0);
  Serial.println("Motors go reverse");
}

void motors_stop() {
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
  Serial.println("Motor stoped");
}

void setup_serial() {
  // initialize serial
  Serial.begin(115200);
  Serial.println("Serial com initialized");
}

void setup_rgb() {
  // initialize pins as outputs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  Serial.println("RGB led initialized");
}

void setup_btn() {
  // initialize push btn
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  Serial.println("Push btn initialized");
}

void setup_motor(){
  // initialize motor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  Serial.println("Motors initialized");
}

void setup_qtr(){
  // initialize line detectors
  
  Serial.println("Qtr sensors initialized");
}

#include <Wire.h>

//Motor A
const int motorPin1 = 6;  // Pin 14 of L293
const int motorPin2 = 9;  // Pin 10 of L293

//Motor B
const int motorPin3 = 10; // Pin  7 of L293
const int motorPin4 = 11;  // Pin  2 of L293

void setup() {
  Wire.begin();
  // initialize serial and digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  
  setup_motor();
  Serial.print(" Motor driver set up ...");
}

void loop() {
  // go straigth
  motor_a_forward();
  motor_b_forward();

  delay(5000);

  // go back  
  motor_a_reverse();
  motor_b_reverse();

  delay(5000);

  //turn right
  motor_a_forward();
  motor_b_reverse();

  delay(2000);

  // turn left
  motor_a_reverse();
  motor_b_forward();

  delay(2000);
}

void setup_motor() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
}

void motor_a_forward() {
  analogWrite(motorPin1, 250);
  analogWrite(motorPin2, 0);
}
void motor_b_forward() {
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 250);
}

void motor_a_reverse() {
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 250);
}
void motor_b_reverse() {
  analogWrite(motorPin3, 250);
  analogWrite(motorPin4, 0);
}

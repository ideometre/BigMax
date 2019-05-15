#include <VL53L0X.h>
#include <Wire.h>

VL53L0X tof_front;
VL53L0X tof_back;

// pin numbers input
const int btn_pin = A0;
const int line_right = A6;
const int line_left = A7;
const int coder_right_a = 2;
const int coder_right_b = 7;
const int coder_left_a = 3;
const int coder_left_b = 8;

// pin numbers output
const int rgb_red =  A1;
const int rgb_green =  A3;
const int rgb_blue =  A2;
const int xshut_front = 12;
const int xshut_back = 13;
const int IN1 = 6;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

// states
const int WAIT = 0;
const int SCAN = 1;
const int PUSH = 2;
const int UTURN = 3;
const int CLOCK = 0;
const int COUNTER_CLOCK = 1;
const int FORWARD = 0;
const int BACKWARD = 1;

// variables
int btn_state = 0;
int current_state = WAIT;
int next_state = WAIT;
int sens = CLOCK;
int dir = FORWARD;
int val_line_right = 0;
int val_line_left = 0;
int val_tof_front = 0;
int val_tof_back = 0;
int timeout_tof_front = 0 ;
int timeout_tof_back = 0 ;
long count_on_left = 0;
long count_on_right = 0;
int speed_motor_right = 255;
int speed_motor_left = 255;
int detect_white_right = 400;
int detect_white_left = 400;
int detect_tof_front = 500;
int detect_tof_back = 500;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(10);
  Serial.println("begin init");

  // output
  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_green, OUTPUT);
  pinMode(rgb_blue, OUTPUT);
  pinMode(xshut_front, OUTPUT);
  pinMode(xshut_back, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  delay(10);

  // input
  pinMode(btn_pin, INPUT);
  pinMode(line_right, INPUT);
  pinMode(line_left, INPUT);
  delay(10);

  // tof
  pinMode(xshut_back, INPUT);
  delay(10);

  tof_back.setAddress(42);
  delay(10);

  pinMode(xshut_front, INPUT);
  delay(10);

  tof_front.init();
  tof_back.init();

  tof_front.setTimeout(500);
  tof_back.setTimeout(500);

  tof_front.startContinuous();
  tof_back.startContinuous();

  // interuption
  attachInterrupt(digitalPinToInterrupt(coder_left_a), encoderLeft, CHANGE );
  attachInterrupt(digitalPinToInterrupt(coder_right_a), encoderRight, CHANGE );

  Serial.println("init done");
}

void loop() {
  // read sensors
  btn_state = digitalRead(btn_pin);
  val_line_right = analogRead(line_right);
  val_line_left = analogRead(line_left);
  val_tof_front = tof_front.readRangeContinuousMillimeters();
  if (tof_front.timeoutOccurred()) {
    timeout_tof_front = 1;
  } else {
    timeout_tof_front = 0;
  }
  val_tof_back = tof_back.readRangeContinuousMillimeters();
  if (tof_back.timeoutOccurred()) {
    timeout_tof_back = 1;
  } else {
    timeout_tof_back = 0;
  }

  // setup next_state
  switch (current_state) {
    case SCAN:
      if (btn_state == 1) next_state = WAIT;
      else if (val_line_right < detect_white_right) {
        next_state = UTURN;
        sens = CLOCK;
      }
      else if (val_line_left < detect_white_left ) {
        next_state = UTURN;
        sens = COUNTER_CLOCK;
      }
      else if (val_tof_front < detect_tof_front) {
        next_state = PUSH;
        dir = FORWARD;
      }
      else if (val_tof_back < detect_tof_back) {
        next_state = PUSH;
        dir = BACKWARD;
      }
      else next_state = SCAN;
      break;
    case PUSH:
      if (btn_state == 1) next_state = WAIT;
      else if (val_line_right < detect_white_right) {
        next_state = UTURN;
        sens = CLOCK;
      }
      else if (val_line_left < detect_white_left ) {
        next_state = UTURN;
        sens = COUNTER_CLOCK;
      }
      else if (val_tof_front < detect_tof_front) {
        next_state = PUSH;
        dir = FORWARD;
      }
      else if (val_tof_back < detect_tof_back) {
        next_state = PUSH;
        dir = BACKWARD;
      }
      else next_state = SCAN;
      break;
    case UTURN:
      if (btn_state == 1) next_state = WAIT;
      else if (val_line_right < detect_white_right) {
        next_state = UTURN;
        sens = CLOCK;
      }
      else if (val_line_left < detect_white_left ) {
        next_state = UTURN;
        sens = COUNTER_CLOCK;
      }
      else if (val_tof_front < detect_tof_front) {
        next_state = PUSH;
        dir = FORWARD;
      }
      else if (val_tof_back < detect_tof_back) {
        next_state = PUSH;
        dir = BACKWARD;
      }
      else next_state = SCAN;
      break;
    default://WAIT
      if (btn_state == 1) {
        next_state = SCAN;
        delay(5000);
      }
  }

  // apply next_state
  set_rgb();

  set_motors();

  printStatus();

  current_state = next_state;

  delay(20);
}


void printStatus()
{
  Serial.print("state ");
  Serial.print(current_state);
  Serial.print("\tnext ");
  Serial.print(next_state);
  Serial.print("\tbutton ");
  Serial.print(btn_state);
  Serial.print("\tline right 1 ");
  Serial.print(val_line_right);
  Serial.print("\tline left 2 ");
  Serial.print(val_line_left);
  Serial.print("\ttof front ");
  if (timeout_tof_front) Serial.print("TIMEOUT"); else Serial.print(val_tof_front);
  Serial.print("\ttof back ");
  if (timeout_tof_back) Serial.print("TIMEOUT"); else Serial.print(val_tof_back);
  Serial.print("\tcoder left ");
  Serial.print(count_on_left);
  Serial.print("\tcoder right ");
  Serial.print(count_on_right);
  Serial.println("");
}

void set_rgb()
{
  switch (next_state)
  {
    case SCAN:
      digitalWrite(rgb_red, LOW);
      digitalWrite(rgb_green, LOW);
      digitalWrite(rgb_blue, HIGH);
      break;
    case PUSH:
      digitalWrite(rgb_red, LOW);
      digitalWrite(rgb_green, HIGH);
      digitalWrite(rgb_blue, LOW);
      break;
    case UTURN:
      digitalWrite(rgb_red, HIGH);
      digitalWrite(rgb_green, LOW);
      digitalWrite(rgb_blue, LOW);
      break;
    default://WAIT
      digitalWrite(rgb_red, LOW);
      digitalWrite(rgb_green, LOW);
      digitalWrite(rgb_blue, LOW);
  }
}

void set_motors()
{
  if (next_state != current_state) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
    delay(100);
  }

  switch (next_state)
  {
    case SCAN:
      analogWrite(IN1, 0);
      analogWrite(IN2, speed_motor_right);
      analogWrite(IN3, 0);
      analogWrite(IN4, speed_motor_left);
      break;
    case PUSH:
      if (dir == FORWARD) {
        analogWrite(IN1, 0);
        analogWrite(IN2, speed_motor_right);
        analogWrite(IN3, speed_motor_left);
        analogWrite(IN4, 0);
      } else {
        analogWrite(IN1, speed_motor_right);
        analogWrite(IN2, 0);
        analogWrite(IN3, 0);
        analogWrite(IN4, speed_motor_left);
      }
      break;
    case UTURN:
      if (sens == CLOCK)
      {
        analogWrite(IN1, 0);
        analogWrite(IN2, speed_motor_right);
        analogWrite(IN3, 0);
        analogWrite(IN4, speed_motor_left);
      }
      else
      {
        analogWrite(IN1, speed_motor_right);
        analogWrite(IN2, 0);
        analogWrite(IN3, speed_motor_left);
        analogWrite(IN4, 0);
      }
      delay(750);
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
      delay(100);
      analogWrite(IN1, 0);
      analogWrite(IN2, speed_motor_right);
      analogWrite(IN3, speed_motor_left);
      analogWrite(IN4, 0);
      delay(750);
      break;
    default://WAIT
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
  }
}

// count encoder
void encoderLeft()
{
  if (digitalRead(coder_left_a) == 1)
  {
    if (digitalRead(coder_left_b) == 1) count_on_left--; else count_on_left++;
  }
  else // coder_left_a == 0
  {
    if (digitalRead(coder_left_b) == 1) count_on_left++; else count_on_left--;
  }
}

// count encoder
void encoderRight()
{
  if (digitalRead(coder_right_a) == 1)
  {
    if (digitalRead(coder_right_b) == 1) count_on_right--; else count_on_right++;
  }
  else // coder_right_a == 0
  {
    if (digitalRead(coder_right_b) == 1) count_on_right++; else count_on_right--;
  }
}

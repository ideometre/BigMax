#include <Wire.h>
#include <VL53L0X.h>

#define xshut_right 12
#define xshut_left 13

VL53L0X tof_right;
VL53L0X tof_left;

// pin numbers input
const int btn_pin = A0;
const int micro_start = 4;
const int line_right = A6;
const int line_left = A7;
const int coder_right_a = 2;
const int coder_right_b = 7;
const int coder_left_a = 3;
const int coder_left_b = 8;

// pin numbers output
const int rgb_red =  A1;
const int rgb_green =  A2;
const int rgb_blue =  A3;
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
int micro_start_prev = 0;
int micro_start_state = 0;
int current_state = WAIT;
int next_state = WAIT;
int sens = CLOCK;
int dir = FORWARD;
int val_line_right = 0;
int val_line_left = 0;
int val_tof_right = 0;
int val_tof_left = 0;
int timeout_tof_right = 0 ;
int timeout_tof_left = 0 ;
long count_on_left = 0;
long count_on_right = 0;
int speed_motor_slow = 150;
int speed_motor_high = 250;
int detect_white_right = 200;
int detect_white_left = 200;
int detect_tof_right = 500;
int detect_tof_left = 500;

void setup() {
//  Serial.begin(115200);
  Wire.begin();
//  Serial.println("begin init");

  // output
  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_green, OUTPUT);
  pinMode(rgb_blue, OUTPUT);
  pinMode(xshut_left, OUTPUT);
  pinMode(xshut_right, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // input
  pinMode(btn_pin, INPUT);
  pinMode(micro_start, INPUT);
  pinMode(line_right, INPUT);
  pinMode(line_left, INPUT);

  // interuption
  attachInterrupt(digitalPinToInterrupt(coder_left_a), encoderLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(coder_right_a), encoderRight, CHANGE);

  // tofs
  pinMode(xshut_right, INPUT);
  delay(30);
  tof_right.setAddress((uint8_t)25);
  tof_right.init();
  tof_right.setTimeout(500);

  pinMode(xshut_left, INPUT);
  delay(30);
  tof_left.setAddress((uint8_t)22);
  tof_left.init();
  tof_left.setTimeout(500);

  tof_right.startContinuous();
  tof_left.startContinuous();

//  Serial.println("init done");
}

void loop() {
  // read sensors
  btn_state = digitalRead(btn_pin);
  micro_start_state = digitalRead(micro_start);

  val_line_right = analogRead(line_right);
  val_line_left = analogRead(line_left);

  val_tof_right = tof_right.readRangeContinuousMillimeters();
  if (tof_right.timeoutOccurred()) timeout_tof_right = 1;
  else timeout_tof_right = 0;

  val_tof_left = tof_left.readRangeContinuousMillimeters();
  if (tof_left.timeoutOccurred()) timeout_tof_left = 1;
  else timeout_tof_left = 0;

  // setup next_state
  switch (current_state) {
    case WAIT:
      if (micro_start_prev == 0 && micro_start_state == 1) {
        delay(5000);
        next_state = SCAN;
        micro_start_prev = micro_start_state;
      }
      break;

    default:
      if (micro_start_prev == 1 && micro_start_state == 0) {
        next_state = WAIT;
        micro_start_prev = micro_start_state;
      }
      else if (val_line_right < detect_white_right) {
        next_state = UTURN;
        sens = COUNTER_CLOCK;
      }
      else if (val_line_left < detect_white_left) {
        next_state = UTURN;
        sens = CLOCK;
      }
      else if (val_tof_left < detect_tof_left && val_tof_right < detect_tof_right) {
        next_state = PUSH;
        dir = FORWARD;
      }
      else if (val_tof_right < detect_tof_right && val_tof_left > detect_tof_left) {
        next_state = SCAN;
        sens = CLOCK;
      }
      else if (val_tof_left < detect_tof_left && val_tof_right > detect_tof_right) {
        next_state = SCAN;
        sens = COUNTER_CLOCK;
      }
      else next_state = SCAN;
  }

  // apply next_state
  set_motors();
  set_rgb();
//  printStatus();
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
  Serial.print("\tmicro_start ");
  Serial.print(micro_start_state);
  Serial.print("\tline right ");
  Serial.print(val_line_right);
  Serial.print("\tline left ");
  Serial.print(val_line_left);
  Serial.print("\ttof right ");
  if (timeout_tof_right) Serial.print("TIMEOUT");
  else Serial.print(val_tof_right);
  Serial.print("\ttof left ");
  if (timeout_tof_left) Serial.print("TIMEOUT");
  else Serial.print(val_tof_left);
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
//  if (next_state != current_state) {
//    analogWrite(IN1, 0);
//    analogWrite(IN2, 0);
//    analogWrite(IN3, 0);
//    analogWrite(IN4, 0);
//    delay(20);
//  }

  switch (next_state)
  {
    case SCAN:
      if (sens == CLOCK)
      {
        analogWrite(IN1, 0);
        analogWrite(IN2, 0);
        analogWrite(IN3, speed_motor_slow);
        analogWrite(IN4, 0);
      } else {
        analogWrite(IN1, 0);
        analogWrite(IN2, speed_motor_slow);
        analogWrite(IN3, 0);
        analogWrite(IN4, 0);
      }
      break;

    case PUSH:
      if (dir == FORWARD) {
        analogWrite(IN1, 0);                // LEFT
        analogWrite(IN2, speed_motor_slow); //
        analogWrite(IN3, speed_motor_slow); // RIGHT
        analogWrite(IN4, 0);                //
      } else {
        analogWrite(IN1, speed_motor_slow);
        analogWrite(IN2, 0);
        analogWrite(IN3, 0);
        analogWrite(IN4, speed_motor_slow);
      }
      break;

    case UTURN:
      if (sens == CLOCK)
      {
        analogWrite(IN1, speed_motor_slow);
        analogWrite(IN2, 0);
        analogWrite(IN3, speed_motor_slow);
        analogWrite(IN4, 0);
      }
      else
      {
        analogWrite(IN1, 0);
        analogWrite(IN2, speed_motor_slow);
        analogWrite(IN3, 0);
        analogWrite(IN4, speed_motor_slow);
      }
      delay(400);
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
      delay(20);
      analogWrite(IN1, 0);
      analogWrite(IN2, speed_motor_high);
      analogWrite(IN3, speed_motor_high);
      analogWrite(IN4, 0);
      delay(300);
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
    if (digitalRead(coder_left_b) == 1) count_on_left++; else count_on_left--;
  }
  else // coder_left_a == 0
  {
    if (digitalRead(coder_left_b) == 1) count_on_left--; else count_on_left++;
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

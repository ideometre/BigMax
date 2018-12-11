#include <Wire.h>
#include <VL53L0X.h>
#include <QTRSensors.h>

#define NUM_SENSORS   2     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {
  A6, A7
},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

VL53L0X sensor;

//Motor A
const int motorPin1 = 6;  // Pin 14 of L293
const int motorPin2 = 9;  // Pin 10 of L293

//Motor B
const int motorPin3 = 10; // Pin  7 of L293
const int motorPin4 = 11;  // Pin  2 of L293

void setup() {
  // initialize serial and digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  setup_VL53L0X();
  setup_QTRRC();
  setup_motor();
}

void loop() {
  loop_VL53L0X();
  loop_QTRRC();
  loop_motor();

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

void setup_VL53L0X()
{
  Wire.begin();
  sensor.init();
  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}

void setup_QTRRC()
{
  digitalWrite(LED_BUILTIN, HIGH);  // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)     // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();              // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(LED_BUILTIN, LOW);   // turn off Arduino's LED to indicate we are through with calibration

  for (int i = 0; i < NUM_SENSORS; i++)
  { // print the calibration minimum values measured when emitters were on
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  for (int i = 0; i < NUM_SENSORS; i++)
  { // print the calibration maximum values measured when emitters were on
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
}

void setup_motor() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
}

void loop_QTRRC()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //       qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtrrc.readLine(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position); // comment this line out if you are using raw values
}

void loop_VL53L0X()
{
  Serial.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred())
  {
    Serial.print(" TIMEOUT");
  }
  Serial.println();
}

void loop_motor() {
  //Motor Control - Motor A: motorPin1,motorpin2 & Motor B: motorpin3,motorpin4

  //This code will turn Motor A clockwise for 2 sec.
  analogWrite(motorPin1, 180);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 180);
  analogWrite(motorPin4, 0);
  delay(5000);
  //This code will turn Motor A counter-clockwise for 2 sec.
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 180);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 180);
  delay(5000);
  //This code will turn Motor B clockwise for 2 sec.
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 180);
  analogWrite(motorPin3, 180);
  analogWrite(motorPin4, 0);
  delay(1000);
  //This code will turn Motor B counter-clockwise for 2 sec.
  analogWrite(motorPin1, 180);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 180);
  delay(1000);
  //And this code will stop motors
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}

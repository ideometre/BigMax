/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
VL53L0X sensor2;

void setup()
{
  setup_vl53l0x();
}

void setup_vl53l0x()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  delay(150);  
  Wire.begin();
  Serial.begin (9600);

  pinMode(12, INPUT);
  delay(150);
  Serial.println("00");
  sensor.init(true);

  Serial.println("01");
  delay(100);
  sensor.setAddress((uint8_t)22);
  Serial.println("02");

  pinMode(13, INPUT);
    delay(150);
  sensor2.init(true);
  Serial.println("03");
  delay(100);
  sensor2.setAddress((uint8_t)25);
  Serial.println("04");

  Serial.println("addresses set");

  sensor.setTimeout(500);
  sensor.startContinuous();

  sensor2.setTimeout(500);
  sensor2.startContinuous();

}

void loop()
{
  Serial.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.print("   ");
  
  Serial.print(sensor2.readRangeContinuousMillimeters());
  if (sensor2.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  
  Serial.println();
  delay(200);
}

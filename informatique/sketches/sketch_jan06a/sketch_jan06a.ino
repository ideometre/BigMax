#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor1;
VL53L0X sensor2;

void setup()
{
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  delay(500);
  Serial.begin (9600);
  Wire.begin();
  
  digitalWrite(4, HIGH);
  delay(150);
  Serial.println("00");
  sensor1.init(true);
  Serial.println("01");
  delay(100);
  sensor1.setAddress((uint8_t)22);
  Serial.println("sensor1 addresse set");

  digitalWrite(5, HIGH);
  delay(150);
  Serial.println("02");
  sensor2.init(true);
  Serial.println("03");
  delay(100);
  sensor2.setAddress((uint8_t)25);
  
  Serial.println("sensor2 addresse set");
}

void loop()
{
  Serial.println ("I2C scanner. Scanning ...");
  
  byte count = 0;

  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
    } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  delay(3000);
}

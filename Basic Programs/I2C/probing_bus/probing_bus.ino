/********************************************************
Finding slave devices on I2C bus
*********************************************************/
#include <Wire.h>

#define BAUD_RATE 115200

void setup()
{
  Serial.begin(BAUD_RATE);
  Wire.begin();
}


void loop()
{
  Serial.println("Starting to probe...");

  uint8_t result;

  for(int addr = 0; addr <=255; addr++)
  {
    Wire.beginTransmission(addr);
    result = Wire.endTransmission();
    if (result == 0)
    {
      Serial.print("Found device with address: 0x");
      Serial.print(addr, HEX);
      Serial.print(" = 0b");
      Serial.print(addr, BIN);
      Serial.println();
    }
    else
    {
//      Serial.println("...");
    }
//    delay(100);
  }
  Serial.print("finished probing.");
  while(1);
}

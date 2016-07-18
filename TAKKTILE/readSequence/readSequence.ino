#include <Wire.h>

// 0x00
// 0x01

#define SLAVE_ADDRESS 0x0C>>1//0xC0>>1//0x0D>>1//0x0C>>1


// Sequence: 0x0C (0xC0 0x12 0x01) 0x0D 0x00 (0xC0 0x00) (0xC1 R R R R) 0x01
int slaveAddr[7] = {0x0C>>1, 0xC0>>1, 0x0D>>1, 0x00>>1, 0xC0>>1, 0xC1>>1, 0x01>>1};
//int slaveAddr = SLAVE_ADDRESS;
int count = 0;

void setup() {
  Wire.begin(slaveAddr[count]);
  Wire.onReceive(receiveCallback);
  Wire.onRequest(requestCallback);
  Serial.begin(230400);
  Serial.print("Starting: 0x");
  Serial.println(slaveAddr[count], HEX);
}



void loop() {

}

void receiveCallback(int howMany)
{
  Serial.print("0x");
  Serial.println(slaveAddr[count]<<1, HEX);
//  Serial.println(" Rec: ");
  uint8_t c;
  count = (count + 1) % 8;
  Wire.begin(slaveAddr[count]);
  
  while (Wire.available())
  {
    c = Wire.read(); // receive byte as a character
  }
}

void requestCallback()
{
  Serial.print("0x");
  Serial.println((slaveAddr[count]<<1) + 1, HEX);
//  Serial.println(" R: ");
  if (slaveAddr[count] == 0xC1>>1)
  {
    Wire.write(0x01);
    Wire.write(0x02);
    Wire.write(0x03);
    Wire.write(0x04);
  }
  count = (count + 1) % 8;
  Wire.begin(slaveAddr[count]);
}


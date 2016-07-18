#include <Wire.h>

// 0x00
// 0x01

#define SLAVE_ADDRESS 0x0C>>1//0x0D>>1//0x0C>>1

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveCallback);
  Wire.onRequest(requestCallback);
  Serial.begin(115200);
}

void loop() {

}

void receiveCallback(int howMany)
{
//  Serial.print("Number of bytes available to read: ");
  Serial.print("Rec: ");
  Serial.print(Wire.available());
  uint8_t c;
  while (Wire.available())
  {
    c = Wire.read(); // receive byte as a character
    Serial.print(" 0x"); Serial.print(c, HEX);
  }
  Serial.println();
}

void requestCallback()
{
  Serial.println("R");
//  Serial.println(Wire.available());
//  uint8_t c;
//  while (Wire.available())
//  {
//    c = Wire.read(); // receive byte as a character
//    Serial.print(c);
//  }
//  Serial.println();
}


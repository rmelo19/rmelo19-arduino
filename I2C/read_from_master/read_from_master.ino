#include <Wire.h>
  
#define SLAVE_ADDRESS 0x08

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveCallback);
  Serial.begin(115200);
}

void loop() {

}

void receiveCallback(int howMany)
{
  Serial.print("Number of bytes available to read: ");
  Serial.println(Wire.available());
  uint8_t c;
  while (Wire.available())
  {
    c = Wire.read(); // receive byte as a character
    Serial.print((char)c);
  }
  Serial.println();
}


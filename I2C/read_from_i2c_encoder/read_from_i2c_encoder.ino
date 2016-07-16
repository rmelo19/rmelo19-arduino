#include <Wire.h>
  
#define SLAVE_ADDRESS_READ 0x40
#define SLAVE_ADDRESS_WRITE 0x41
#define SLAVE_ADDRESS_7_BIT 0x20

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop()
{
  int bytesWritten;
  int bytesReturned;
  int transmissionStatus;
  uint8_t data;
  bool dataAvailable = false;
  int slave_address = 0;
  while(!dataAvailable && slave_address != 255)
  {
    Serial.print("Slave address: ");
    Serial.println(slave_address);
    Wire.beginTransmission(slave_address);
    bytesWritten = Wire.write((uint8_t)0xFA);
    transmissionStatus = Wire.endTransmission(false);
    Serial.print("Transmission Status: ");
    Serial.println(transmissionStatus);
    Serial.print("Bytes written: ");
    Serial.println(bytesWritten);
    bytesReturned = Wire.requestFrom(slave_address, 1);
    Serial.print("Bytes returned: ");
    Serial.print(bytesReturned);
    bytesReturned = Wire.available();
    Serial.print(" ");
    Serial.println(bytesReturned);
    
//    Serial.print("Bytes available: ");
    Serial.println();
    dataAvailable = bytesReturned != 0;
    slave_address++;
    delay(10);
  }
  if (slave_address == 0b10000000)
  {
    Serial.println("Didn't find any slave in the bus.");
  }
  else
  {
    for (int i=0;i<bytesReturned;i++)
    {
      data = Wire.read();
      Serial.print("    Data: ");
      Serial.println(data);
      delay(0.5);
    }
  }
  while(1);
  
}
//
//void receiveCallback(int howMany)
//{
//  Serial.print("Number of bytes available to read: ");
//  Serial.println(Wire.available());
//  uint8_t c;
//  while (Wire.available())
//  {
//    c = Wire.read(); // receive byte as a character
//    Serial.print((char)c);
//  }
//  Serial.println();
//}


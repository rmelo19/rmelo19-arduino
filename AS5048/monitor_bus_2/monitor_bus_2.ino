#include <Wire.h>

// Slave address;
#define SLAVE_ADRR 0x20
// 0b0000110, BCAST_ENABLE
// 0x60, BAROMETER

int count = 0;

void receivedByte(int num_bytes);
void receivedRequest(void);

void setup()
{
  Serial.begin(115200);
  Serial.print("-> 0x");
  Serial.println(SLAVE_ADRR, HEX);
  Wire.begin(SLAVE_ADRR);
  Wire.onReceive(receivedByte);
  Wire.onRequest(receivedRequest);
  Serial.println("Monitoring bus.");
  delay(200);
  
}

void loop()
{
}

void receivedByte(int num_bytes)
{
//  Serial.print("Number of bytes received: ");
  Serial.print(num_bytes);
  if (num_bytes > 0)
  {
    while(Wire.available())
    {
      Serial.print(" 0x");
      Serial.print(Wire.read(), HEX);
    Wire.read();
    }
    Serial.println();
  }
  else
  {
    Serial.println();
  }
}

void receivedRequest(void)
{
//  Serial.println("R"); // Serial.println("Received request...");
  Wire.write(0x00);
  Wire.write(0x00);
}


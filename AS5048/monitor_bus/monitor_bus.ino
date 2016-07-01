#include <Wire.h>

// Slave address;
#define SLAVE_ADRR 0x20
// 0b0000110, BCAST_ENABLE
// 0x60, BAROMETER

int count = 0;

void receivedByte(int num_bytes);
void receivedByte2(int num_bytes);
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
  delay(2);
  
}

void loop()
{
//  while(1);  
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
    }
    Serial.println();
  }
  else
  {
    Serial.println();
  }
 
//  int n = Wire.available();
//  Serial.print("Number: ");
//  Serial.println(n);
  if (count == 0)
  {
//    Serial.println("Changed slave address to 0x60");
   
    Wire.begin(0x60);
    Serial.print("-> 0x");
    Serial.println(0x60, HEX);

//    Wire.onReceive(receivedByte2);
    count++;
  }
  else if (count == 1)
  {
    Wire.begin(SLAVE_ADRR);
    Serial.print("-> 0x");
    Serial.println(SLAVE_ADRR, HEX);
    count++;
  }
  else if (count == 2)
  {
    Wire.begin(0x60);
    Serial.print("-> 0x");
    Serial.println(0x60, HEX);
    count++;
  }
  else if (count == 3)
  {
    Wire.begin(0x60);
    Serial.print("-> 0x");
    Serial.println(0x60, HEX);
    count++;
  }
}
//
//void receivedByte2(int num_bytes)
//{
////  Serial.print("Number of bytes received2: ");
//  Serial.println(num_bytes);
//  while(Wire.available())
//  {
//    Wire.read();
//  }
////  int n = Wire.available();
////  Serial.print("Number: ");
////  Serial.println(n);
//  
//}


void receivedRequest(void)
{
  Serial.println("R"); // Serial.println("Received request...");
  Wire.begin(0x02);
}


/********************************************************
Basic Communicatio:
 read from sensor AS5048B
*********************************************************/
#include <Wire.h>

#define BAUD_RATE 115200
#define AS5048B_ANGLLSB_REG 0xFF //bits 0..5
#define AS5048B_RESOLUTION 16384.0 //14 bits

void setup()
{
  Serial.begin(BAUD_RATE);
  Wire.begin();
}

void setRegister(int, int);

void loop()
{
  Serial.println("Starting to communicate...");
  uint8_t addr = 0xC0;
//  setRegister(addr, AS5048B_ANGLLSB_REG);
//  addr = 0xC0;
  setRegister(addr, AS5048B_ANGLLSB_REG);

  int result;
  while(1)
  {
    result = Wire.requestFrom(addr, 2);
    if (result == 0)
    {
      Serial.println("Device not responding...");
      break;
    }
    
    uint16_t readValue;
    readValue = (((uint16_t) Wire.read()) << 6) ;
    readValue += (uint16_t) (Wire.read() & 0x3F);
  
    double angle;
    angle = (double) ((readValue * 360.0) / AS5048B_RESOLUTION);
  
    Serial.print("Angle: ");
    Serial.println(angle);
    setRegister(addr, AS5048B_ANGLLSB_REG);
    delay(100);

    
  }
  
}


void setRegister(int dev_addr, int reg_addr)
{
  int result;
  Serial.print("Transmitting to address: ");
  Serial.println(dev_addr, HEX);
  Wire.beginTransmission(dev_addr);
  result = Wire.write(reg_addr);
  if (result == 0)
  {
    Serial.println("Failed to read byte");
  }
  result = Wire.endTransmission();
  if (result == 0)
  {
    Serial.println("Success, register set");
  }
}




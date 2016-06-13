byte inByte;

bool flagShowAddress = 0;
bool flagShowPressure = 0;
bool flagShowTemperature = 0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (Serial.available())
  {
    inByte = (byte)
    Serial.read();
    if (inByte=='n')
    {
      flagShowAddress = !flagShowAddress;
    }
    if (inByte=='p')
    {
      flagShowPressure = !flagShowPressure;
    }
    if (inByte=='t')
    {
      flagShowTemperature = !flagShowTemperature;
    } 
  }
}

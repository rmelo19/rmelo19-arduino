String buffer;

void setup()
{ 
  Serial.begin(115200);
}
void loop()
{
  if (Serial.available() > 0)
  {
    buffer = Serial.readString();
    Serial.print(buffer);
  }
}


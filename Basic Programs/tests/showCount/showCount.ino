int count = 0;

void setup()
{
Serial.begin(115200);
}

void loop()
{
  Serial.print("Count: ");
  Serial.println(count);  
  Serial.read();
  count++;
  if (count == 100)
  {
    count = 0;
  }
  delay(1000);
}

String buffer;
bool readSuccess;

void setup()
{
  Serial.begin(115200);
}
void loop()
{
  Serial.println("Waiting next message...!");
  buffer = Serial.readString();
  readSuccess = buffer.length() > 0;
  while (Serial.available() <= 0 && !readSuccess)
  {
    buffer = Serial.readString();
    readSuccess = buffer.length() > 0;
  }
  // say what you got:
  buffer.remove(buffer.indexOf('\n'));
  Serial.print(buffer.length());
  Serial.print(" bytes received: ");
  Serial.println(buffer);
  delay(1000);  
}


String buffer;

void setup()
{ 
  Serial.begin(115200);
}
void loop()
{
  Serial.println("Waiting next message...!");
  while(Serial.available() <= 0)
  {
  }
  buffer = Serial.readString();
  buffer.remove(buffer.indexOf('\n'));
  Serial.print(buffer.length());
  Serial.print(" bytes received: ");
  Serial.println(buffer);
  Serial.flush();
}


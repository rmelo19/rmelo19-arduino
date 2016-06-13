void setup()
{
  Serial.begin(115200);
}
void loop()
{
  String mystring = "mystring"; // define String object
  mystring.remove('m'); // remove one of the letters
  Serial.println(mystring); // print in the serial port
}

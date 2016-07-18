const int analogOutPin = 9;

int sensorValue = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(19200);
//  pinMode(9, INPUT);
}

void loop() {
  int MAX_SENSOR_VALUE = 10;
  
  
  sensorValue++;
  if (sensorValue == MAX_SENSOR_VALUE)
    sensorValue = 0;
  
  analogWrite(analogOutPin, MAX_SENSOR_VALUE);

  Serial.print("sensor = ");
  Serial.print(sensorValue);

  delay(100);
}

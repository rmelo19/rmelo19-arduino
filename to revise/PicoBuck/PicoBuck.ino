const int analogOutPin = 9;

int sensorValue = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(19200);
}

void loop() {
  int MAX_SENSOR_VALUE = 30;
  
  sensorValue++;
  if (sensorValue == MAX_SENSOR_VALUE)
    sensorValue = 0;
  
  analogWrite(9, MAX_SENSOR_VALUE);
  analogWrite(10, MAX_SENSOR_VALUE);
  analogWrite(11, MAX_SENSOR_VALUE);


  Serial.print("sensor = ");
  Serial.print(sensorValue);

  delay(100);
}

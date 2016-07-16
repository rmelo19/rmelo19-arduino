int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

#include <Servo.h>

Servo myservo; 

void setup() {
  pinMode(ledPin, OUTPUT);
  myservo.attach(2);
  Serial.begin(9600);
}

void loop() {
  myservo.writeMicroseconds(1300);
  delay(500);
  myservo.writeMicroseconds(1500);
  while(1)
    {
    sensorValue = analogRead(sensorPin);
    while(sensorValue < 500)
    {
       sensorValue = analogRead(sensorPin);
       Serial.print("Sensor value < 500:");
       Serial.println(sensorValue);
       delay(100);
    }
    while(sensorValue > 500)
    {
       sensorValue = analogRead(sensorPin);
       Serial.print("    Sensor value > 500:");
       Serial.println(sensorValue);
       delay(100);
    }
    while(1)
    {
      myservo.writeMicroseconds(1700);
      delay(500);
      myservo.writeMicroseconds(1500);
      //delay(100);// waits for the servo to get there 
      while(1)
      {
      }
    }
    }
}

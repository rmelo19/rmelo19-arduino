/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground

 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.


 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/AnalogInput

 */

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

#include <Servo.h>

Servo myservo; 

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  myservo.attach(2);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  //  int count;
  //count = 0;
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
    //count = count + 1;
    //Serial.print("COUNT! COUNT! COUNT! COUNT! ");
    //Serial.println(count);
    
    // turn the ledPin on
    //digitalWrite(ledPin, HIGH);
    // stop the program for <sensorValue> milliseconds:
    //delay(sensorValue);
    // turn the ledPin off:
    //digitalWrite(ledPin, LOW);
    // stop the program for for <sensorValue> milliseconds:
    //delay(100);
    }
}

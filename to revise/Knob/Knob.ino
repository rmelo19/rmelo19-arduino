/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup()
{
  //myservo.writeMicroseconds(500); //set initial servo position if desired
  myservo.attach(2);//, 500, 2500);  //the pin for the servo control, and range if desired
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  //Serial.begin(9600);
}

void loop() 
{ 
  //val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  //Serial.println(val);
  //val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180) 
  //Serial.print("writing Microseconds: ");
  //Serial.println(500);
  myservo.writeMicroseconds(1300);
  delay(500);
  myservo.writeMicroseconds(1500);
  delay(5000);
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

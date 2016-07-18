#include <SoftwareServo.h> 

SoftwareServo myservo;  // create servo object to control a servo 

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 

void setup() 
{ 
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(A2, OUTPUT);
  myservo.attach(A0);  // attaches the servo on pin 2 to the servo object
  Serial.begin(9600);
  analogWrite(9, 0);
  analogWrite(10, 0);
  digitalWrite(A2, LOW);
} 

#define SERVO_REFRESH 50

void loop() 
{ 
  for (int i = 1; i < 5; i++)
  {
  Serial.println("Alive 0");
  myservo.write(0);                  // sets the servo position according to the scaled value 
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
//  delay(SERVO_REFRESH);                           // waits for the servo to get there 
//  SoftwareServo::refresh();
//  delay(SERVO_REFRESH);                           // waits for the servo to get there 
//  SoftwareServo::refresh();
  Serial.println("Alive 90");
  myservo.write(90);                  // sets the servo position according to the scaled value 
  delay(1000);
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
  delay(SERVO_REFRESH);                           // waits for the servo to get there 
  SoftwareServo::refresh();
//  SoftwareServo::refresh();
//  Serial.println("Alive 1");
//  myservo.write(179);                  // sets the servo position according to the scaled value 
//  delay(SERVO_REFRESH);    
//  SoftwareServo::refresh();
//  Serial.println("Alive 1");
//  myservo.write(90);                  // sets the servo position according to the scaled value 
//  delay(SERVO_REFRESH);    
//  SoftwareServo::refresh();
//  delay(1000);
}
  while(1)
  {
    analogWrite(9, 20);
    digitalWrite(A2, HIGH);
  }
} 

/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */
    

#include <IRremote.h>
IRsend irsend;

int redpin = 11; //select the pin for the red LED
int bluepin =10; // select the pin for the  blue LED
int greenpin =9;// select the pin for the green LED
int val;

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(redpin, OUTPUT); pinMode(bluepin, OUTPUT); pinMode(greenpin, OUTPUT); Serial.begin(9600);
}



void loop() {
  for (int i = 1; i<=3; i++){
    irsend.sendSony(0xa92, 12);
    delay(40); 
  }
  for(val=255; val>0; val--)
  {
  analogWrite(11, val); analogWrite(10, 255-val); analogWrite(9, 128-val); delay(1);
  }
  for(val=0; val<255; val++)
  {
  analogWrite(11, val); analogWrite(10, 255-val); analogWrite(9, 128-val); delay(1);
  }
  Serial.println(val, DEC);
  delay(1000);
}

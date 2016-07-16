#define NUM 11
#define FIRST 2

#include <EEPROM.h>
#define NUM_PROGRAMS 2

int n=FIRST;
int i=0;
int del=45;
int delta = 10;
int program;

void setup()
{
  program =  EEPROM.read(0);
  if (++program >= NUM_PROGRAMS) program = 0;
  EEPROM.write(0, program);
  
  for(i=2; i<=13; i++)
  {
    pinMode(i, OUTPUT);
  }
  
  randomSeed(analogRead(0));
}

void loop0()
{
  del = del-delta;
  if (del < 10 || del > 45) 
    delta = delta * -1;
  
  int n;
  n = random(12) + 2;
  digitalWrite(n, HIGH);
  delay(del); 
  digitalWrite(n, LOW);
}

void loop1() {
  static int pin = 2;
  del += delta;
  if (del > 100) {
    delta = -2;
    del = 100;
  } else if (del < 30) {
    delta = 2;
    del = 30;
  }
  if (++pin > 13) pin = 2;
  digitalWrite(pin, HIGH);
  delay(del);
  digitalWrite(pin, LOW);
}


void loop() {
 if (program == 0) loop0();
 else loop1();
}

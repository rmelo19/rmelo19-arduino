#include "LedControl.h"
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT

// Some useful functions:
//  lc.setLed(0,2,7,false);

LedControl lc=LedControl(12,10,11,1); // (PINS 12, 10, 11) <-> (PINS DIN< CLK, /CS)

uint16_t rol(uint16_t val);
void printSerialNumber(uint16_t number*);
void printSerialNumber(uint8_t number*);

int num1[] = {0b00011000,
              0b00111000,
              0b01111000,
              0b00011000,
              0b00011000,
              0b00011000,
              0b00111100,               
              0b01111110};
int num2[] = {0b00111110,
             0b01100011,
             0b01000011,
             0b00000110,
             0b00001100,
             0b00011000,
             0b00111111,               
             0b01111111};

void printNumber_2(LedControl lc, uint8_t *num)
{
    lc.setRow(0, 0, num[0]);
    lc.setRow(0, 1, num[1]);
    lc.setRow(0, 2, num[2]);
    lc.setRow(0, 3, num[3]);
    lc.setRow(0, 4, num[4]);
    lc.setRow(0, 5, num[5]);
    lc.setRow(0, 6, num[6]);
    lc.setRow(0, 7, num[7]); 
}

void printNumber(LedControl lc, uint8_t number)
{
  int num[] = {0b00011000,
               0b00111000,
               0b01111000,
               0b00011000,
               0b00011000,
               0b00011000,
               0b00111100,               
               0b01111110};
               
  if (number == '1')
  {
//    lc.setRow(0, 0, 0b00011000);
//    lc.setRow(0, 1, 0b00111000);
//    lc.setRow(0, 2, 0b01111000);
//    lc.setRow(0, 3, 0b00011000);
//    lc.setRow(0, 4, 0b00011000);
//    lc.setRow(0, 5, 0b00011000);
//    lc.setRow(0, 6, 0b00111100);
//    lc.setRow(0, 7, 0b01111110);
    
    lc.setRow(0, 0, num[0]);
    lc.setRow(0, 1, num[1]);
    lc.setRow(0, 2, num[2]);
    lc.setRow(0, 3, num[3]);
    lc.setRow(0, 4, num[4]);
    lc.setRow(0, 5, num[5]);
    lc.setRow(0, 6, num[6]);
    lc.setRow(0, 7, num[7]); 
  }
  else if (number == '2')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b01000011);
    lc.setRow(0, 3, 0b00000110);
    lc.setRow(0, 4, 0b00001100);
    lc.setRow(0, 5, 0b00011000);
    lc.setRow(0, 6, 0b00111111);
    lc.setRow(0, 7, 0b01111111);
  }
  else if (number == '3')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b00000111);
    lc.setRow(0, 3, 0b00001100);
    lc.setRow(0, 4, 0b00000110);
    lc.setRow(0, 5, 0b00000011);
    lc.setRow(0, 6, 0b01100011);
    lc.setRow(0, 7, 0b00111111);
  }
  else if (number == '4')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b01000011);
    lc.setRow(0, 3, 0b00000110);
    lc.setRow(0, 4, 0b00001100);
    lc.setRow(0, 5, 0b00011000);
    lc.setRow(0, 6, 0b00111111);
    lc.setRow(0, 7, 0b01111111);
  }
  else if (number == '5')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b01000011);
    lc.setRow(0, 3, 0b00000110);
    lc.setRow(0, 4, 0b00001100);
    lc.setRow(0, 5, 0b00011000);
    lc.setRow(0, 6, 0b00111111);
    lc.setRow(0, 7, 0b01111111);
  }
  else if (number == '6')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b01000011);
    lc.setRow(0, 3, 0b00000110);
    lc.setRow(0, 4, 0b00001100);
    lc.setRow(0, 5, 0b00011000);
    lc.setRow(0, 6, 0b00111111);
    lc.setRow(0, 7, 0b01111111);
  }
  else if (number == '7')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b01000011);
    lc.setRow(0, 3, 0b00000110);
    lc.setRow(0, 4, 0b00001100);
    lc.setRow(0, 5, 0b00011000);
    lc.setRow(0, 6, 0b00111111);
    lc.setRow(0, 7, 0b01111111);
  }
  else if (number == '8')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b01000011);
    lc.setRow(0, 3, 0b00000110);
    lc.setRow(0, 4, 0b00001100);
    lc.setRow(0, 5, 0b00011000);
    lc.setRow(0, 6, 0b00111111);
    lc.setRow(0, 7, 0b01111111);
  }
  else if (number == '9')
  {
    lc.setRow(0, 0, 0b00111110);
    lc.setRow(0, 1, 0b01100011);
    lc.setRow(0, 2, 0b01000011);
    lc.setRow(0, 3, 0b00000110);
    lc.setRow(0, 4, 0b00001100);
    lc.setRow(0, 5, 0b00011000);
    lc.setRow(0, 6, 0b00111111);
    lc.setRow(0, 7, 0b01111111);
  }
}

uint8_t count = 0;
uint8_t dis_num[8];
uint8_t dis_num1[8];
uint8_t dis_num2[8];
uint16_t mynum[8];

void setup()
{
    //wake up the MAX72XX from power-saving mode 
    lc.shutdown(0,false);
    //set a medium brightness for the Leds
    lc.setIntensity(0,8);
    
    for (int i=0;i<8;i++)
    {
        mynum[i] = num1[i]*256 + num2[i];
    }
    Serial.begin(250000);
    Serial.println("Initialized");
    Serial.println("Number to display, mynum: ");
    for (int i=0;i<8;i++)
    {
        Serial.print("    ");
        Serial.println(mynum[i], BIN);
    }
    Serial.println();
}

void loop()
{
  Serial.println("Cycle:");
  
  for (int i=0;i<8;i++)
  {
    //dis_num[i] = mynum[i] & 0xFF; //((dis_num1[i] << count) & 0xFE) & (dis_num2[i] & count);
    dis_num[i] = static_cast<uint8_t>(mynum[i]);
  }
  Serial.println("Number to display, dis_num: ");
  for (int i=0;i<8;i++)
  {
      Serial.print("    ");
      Serial.println(dis_num[i], BIN);
  }
  Serial.println();
//  count++;
  for (int i=0;i<8;i++)
  {
    mynum[i] = rol(mynum[i]);
  }

  
//  if (count == 8)
//  {
//    count = 0;
//  }
  


//  while(1)
//  {
//    printNumber(lc, '0');
//    delay(1000);
//    printNumber(lc, '1');
//    delay(1000);
//    printNumber(lc, '2');
//    delay(1000);

    printNumber_2(lc, dis_num);
//    delay(1000);
//    printNumber(lc, '4');
//    delay(1000);
//    printNumber(lc, '5');
//    delay(1000);
//    printNumber(lc, '6');
//    delay(1000);
//    printNumber(lc, '7');
//    delay(1000);
//    printNumber(lc, '8');
//    delay(1000);
//    printNumber(lc, '9');
//    delay(1000);
//  }
    delay(200);
}

void printSerialNumber(uint16_t number*)
{
    Serial.println("Number to display, mynum: ");
    for (int i=0;i<8;i++)
    {
        Serial.print("    ");
        Serial.println(number[i], BIN);
    }
    Serial.println();  
}
void printSerialNumber(uint8_t number*)
{
    Serial.println("Number to display, mynum: ");
    for (int i=0;i<8;i++)
    {
        Serial.print("    ");
        Serial.println(number[i], BIN);
    }
    Serial.println();  
}

uint16_t rol(uint16_t val) {
    return (val << 1) | (val >> (sizeof(uint16_t)*CHAR_BIT-1));
}



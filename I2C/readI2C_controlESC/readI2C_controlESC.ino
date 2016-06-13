#include <Wire.h>
#include <Arduino.h>
#include <RCArduinoFastLib.h>
#include <PinChangeInt.h>
  
#define SLAVE_ADDRESS 0x08

// Assign servo indexes
#define SERVO_M0 0
#define SERVO_M1 1
#define SERVO_M2 2
#define SERVO_M3 3
#define SERVO_M0_PIN 9
#define SERVO_M1_PIN 10
#define SERVO_M2_PIN 11
#define SERVO_M3_PIN 12
#define SERVO_FRAME_SPACE 4


int pwm_out[4] = {1000, 1000, 1000, 1000};

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveCallback);
  Serial.begin(115200);
  
  CRCArduinoFastServos::attach(SERVO_M0, SERVO_M0_PIN);
  CRCArduinoFastServos::attach(SERVO_M1, SERVO_M1_PIN);
  CRCArduinoFastServos::attach(SERVO_M2, SERVO_M2_PIN);
  CRCArduinoFastServos::attach(SERVO_M3, SERVO_M3_PIN);
  CRCArduinoFastServos::begin();

  CRCArduinoFastServos::writeMicroseconds(SERVO_M0, 1000);
  CRCArduinoFastServos::writeMicroseconds(SERVO_M1, 1000);
  CRCArduinoFastServos::writeMicroseconds(SERVO_M2, 1000);
  CRCArduinoFastServos::writeMicroseconds(SERVO_M3, 1000);
  delay(2000);
  Serial.println();
  Serial.println("Finished Initialization: ");
}

void loop()
{

}

void receiveCallback(int howMany)
{
  int bytesReceived = Wire.available();
//  Serial.print(/"Bytes received: ");
//  Serial.pri/ntln(bytesReceived);
  
  //  while ((Serial.available() <= 16) || (Serial.read() != 'a') ) {};
  // Checking for errors
  if (bytesReceived > 17)
  {
    Serial.println("ERROR: Received too many bytes.");
    while(Wire.available()) Wire.read(); // flush I2C bus
    return;
  }

  uint8_t c = Wire.read();
  Serial.print("Flag Character: ");
  Serial.println((char)c);
  if (c != 'a')
  {
    Serial.println("ERROR: Missing initializer 'a'");
    while(Wire.available()) Wire.read(); // flush I2C bus
    return;
  }

  int tmp;
  for(int i = 0; i < 4; i++)
  {
     tmp = (int)(Wire.read() - '0');
     tmp = tmp*10 + (int)(Wire.read() - '0');
     tmp = tmp*10 + (int)(Wire.read() - '0');
     tmp = tmp*10 + (int)(Wire.read() - '0');
     
//     Serial.print("tmp: ");
//     Serial.println(tmp);
     

      if(tmp < 2001 && tmp > 999)
      {
          pwm_out[i] = tmp;
      }
      else
      {
        Serial.println("Inconsistent PWM");
      }
  }
//  Serial.print("    MOTOR PWM1: ");
//  Serial.println(pwm_out[0]);
//  Serial.print("    MOTOR PWM2: ");
//  Serial.println(pwm_out[1]);
//  Serial.print("    MOTOR PWM3: ");
//  Serial.println(pwm_out[2]);
//  Serial.print("    MOTOR PWM4: ");
//  Serial.println(pwm_out[3]);
  
  CRCArduinoFastServos::writeMicroseconds(SERVO_M0, pwm_out[0]);
  CRCArduinoFastServos::writeMicroseconds(SERVO_M1, pwm_out[1]);
  CRCArduinoFastServos::writeMicroseconds(SERVO_M2, pwm_out[2]);
  CRCArduinoFastServos::writeMicroseconds(SERVO_M3, pwm_out[3]);

  while(Wire.available()) Wire.read(); // flush I2C bus
  return;
}


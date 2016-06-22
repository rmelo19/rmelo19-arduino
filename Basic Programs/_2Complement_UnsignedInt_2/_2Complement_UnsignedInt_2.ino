void setup() {
  Serial.begin(9600);
  delay(4000);
}

void loop() {
//  delay(2000);/
  Serial.println("Coefficients: ");
  // Unsigned int test
  int16_t a0coeff = ((0x3E << 8) | 0xCE);
  int16_t b1coeff = ((0xB3 << 8) | 0xF9);
  int16_t b2coeff = ((0xC5 << 8) | 0x17);
  int16_t c12coeff = ((0x33 << 8) | 0xC8);

//  float a0 = (float)a0coeff / 8; // shift right by 3 bits
//  float b1 = (float)b1coeff / 8192; // shift right by 13 bits
//  float b2 = (float)b2coeff / 16384; // shift right by 14 bits
//  float c12 = (float)c12coeff;
//  c12 /= 4194304.0/; // shift right by 22 bits
  int16_t a0 = a0coeff;
  int16_t b1 = b1coeff;
  int16_t b2 = b2coeff;
  int16_t c12 = c12coeff;
  Serial.print("    a0: ");
  Serial.println(a0);
  Serial.print("    b1: ");
  Serial.println(b1);  
  Serial.print("    b2: ");
  Serial.println(b2);
  Serial.print("    c12: ");
  Serial.println(c12);

  uint16_t pressure = ((0x66 << 8) | 0x80);
  uint16_t temp =     ((0x7E << 8) | 0xC0);
  pressure =  pressure >> 6;
  temp = temp >> 6;
  Serial.print("    pressure: ");
  Serial.println(pressure);
  Serial.print("    temp: ");
  Serial.println(temp);

  int16_t pressureComp16;
  int32_t pressureComp;
//  int32_t pressureComp = a0 + (b1 + c/12 * temp) * pressure + b2 * temp;
  int32_t aux = (((int32_t) c12)*temp) >> 11;
  aux = (int32_t)b1 + aux;
  aux = aux*pressure;
  aux = (((int32_t)a0)<<10) + aux;
  int32_t aux2 = (((int32_t)b2) *temp)>>1;
  pressureComp = (aux + aux2)>>9;
  
  pressureComp16 = (int16_t) pressureComp;
  Serial.print("    compensated pressure: ");
  Serial.println(pressureComp16);

//  int32_t finalPressure = ((65 / 1023.0) * pressureComp16) + 50; // kPa
  int32_t finalPressure = ((((int32_t) pressureComp16) * 1041) >> 14) + 50;
  uint16_t finalPressure16 = (uint16_t) finalPressure;
  Serial.print("    final pressure: ");
  Serial.println(finalPressure16/16);

  
  
  
  while(1)
  {
    Serial.println("I am alive!");
    delay(1000);
  }
}

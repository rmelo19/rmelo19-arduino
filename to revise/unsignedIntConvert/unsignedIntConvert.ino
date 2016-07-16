void setup() {
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  Serial.println("Coefficients: ");
  // Unsigned int test
  int16_t a0coeff = (((uint16_t) 0x3E << 8) | 0xCE);
  int16_t b1coeff = (((uint16_t) 0xB3 << 8) | 0xF9);
  int16_t b2coeff = (((uint16_t) 0xC5 << 8) | 0x17);
  int16_t c12coeff = (((uint16_t) (0x33 << 8) | 0x68)) >> 2;

  float a0 = (float)a0coeff / 8; // shift right by 3 bits
  float b1 = (float)b1coeff / 8192; // shift right by 13 bits
  float b2 = (float)b2coeff / 16384; // shift right by 14 bits
  float c12 = (float)c12coeff;
  c12 /= 4194304.0; // shift right by 22 bits
  Serial.print("    a0: ");
  Serial.println(a0, 5);
  Serial.print("    b1: ");
  Serial.println(b1, 5);
  Serial.print("    b2: ");
  Serial.println(b2, 5);
  Serial.print("    c12: ");
  Serial.println(c12, 5);
  
  while(1)
  {
    Serial.println("I am alive!");
    delay(1000);
  }
}

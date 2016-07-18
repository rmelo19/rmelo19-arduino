#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme; // I2C

void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));
  delay(100);
  if (!bme.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
}
void loop() {
    Serial.print(F("Temperature = "));
    Serial.print(bme.readTemperature());
    Serial.print("*C ");

    Serial.print(F("Raw Temperature = "));
    Serial.print(bme.readRawTemperature());
    Serial.print("*C ");
    
    Serial.print(F("Pressure = "));
    Serial.print(bme.readPressure());
    Serial.print("Pa ");

    Serial.print(F("Raw Pressure = "));
    Serial.print(bme.readRawPressure());
    Serial.println();
    delay(100);
}

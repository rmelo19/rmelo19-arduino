/* Testing UART Communication between Arduino and Drone
 * Hardware:
 *  - Connect the following wires:
 *      SDA (A4) Arduino  <-> Logic Level Converter B1  <-> Logic Level Converter A1  <-> SDA Jetson
 *      SCL (A5) Arduino  <-> Logic Level Converter B2  <-> Logic Level Converter A2  <-> SCL Jetson
 *      5V Arduino  <-> Logic Level Converter HV
 *      GND Arduino <-> Logic Level Converter GND
 *      GND Jetson  <-> Logic Level Converter GND
 *      3.3V Jetson <-> Logic Level Converter LV
 * Arduino:
 *  - Upload this code
 * Jetson TK1 or TX1:
 *  - Open the terminal
 *  - sudo i2cdetect -r 1: you should see the address 0x08 appearing
 *  - sudo i2cdump -y 1 0x08: you should see a matrix of 0x09 (9 in decimal) 
 * 
 * Author: Rodrigo Marques
 * Date: 11/05/2016
 * Project: Agile Drones
 */

#include <Wire.h>

#define SLAVE_ADDRESS 0x08

void setup()
{
  // initializing i2c bus
  Wire.begin(SLAVE_ADDRESS);
  // callback function when data is requested
  Wire.onRequest(requestEvent);
}

void loop() {
}

void requestEvent()
{
  Wire.write(9);
}

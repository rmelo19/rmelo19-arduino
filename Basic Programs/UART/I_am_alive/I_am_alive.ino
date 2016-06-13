/* Testing UART Communication between Arduino and Drone
 * Hardware:
 *  - Connect the following wires:
 *      TX Arduino  <-> Logic Level Converter A1  <-> RX Jetson
 *      RX Arduino  <-> Logic Level Converter A2  <-> TX Jetson
 *      5V Arduino  <-> Logic Level Converter HV
 *      GND Arduino <-> Logic Level Converter GND
 *      GND Jetson  <-> Logic Level Converter GND
 *      1.8V Jetson <-> Logic Level Converter LV
 * Arduino:
 *  - Upload this code
 * Jetson TK1:
 *  - Open the terminal
 *  - cat < /dev/ttyTHS*
 * 
 * Jetson TX1 with Astro Carrier Board
 *  - It doesn't have exposed UART
 *   
 * Author: Rodrigo Marques
 * Date: 11/05/2016
 * Project: Agile Drones
 */

void setup() {
  Serial.begin(115200);
}
int i = 0;
void loop() {
  Serial.print("Are you listenning? ");
  Serial.println(i);
  i = (i + 1) % 100;
  delay(1000);
}

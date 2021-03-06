// #include "leds.h"
#include "stm32f4xx.h"
// #include "systime.h"
#include "pin.h"

#include "leds.h"
#include "error.h"
#include "console.h"
#include "enet.h"
#include <stdio.h>
#include "systime.h"
#include "dmxl.h"
#include "fan.h"
#include "tactile.h"
#include "enc.h"
#include "state.h"
#include "async_poll.h"

// #include <string>

#include <stdio.h> // check

#define APB_MHZ 42                            // our APB frequency is 42 mhz
#define I2C_CCR 210                           // for 100 kHz i2c: 42 mhz / (2 * 100 khz) = 210
#define I2C_TRISE (APB_MHZ * 200 / 1000 + 1)

// I2C ADRESSES
#define AS5048_ADDRESS       0x40             // 0b10000 + ( A1 & A2 to GND) -> 0b1000000
#define AS5048_ADDRESS_READ  0x41             // 0b01000001
#define AS5048_ADDRESS_WRITE 0x40             // 0b01000000
#define ARDUINO_7BIT_ADDR    0x20


// AS5048B, DEVICE SPECIFIC REGISTER ADDRESSES
#define AS5048B_ANGLMSB_REG 0xFE //bits 0..7
#define AS5048B_ANGLLSB_REG 0xFF //bits 0..5

// AS5048 CONSTANTS
#define AS5048B_RESOLUTION 16384.0 //14 bits


// I2C PORTS ON MICROCONTROLLER
#define PORTB_I2C1_SCL 6
#define PORTB_I2C1_SDA 7
#define PORTA_I2C3_SCL 8
#define PORTC_I2C3_SDA 9

#define ON  1
#define OFF 0

// FUNCTION PROTOTYPES
void ledsPattern(int, int, int, int);
void udelay(int);
void writeRegister(uint8_t address, uint8_t registerAddress);
void readBytes(uint8_t addresss, int numBytes, uint8_t* values);
bool isBusyI2C3(void);



int main()
{

  char buffer[2000];
  uint16_t bytesToSend;

  // initialization
  //############################################################################################
  systime_init();
  leds_init();
  enet_init();

  // setting reset and clock control register
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | // GPIOA clock enable during sleep mode
                  RCC_AHB1ENR_GPIOBEN | // GPIOB clock enable during sleep mode
                  RCC_AHB1ENR_GPIOCEN | // GPIOC clock enable during sleep mode
                  RCC_AHB1ENR_GPIODEN;  // GPIOD clock enable during sleep mode

  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN |  // enable I2C bus 1 
                  RCC_APB1ENR_I2C3EN;   // enable I2C bus 3

  // set i2c pins for each bus
  pin_set_alternate_function(GPIOB, PORTB_I2C1_SCL, 4);
  pin_set_alternate_function(GPIOB, PORTB_I2C1_SDA, 4);
  pin_set_alternate_function(GPIOA, PORTA_I2C3_SCL, 4);
  pin_set_alternate_function(GPIOC, PORTC_I2C3_SDA, 4);

  // set 
  pin_set_output_type(GPIOB, PORTB_I2C1_SCL, PIN_OUTPUT_TYPE_OPEN_DRAIN);
  pin_set_output_type(GPIOB, PORTB_I2C1_SDA, PIN_OUTPUT_TYPE_OPEN_DRAIN);
  pin_set_output_type(GPIOA, PORTA_I2C3_SCL, PIN_OUTPUT_TYPE_OPEN_DRAIN);
  pin_set_output_type(GPIOC, PORTC_I2C3_SDA, PIN_OUTPUT_TYPE_OPEN_DRAIN);

  __enable_irq();

  // Configuring Control Registers, i2c bus 1
  I2C1->CR2   |= APB_MHZ;     // 42MHz -> 101010
  I2C1->CCR   |= I2C_CCR;     // I2C_CCR = 210
  I2C1->TRISE &= ~0x3f;       // 0x3f = 0b00111111 -> 0b11000000, clearing register
  I2C1->TRISE |= I2C_TRISE;   // = I2C_TRISE = (APB_MHZ * 200 / 1000 + 1), maximum rise time
  I2C1->CR1   |= I2C_CR1_PE;  // peripheral enable

  // Configuring Control Registers, i2c bus 3
  I2C3->CR2   |= APB_MHZ;     // 42MHz -> 101010
  I2C3->CCR   |= I2C_CCR;     // I2C_CCR = 210
  I2C3->TRISE &= ~0x3f;       // 0x3f = 0b00111111 -> 0b11000000, clearing register
  I2C3->TRISE |= I2C_TRISE;   // = I2C_TRISE = (APB_MHZ * 200 / 1000 + 1), maximum rise time
  I2C3->CR1   |= I2C_CR1_PE;  // peripheral enable

  udelay(1000000);

  bool error = false;
  while(1)
  {
		// I2C communication
		// ############################################################################################
		writeRegister(AS5048_ADDRESS, AS5048B_ANGLLSB_REG);

		// Request and read 2 bytes
		uint8_t valueRead[2000];
		readBytes(AS5048_ADDRESS, 2, valueRead);

		udelay(200000);

		// uint16_t readValue;
		// readValue = (((uint16_t) Wire.read()) << 6) ;
		// readValue += (uint16_t) (Wire.read() & 0x3F);

		double angle;
		angle = (double) (((((uint16_t) valueRead[0] << 6) + ((uint16_t) (valueRead[1] & 0x3F))) * 360.0) / AS5048B_RESOLUTION);
		bytesToSend = sprintf(buffer, "Found error: %d, value read: %2x %2x %6.2f", (int) error, valueRead[0], valueRead[1], angle);

		if (enet_get_link_status() == ENET_LINK_UP)
		{
			volatile uint8_t* messages = (uint8_t*) buffer;
			enet_send_state(messages, bytesToSend);
		}
		enet_process_rx_ring();
  }
  return 0;
}


void ledsPattern(int led1, int led2, int led3, int led4)
{
	int leds[4] = {led1, led2, led3, led4};

	for(int i=0; i<4;i++)
	{
		if (leds[i] == ON)
		{
			leds_on(i);
		}
		else
		{
			leds_off(i);
		}
	}
}

void udelay(int utime)
{
	int mytime = SYSTIME;
	while(SYSTIME - mytime < utime);
}
// bool timeout(int utime, int initialTime)
// {
// 	return(SYSTIME - initialTime > utime);
// }

void writeRegister(uint8_t address, uint8_t registerAddress)
{
	// starting i2c communication on I2c bus 3 
	I2C3->CR1 |=  I2C_CR1_START; // generating start condition
	I2C3->SR1 &= ~I2C_SR1_AF;    // clearing acknowledge

	while (!(I2C3->SR1 & I2C_SR1_SB));
	// initialTime = SYSTIME; while (!(I2C3->SR1 & I2C_SR1_SB) && !timeout(1000, initialTime));

	// Send address with write bit
	I2C3->DR = ((uint8_t) address << 1); // puts the address to be sent on the buffer using global variable

	while(!(I2C3->SR1 & (I2C_SR1_ADDR))); // wait for address transmission
	I2C3->SR2; // un-stretch clock by reading here (?)

	I2C3->DR = registerAddress; // Send the address of the desired register

	while (!(I2C3->SR1 & (I2C_SR1_BTF | I2C_SR1_AF)));

	I2C3->CR1 |= I2C_CR1_STOP;


	while (isBusyI2C3()); // wait until bus is not busy anymore	  
}


void readBytes(uint8_t address, int numBytes, uint8_t* values)
{
	// Send the address with the read bit
	// starting I2C3 communication
	I2C3->CR1 |=  I2C_CR1_START;
	I2C3->SR1 &= ~I2C_SR1_AF;

	while (!(I2C3->SR1 & I2C_SR1_SB));	

	// // Send address with read bit
	I2C3->DR = ((uint8_t) address << 1) + 1; // puts the address to be sent on the buffer using global variable

	while (!(I2C3->SR1 & (I2C_SR1_ADDR)));
	I2C3->SR2; // un-stretch clock by reading here (?)

	I2C3->CR1 |=  I2C_CR1_ACK; // multi-byte read. Acknowledge enable

	for (int i = 0; i < numBytes-1; i++)
	{
		while (!(I2C3->SR1 & I2C_SR1_RXNE));
		values[i] = I2C3->DR;
		I2C3->CR1 |=  I2C_CR1_ACK; // multi-byte read. Acknowledge enable
	}
	while (!(I2C3->SR1 & I2C_SR1_RXNE));
	values[numBytes-1] = I2C3->DR;
	I2C3->CR1 &= ~I2C_CR1_ACK; // last read

	I2C3->CR1 |= I2C_CR1_STOP;

	while (isBusyI2C3()); // wait until bus is not busy anymore
}

bool isBusyI2C3()
{
	return I2C3->SR2 & I2C_SR2_BUSY;
}
#include "enc.h"
#include "./stm32/stm32f4xx.h"
#include <stdio.h>
#include "pin.h"
#include "state.h"
#include "systime.h"
#include "error.h"
#include "leds.h"   //rmelo19, for debugging

// FOR SPI
#define PORTA_BRIDGE0_CS            4

// ERRORS
#define LIGHT_LED_THRESHOLD         100 // The number of times we get the same reading before we light an LED

#define I2C_ENC true

// AS5048 ADDRESSES
#define AS5048_7BIT_ADDRESS         0x40 // 0b10000 + ( A1 & A2 to GND) -> 0b1000000
#define AS5048_ADDRESSS_READ        0x81 // 0b01000001
#define AS5048_ADDRESS_WRITE        0x80 // 0b01000000
#define AS5048B_ANGLMSB_REG         0xFE // bits 0..7
#define AS5048B_ANGLLSB_REG         0xFF // bits 0..5

// ARDUINO TESTS
#define ARDUINO_7BIT_ADDR 0x20 

// EASY TO READ
#define TRUE                        1
#define FALSE                       0
#define ON                          1
#define OFF                         0

// FUNCTION PROTOTYPES, rmelo19
void udelay(int);
void ledsPattern(int led1, int led2, int led3, int led4);
int checkTimeout(int utime, int initialTime);

// SPI
void writeRegisterSPI(uint8_t address, uint8_t registerAddress);
void readBytesSPI(uint8_t addresss, uint8_t numBytes, uint8_t* values);

// I2C
int writeRegisterI2C(I2C_TypeDef* i2cPort, uint8_t address, uint8_t registerAddress);
int readBytesI2C(I2C_TypeDef* i2cPort, uint8_t addresss, int numBytes, uint8_t* values);
int isBusyI2CPort(I2C_TypeDef* i2cPort);
int readEncoderI2C(I2C_TypeDef* i2cPort, uint8_t encoderNumber, int timeout);

// GLOBAL VARIABLES
enc_async_poll_state_t enc_poll_state = EPS_DONE;

void enc_init()
{
  printf("enc_init()\r\n");
  RCC->APB2ENR |= RCC_APB2ENR_SPI4EN; // turn on SPI4

  pin_set_output(GPIOE, PORTE_ENC_CS);
  pin_set_output_level(GPIOE, PORTE_ENC_CS, 1);
  pin_set_alternate_function(GPIOE, PORTE_ENC_SCLK, 5);
  pin_set_alternate_function(GPIOE, PORTE_ENC_MISO, 5);
  pin_set_alternate_function(GPIOE, PORTE_ENC_MOSI, 5);

  // spi4 is running from a 84 MHz pclk. set it up with
  // sclk = pclk/128 to be super slow for now.
  SPI4->CR1 = SPI_CR1_DFF  | // 16-bit mode
              SPI_CR1_BR_2 | // sclk = pclk/128
              SPI_CR1_BR_1 | // sclk = pclk/128
              SPI_CR1_MSTR | // master mode
              SPI_CR1_CPHA | // cpha=1, cpol=0 for AS5048A
              SPI_CR1_SSM  | // software slave-select mode
              SPI_CR1_SSI  | // the value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored.
              SPI_CR1_SPE;   // SPI enable
}


/*
void enc_poll_nonblocking_tick(const uint8_t bogus __attribute__((unused)))
  Reads encoders values.

  Encoders connections:
    0 -> Port I2C1
    1 -> Port SPI
    2 -> Port I2C3
*/
void enc_poll_nonblocking_tick(const uint8_t bogus __attribute__((unused)))
{
  static uint8_t i2cPort1Updated = 0;
  static uint8_t i2cPort3Updated = 0;

  switch(enc_poll_state)
  {
    case EPS_DONE:
        readEncoderSPI(SPI1, 1, timeout); // (spiPort, encoderNumber, timeout)
        
        if (isBusyI2CPort(I2C3) && isBusyI2CPort(I2C1))
        {
          enc_poll_state = EPS_I2C;
          i2cPort1Updated = 0;
          i2cPort3Updated = 0;
        }
        else if (!isBusyI2CPort(I2C3) && !isBusyI2CPort(I2C1))
        {
          readEncoderI2C(I2C1, 0, timeout);  // (i2cPort, encoderNumber, timeout)
          readEncoderI2C(I2C3, 2, timeout);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        } 
        else if (!isBusyI2CPort(I2C1))
        {
          readEncoderI2C(I2C1, 0, timeout);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        }
        else if (!isBusyI2CPort(I2C3))
        {
          readEncoderI2C(I2C3, 2, timeout);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        }      
      break;
    case EPS_I2C:
        if (!isBusyI2CPort(I2C3) && !isBusyI2CPort(I2C1))
        {
          readEncoderI2C(I2C1, 0, timeout);  // (i2cPort, encoderNumber, timeout)
          readEncoderI2C(I2C3, 2, timeout);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        } 
        else if (!isBusyI2CPort(I2C1) && i2cPort1Updated == 0)
        {
          readEncoderI2C(I2C1, 0, timeout);  // (i2cPort, encoderNumber, timeout)
          i2cPort1Updated = 1;
          if (i2cPort3Updated == 1)
            enc_poll_state = EPS_DONE;
        }
        else if (!isBusyI2CPort(I2C3) && i2cPort3Updated == 0)
        {
          readEncoderI2C(I2C3, 2, timeout);  // (i2cPort, encoderNumber, timeout)
          i2cPort3Updated = 1;
          if (i2cPort1Updated == 1)
            enc_poll_state = EPS_DONE;
        }
      break;
    default:
      enc_poll_state = EPS_DONE; // shouldn't get here
      break;
  }
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

int checkTimeout(int utime, int initialTime)
{
 return (SYSTIME - initialTime > utime);
}

int writeRegisterI2C(I2C_TypeDef *i2cPort, uint8_t address, uint8_t registerAddress)
{
  // starting i2c communication on I2c bus 3 
  i2cPort->CR1 |=  I2C_CR1_START; // generating start condition
  i2cPort->SR1 &= ~I2C_SR1_AF;    // clearing acknowledge

  ledsPattern(ON, ON, ON, ON);
  while (!(i2cPort->SR1 & I2C_SR1_SB));
  // initialTime = SYSTIME; while (!(i2cPort->SR1 & I2C_SR1_SB) && !timeout(1000, initialTime));

  // Send address with write bit
  i2cPort->DR = ((uint8_t) address << 1); // puts the address to be sent on the buffer using global variable

  ledsPattern(ON, ON, ON, OFF);
  while(!(i2cPort->SR1 & (I2C_SR1_ADDR))); // wait for address transmission
  i2cPort->SR2; // un-stretch clock by reading here (?)

  i2cPort->DR = registerAddress; // Send the address of the desired register

  ledsPattern(ON, ON, OFF, ON);
  while (!(i2cPort->SR1 & (I2C_SR1_BTF | I2C_SR1_AF)));

  i2cPort->CR1 |= I2C_CR1_STOP;

  ledsPattern(ON, OFF, ON, ON);
  while (isBusyI2CPort(i2cPort)); // wait until bus is not busy anymore   
}


int readBytesI2C(I2C_TypeDef *i2cPort, uint8_t address, int numBytes, uint8_t* values)
{
  // Send the address with the read bit
  // starting I2C3 communication
  i2cPort->CR1 |=  I2C_CR1_START;
  i2cPort->SR1 &= ~I2C_SR1_AF;

  while (!(i2cPort->SR1 & I2C_SR1_SB));  

  // // Send address with read bit
  i2cPort->DR = ((uint8_t) address << 1) + 1; // puts the address to be sent on the buffer using global variable

  while (!(i2cPort->SR1 & (I2C_SR1_ADDR)));
  i2cPort->SR2; // un-stretch clock by reading here (?)

  i2cPort->CR1 |=  I2C_CR1_ACK; // multi-byte read. Acknowledge enable

  for (int i = 0; i < numBytes-1; i++)
  {
    while (!(i2cPort->SR1 & I2C_SR1_RXNE));
    values[i] = i2cPort->DR;
    i2cPort->CR1 |=  I2C_CR1_ACK; // multi-byte read. Acknowledge enable
  }
  while (!(i2cPort->SR1 & I2C_SR1_RXNE));
  values[numBytes-1] = i2cPort->DR;
  i2cPort->CR1 &= ~I2C_CR1_ACK; // last read

  i2cPort->CR1 |= I2C_CR1_STOP;

  while (isBusyI2CPort(i2cPort)); // wait until bus is not busy anymore
}

int isBusyI2CPort(I2C_TypeDef* i2cPort)
{
  return i2cPort->SR2 & I2C_SR2_BUSY;
}

void writeRegisterSPI(SPI_TypeDef* spiPort, uint8_t address, uint8_t registerAddress)
{
  GPIO_TypeDef *cs_gpio = NULL;
  uint32_t cs_pin_mask = 0;

  cs_gpio = GPIOA;
  cs_pin_mask = 1 << PORTA_BRIDGE0_CS;
  
  cs_gpio->BSRRH = cs_pin_mask; // assert CS
  
  udelay(4); // delay 4us
  spiPort->DR = 0x00; // send write command 

  udelay(15); // delay 15us
  spiPort->DR = (uint8_t) 1; // send data len

  udelay(15); // delay 15us
  spiPort->DR = address; // send addr

  udelay(15); // delay 15us
  spiPort->DR = registerAddress;

  udelay(15); // delay 15us
  cs_gpio->BSRRL = cs_pin_mask; // de-assert CS

  const uint32_t us_to_wait = 180 + 110 * 1;
  udelay(us_to_wait);

}
void readBytesSPI(SPI_TypeDef* spiPortPort, uint8_t address, uint8_t numBytes, uint8_t* values)
{
  GPIO_TypeDef *cs_gpio = NULL;
  uint32_t cs_pin_mask = 0;

  cs_gpio = GPIOA;
  cs_pin_mask = 1 << PORTA_BRIDGE0_CS;
  
  cs_gpio->BSRRH = cs_pin_mask; // assert CS
  
  udelay(4); // delay 4us
  spiPort->DR = 0x01; // send read command 
        
  udelay(15); // delay 15us
  spiPort->DR = (uint8_t) numBytes; // send data len

  udelay(15); // delay 15us
  spiPort->DR = address; // send addr

  udelay(15); // delay 15us
  cs_gpio->BSRRL = cs_pin_mask; // de-assert CS

  const uint32_t us_to_wait = 180 + 110 * numBytes;
  udelay(us_to_wait);
  cs_gpio->BSRRH = cs_pin_mask; // assert CS

  udelay(4);
  spiPort->DR = 0x06; // read buffer command
  spiPort->DR;

  udelay(15);
  values[0] = (uint8_t) spiPort->DR;
  spiPort->DR = 0x0;

  udelay(15);
  values[1] = (uint8_t) spiPort->DR;
  cs_gpio->BSRRL = cs_pin_mask; // de-assert CS

  udelay(30);
}

int readEncoderI2C(I2C_TypeDef* i2cPort, uint8_t encoderNumber, int timeout)
{
  writeRegisterI2C(i2cPort, AS5048_7BIT_ADDRESS, AS5048B_ANGLLSB_REG);
  uint8_t valueRead[2];
  readBytesI2C(i2cPort, AS5048_7BIT_ADDRESS, 2, valueRead);
  g_state.encoders[encoderNumber] = (((uint16_t) valueRead[0] << 6) + ((uint16_t) (valueRead[1] & 0x3F))); 
}

int readEncoderSPI(SPI_TypeDef* spiPort, uint8_t encoderNumber, int timeout)
{
  writeRegisterSPI(spiPort, AS5048_7BIT_ADDRESS, AS5048B_ANGLLSB_REG);
  uint8_t valueRead[2];
  readBytesSPI(spiPort, AS5048_7BIT_ADDRESS, 2, valueRead);
  g_state.encoders[encoderNumber] = (((uint16_t) valueRead[0] << 6) + ((uint16_t) (valueRead[1] & 0x3F)));
}
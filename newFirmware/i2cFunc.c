#include "i2cFunc.h"

int writeRegisterI2C(I2C_TypeDef *i2cPort, uint8_t address, uint8_t registerAddress)
{
  // starting i2c communication on I2c bus 3 
  i2cPort->CR1 |=  I2C_CR1_START; // generating start condition
  i2cPort->SR1 &= ~I2C_SR1_AF;    // clearing acknowledge

  while (!(i2cPort->SR1 & I2C_SR1_SB));
  // initialTime = SYSTIME; while (!(i2cPort->SR1 & I2C_SR1_SB) && !timeout(1000, initialTime));

  // Send address with write bit
  i2cPort->DR = ((uint8_t) address << 1); // puts the address to be sent on the buffer using global variable

  while(!(i2cPort->SR1 & (I2C_SR1_ADDR))); // wait for address transmission
  i2cPort->SR2; // un-stretch clock by reading here (?)

  i2cPort->DR = registerAddress; // Send the address of the desired register

  while (!(i2cPort->SR1 & (I2C_SR1_BTF | I2C_SR1_AF)));

  i2cPort->CR1 |= I2C_CR1_STOP;

  while (isBusyI2CPort(i2cPort)); // wait until bus is not busy anymore   

  return 0;
}


int readBytesI2C(I2C_TypeDef *i2cPort, uint8_t address, int numBytes, uint8_t* values)
{
  // Send the address with the read bit
  // starting I2C communication
  i2cPort->CR1 |=  I2C_CR1_START;
  i2cPort->SR1 &= ~I2C_SR1_AF;

  while (!(i2cPort->SR1 & I2C_SR1_SB));  

  // Send address with read bit
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

  return 0;
}

int isBusyI2CPort(I2C_TypeDef* i2cPort)
{
  return i2cPort->SR2 & I2C_SR2_BUSY;
}
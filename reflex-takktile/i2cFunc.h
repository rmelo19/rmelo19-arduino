#ifndef I2CFUNC_H
#define I2CFUNC_H

#include "./stm32/stm32f4xx.h"
#include "config.h"

int writeRegisterI2C(I2C_TypeDef* i2cPort, uint8_t address, uint8_t registerAddress);
int readBytesI2C(I2C_TypeDef* i2cPort, uint8_t addresss, int numBytes, uint8_t* values);
int isBusyI2CPort(I2C_TypeDef* i2cPort);

#endif
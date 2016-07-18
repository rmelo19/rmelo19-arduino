#ifndef SPIFUNC_H
#define SPIFUNC_H

#include "./stm32/stm32f4xx.h"
#include "systime.h"
#include <stdio.h>
#include <leds.h>
// #include <stdint.h>
#include "ports.h"

#define ON                          1
#define OFF                         0

#define PORTC_I2C_BRIDGE_RESET 14

void udelay(int);
int writeRegisterSPI(SPI_TypeDef* spiPort, uint8_t address, uint8_t registerAddress);
int readBytesSPI(SPI_TypeDef* spiPort, uint8_t address, uint8_t numBytes, uint8_t* values);
int writeBytesSPI(SPI_TypeDef* spiPort, uint8_t address, uint8_t* data, int len, int toggleAddress);

uint8_t readSPIStatus(SPI_TypeDef * spiPort);
int readBytesSPIAssert(SPI_TypeDef* spiPort, uint8_t address, uint8_t numBytes, volatile uint8_t* values);
int readCommmand(SPI_TypeDef* spiPort,uint8_t address, uint8_t numBytes);
void ledStatus(uint8_t status);
void ledsPattern(int led1, int led2, int led3, int led4);

#endif
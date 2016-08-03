#ifndef ENC_H
#define ENC_H

#include "i2cFunc.h"
#include "spiFunc.h"

#include <reflex.h>
#include <stdint.h>
#include "async_poll.h"
#include "./stm32/stm32f4xx.h"
#include <stdio.h>
#include "pin.h"
#include "state.h"
#include "systime.h"
#include "error.h"
#include "leds.h"   //rmelo19, for debugging
#include "ports.h"

// FOR SPI
#define PORTA_BRIDGE0_CS            4

// ERRORS
#define LIGHT_LED_THRESHOLD         100 // The number of times we get the same reading before we light an LED

// ARDUINO TESTS
#define ARDUINO_7BIT_ADDR 0x20 

// EASY TO READ
#define TRUE                        1
#define FALSE                       0
#define ON                          1
#define OFF                         0

typedef enum 
{ 
  ENCODER_STATE_SET_REGISTER = 0,
  ENCODER_STATE_READ_VALUES,
  ENCODER_STATE_WAIT = ASYNC_POLL_DONE
} enc_async_poll_state_t;

extern enc_async_poll_state_t enc_poll_state[3];

uint8_t setEncoderRegister(uint8_t encoderNumber, uint8_t encoderRegister,int timeout);
uint8_t readEncoderValues(uint8_t encoderNumber, int timeout);

void encInit();
void enc_poll();
void enc_poll_nonblocking_tick(const uint8_t encoderNumber);



#endif


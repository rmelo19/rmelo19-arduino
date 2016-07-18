#ifndef ENC_H
#define ENC_H


#define NUM_ENC 3

#include "i2cFunc.h"
#include "spiFunc.h"

#include <stdint.h>
#include "async_poll.h"
#include "./stm32/stm32f4xx.h"
#include <stdio.h>
#include "pin.h"
#include "state.h"
#include "systime.h"
#include "error.h"
#include "leds.h"   //rmelo19, for debugging

void enc_init();
void enc_poll();
void enc_poll_nonblocking_tick(const uint8_t bogus __attribute__((unused)));

typedef enum 
{ 
  EPS_I2C,
  EPS_DONE = ASYNC_POLL_DONE
} enc_async_poll_state_t;
extern enc_async_poll_state_t enc_poll_state;



#endif


#include "enc.h"
#include "./stm32/stm32f4xx.h"
#include <stdio.h>
#include "pin.h"
#include "state.h"
#include "systime.h"
#include "error.h"
#include "leds.h" //rmelo19
#include <stdio.h>  //rmelo19

/////////////////////////////////////////////////////////////////////
// encoder daisy-chain setup
//
// spi4 cs   = pe11, af 5
// spi4 sclk = pe12, af 5
// spi4 miso = pe13, af 5
// spi4 mosi = pe14, af 5

#define PORTE_ENC_CS   11
#define PORTE_ENC_SCLK 12
#define PORTE_ENC_MISO 13
#define PORTE_ENC_MOSI 14

#define TRUE 1
#define FALSE 0
// The number of times we get the same reading before we light an
// LED
static const uint8_t LIGHT_LED_THRESHOLD = 100;

enc_async_poll_state_t enc_poll_state = { EPS_DONE };

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
              SPI_CR1_SPE; // SPI enable

  enc_poll(); // first one will be garbage
  enc_poll(); // and the second one
}

/*
 * Blocking call to get encoder data for all 3 encoders
 */
void enc_poll()
{
  GPIOE->BSRRH = 1 << PORTE_ENC_CS; // assert (pull down) CS
  for (volatile int i = 0; i < 10; i++) { } // needs at least 350 ns
  SPI4->DR; // clear the rx data register in case it has some garbage
  for (int i = 0; i < NUM_ENC; i++)
  {
    SPI4->DR = 0xffff;
    while (!(SPI4->SR & SPI_SR_TXE)) { } // Wait for transmit buffer empty
    while (!(SPI4->SR & SPI_SR_RXNE)) { } // Wait for recieve buffer not empty
    g_state.encoders[NUM_ENC-1-i] = SPI4->DR & 0x3fff;  // Idx goes 2, 1, 0
  }
  for (volatile int i = 0; i < 1; i++) { } // needs at least 50 ns
  GPIOE->BSRRL = 1 << PORTE_ENC_CS; // de-assert (pull up) CS
  /*
  printf("       enc: %06d  %06d  %06d\r\n",
         g_state.encoders[0],
         g_state.encoders[1],
         g_state.encoders[2]);
  */
}


void enc_poll_nonblocking_tick(const uint8_t bogus __attribute__((unused)))
{
  static uint_fast8_t enc_poll_state_word_idx = 0;
  static uint32_t enc_poll_state_start_time_us = 0;
  static uint8_t all_the_same = TRUE;
  static uint8_t all_the_same_count = 0;
  static uint8_t same_count[3] = {0};

// #define I2C_ENC true
#define AS5048_ADDRESS 0x40 // 0b10000 + ( A1 & A2 to GND) -> 0b1000000
#define AS5048_ADDRESS_READ 0b01000001 // 0x41
#define AS5048_ADDRESS_WRITE 0b01000000 // 0x40
#define AS5048B_ANGLMSB_REG 0xFE //bits 0..7
#define AS5048B_ANGLLSB_REG 0xFF //bits 0..5

#define ARDUINO_7BIT_ADDR 0x20 

  switch(enc_poll_state)
  {
    // Initial State 
    //#########################################################################################
    case EPS_DONE:
      // I2C
      //######################################
      #ifdef I2C_ENC

      // defining i2c port
      // const uint_fast8_t port = 0;
      // I2C_TypeDef *i2c;
      // if (port == 0)
        // i2c = I2C1;
      // else
        // i2c = I2C3;
      // i2c = I2C1;
      leds_on(0);
      leds_on(1);
      leds_on(2);
      leds_on(3);

      uint32_t mytime;
      

      // starting i2c communication
      I2C3->CR1 |=  I2C_CR1_START; // generating start condition
      I2C3->SR1 &= ~I2C_SR1_AF; // clearing acknowledge
      mytime = SYSTIME;
      while (!(I2C3->SR1 & I2C_SR1_SB)){}
      if (SYSTIME - mytime < 1000000){} // rmelo19
      leds_off(0);
      leds_on(1);
      leds_on(2);
      leds_on(3);
      
      // Send address with write bit
      I2C3->DR = ((uint8_t) ARDUINO_7BIT_ADDR << 1); // puts the address to be sent on the buffer using global variable
      mytime = SYSTIME;
      // while (!(I2C3->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF))){} // wait for address transmission
      // while (!(I2C3->SR1 & (I2C_SR1_AF))){} // check if acknowledge received
      while (!(I2C3->SR1 & (I2C_SR1_ADDR))){}
      I2C3->SR2; // un-stretch clock by reading here (?)
      // while(SYSTIME - mytime < 1000000);
      leds_off(0);
      leds_off(1);
      leds_on(2);
      leds_on(3);

      I2C3->DR = AS5048B_ANGLLSB_REG; // Send the address of the desired register
      mytime = SYSTIME;
      while (!(I2C3->SR1 & (I2C_SR1_BTF | I2C_SR1_AF))){}
      // while(SYSTIME - mytime < 1000000);
      leds_off(0);
      leds_off(1);
      leds_off(2);
      leds_on(3);

      I2C3->CR1 |= I2C_CR1_STOP;


      mytime = SYSTIME;
      while (I2C3->SR2 & I2C_SR2_BUSY) // wait until bus is not busy anymore
      // while(SYSTIME - mytime < 1000000);
      leds_off(0);
      leds_off(1);
      leds_off(2);
      leds_off(3);

      // Send the address with the read bit

      // starting I2C3 communication
      I2C3->CR1 |=  I2C_CR1_START;
      I2C3->SR1 &= ~I2C_SR1_AF;
      mytime = SYSTIME;
      while (!(I2C3->SR1 & I2C_SR1_SB)){}
      // while(SYSTIME - mytime < 1000000);
      leds_on(0);
      leds_off(1);
      leds_off(2);
      leds_off(3);

      // Send address with read bit
      I2C3->DR = AS5048_ADDRESS_READ; // puts the address to be sent on the buffer using global variable
      mytime = SYSTIME;
      while (!(I2C3->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF))){}
      while (!(I2C3->SR1 & (I2C_SR1_AF))){} // check if acknowledge received
      I2C3->SR2; // un-stretch clock by reading here (?)

      // while(SYSTIME - mytime < 1000000);
      leds_on(0);
      leds_on(1);
      leds_off(2);
      leds_off(3);
      
      // Request and read 2 bytes
      // I2C3->CR1 &= ~I2C_CR1_ACK; // last read
      I2C3->CR1 |=  I2C_CR1_ACK; // multi-byte read. Acknowledge enable
      mytime = SYSTIME;
      while (!(I2C3->SR1 & I2C_SR1_RXNE)){}
      g_state.encoders[0] = I2C3->DR;
      // while(SYSTIME - mytime < 1000000);
      leds_on(0);
      leds_on(1);
      leds_on(2);
      leds_off(3);


      I2C3->CR1 &= ~I2C_CR1_ACK; // last read
      mytime = SYSTIME;
      while (!(I2C3->SR1 & I2C_SR1_RXNE)){}
      g_state.encoders[0] = I2C3->DR;
      // while(SYSTIME - mytime < 1000000);
      leds_on(0);
      leds_on(1);
      leds_on(2);
      leds_on(3);
      
      I2C3->CR1 |= I2C_CR1_STOP; 
      mytime = SYSTIME;
      while (I2C3->SR2 & I2C_SR2_BUSY) // wait until bus is not busy anymore
      // while(SYSTIME - mytime < 1000000);
      leds_off(0);
      leds_on(1);
      leds_on(2);
      leds_off(3);

      enc_poll_state = EPS_SPI_TXRX_DONE;
      // SPI
      //######################################
      #else
      GPIOE->BSRRH = 1 << PORTE_ENC_CS; // assert (pull down) CS

      // start counting time
      enc_poll_state_start_time_us = SYSTIME;

      // move to next state
      enc_poll_state = EPS_CS_ASSERTED;
      #endif
      
      break;

    // 
    //#########################################################################################
    case EPS_CS_ASSERTED:
      if (SYSTIME - enc_poll_state_start_time_us > 2) // wait for 2us
      {
        all_the_same = TRUE;
        SPI4->DR; // clear the rx data register in case it has some garbage
        enc_poll_state_word_idx = 0; //rmelo19
        SPI4->DR = 0xffff; // this register has 4 bytes

        // move to next state
        enc_poll_state = EPS_SPI_TXRX;
      }
      break;

    // 
    //#########################################################################################
    case EPS_SPI_TXRX: // NUM_ENC is 3
      if ((SPI4->SR & SPI_SR_TXE) && (SPI4->SR & SPI_SR_RXNE)) // if tx is empy and rx is not empty
      {
        uint16_t readValue =  SPI4->DR & 0x3fff; // 0b0011 1111 1111 1111

        if (readValue == g_state.encoders[NUM_ENC-1-enc_poll_state_word_idx]) // checking if encoder is stuck
        {
          same_count[NUM_ENC-1-enc_poll_state_word_idx]++;
        }
        else
        {
          same_count[NUM_ENC-1-enc_poll_state_word_idx] = 0;
          all_the_same = FALSE;
        }

        // store value in global variable and increment index
        g_state.encoders[NUM_ENC-1-(enc_poll_state_word_idx++)] = readValue;
        if (enc_poll_state_word_idx >= NUM_ENC)
        {
          enc_poll_state = EPS_SPI_TXRX_DONE;
          enc_poll_state_start_time_us = SYSTIME;
        }
        else
        {
          SPI4->DR = 0xffff; // waiting for another set of data
        }
      }
      break;

    // 
    //#########################################################################################
    case EPS_SPI_TXRX_DONE:
      if (SYSTIME - enc_poll_state_start_time_us > 2)  // wait for 2us
      {
        if (all_the_same)
        {
          if (all_the_same_count < LIGHT_LED_THRESHOLD)
          {
            all_the_same_count++;
          }
        }
        else
        {
          all_the_same_count = 0;
        }


        // Error Checking
        // if all encoders are stuck
        if (all_the_same_count >= LIGHT_LED_THRESHOLD)
        {
          err_set(ERR_ENC_ALL_STUCK);
        }
        else
        {
          err_unset(ERR_ENC_ALL_STUCK);
        }

        // if just encoder 1 is stuck
        if (same_count[0] >= LIGHT_LED_THRESHOLD)
        {
          err_set(ERR_ENC_1_STUCK);
        }
        else
        {
          err_unset(ERR_ENC_1_STUCK);
        }

        // if just encoder 2 is stuck
        if (same_count[1] >= LIGHT_LED_THRESHOLD)
        {
          err_set(ERR_ENC_2_STUCK);
        } 
        else
        {
          err_unset(ERR_ENC_2_STUCK);
        }

        // if just encoder 3 is stuck
        if (same_count[2] >= LIGHT_LED_THRESHOLD)
        {
          err_set(ERR_ENC_3_STUCK);
        } else {
          err_unset(ERR_ENC_3_STUCK);
        }

        // clear chip select
        GPIOE->BSRRL = 1 << PORTE_ENC_CS; // de-assert (pull up) CS

        // move to next state
        enc_poll_state = EPS_DONE;
      }
      break;

      #ifdef I2C_ENC
      // rmelo19  
      // case EPS_I2C_TXRX:


      // break;
      // case EPS_I2C_TXRX_DONE:


      // break;



      #endif





    default:
      enc_poll_state = EPS_DONE; // shouldn't get here
      break;
  }
}


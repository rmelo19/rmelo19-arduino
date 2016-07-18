
#include "enc.h"

// FOR SPI
#define PORTA_BRIDGE0_CS            4

// ERRORS
#define LIGHT_LED_THRESHOLD         100 // The number of times we get the same reading before we light an LED

// TIMEOUT
#define SPI_TIMEOUT                 100 // timeout in microseconds
#define I2C_TIMEOUT                 100 // timeout in microseconds

#define I2C_ENC true

// AS5048 ADDRESSES
#define AS5048_7BIT_ADDRESS         0x40 // 0b10000 + ( A1 & A2 to GND) -> 0b1000000
#define AS5048_7BIT_ADDRESS_SPI     0x40//0x42 // 0b1000010 // 0b1000011 = 0x43 //0x10 // 0b00100 -> 0b0010000 0x10 -> 0x20 0x21
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
void ledsPattern(int led1, int led2, int led3, int led4);
int checkTimeout(int utime, int initialTime);

// SPI
int readEncoderSPI(SPI_TypeDef* spiPort, uint8_t encoderNumber, int timeout);

// I2C
int readEncoderI2C(I2C_TypeDef* i2cPort, uint8_t encoderNumber, int timeout);

// GLOBAL VARIABLES
enc_async_poll_state_t enc_poll_state = EPS_DONE;

void enc_init()
{
  // change encoder I2C address
  // uint8_t data[2] = {0x15, 0x04}; // 0b0000100 = 0x04 0b0010000 = 0x10 
  // writeBytesSPI(SPI1, AS5048_7BIT_ADDRESS, data, 2, 0);

  // initializing encoders state
  for (int i = 0; i < NUM_ENC; i++)
  {
    g_state.encoders[i] = 0;
  }
}

/*
void enc_poll_nonblocking_tick(const uint8_t bogus __attribute__((unused)))
  Description: Reads encoders values.

  Returns: void

  Encoders connections:
    0 -> Port I2C1
    1 -> Port SPI
    2 -> Port I2C3
*/
void enc_poll_nonblocking_tick(const uint8_t bogus __attribute__((unused)))
{
  static uint8_t i2cPort1Updated = 0;
  static uint8_t i2cPort3Updated = 0;
  int result;

  switch(enc_poll_state)
  {
    case EPS_DONE:
        // result = readEncoderSPI(SPI1, 1, SPI_TIMEOUT); // (spiPort, encoderNumber, timeout)
        
        if (isBusyI2CPort(I2C3) && isBusyI2CPort(I2C1))
        {
          enc_poll_state = EPS_I2C;
          i2cPort1Updated = 0;
          i2cPort3Updated = 0;
        }
        else if (!isBusyI2CPort(I2C3) && !isBusyI2CPort(I2C1))
        {
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          result = readEncoderI2C(I2C3, 2, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        } 
        else if (!isBusyI2CPort(I2C1))
        {
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        }
        else if (!isBusyI2CPort(I2C3))
        {
          result = readEncoderI2C(I2C3, 2, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        }      
      break;
    case EPS_I2C:
        if (!isBusyI2CPort(I2C3) && !isBusyI2CPort(I2C1))
        {
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          result = readEncoderI2C(I2C3, 2, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        } 
        else if (!isBusyI2CPort(I2C1) && i2cPort1Updated == 0)
        {
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          i2cPort1Updated = 1;
          if (i2cPort3Updated == 1)
            enc_poll_state = EPS_DONE;
        }
        else if (!isBusyI2CPort(I2C3) && i2cPort3Updated == 0)
        {
          result = readEncoderI2C(I2C3, 2, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          i2cPort3Updated = 1;
          if (i2cPort1Updated == 1)
            enc_poll_state = EPS_DONE;
        }
      break;
    default:
      enc_poll_state = EPS_DONE; // shouldn't get here
      break;

      printf("%d\n", result); // rmelo19, to correct, change to error checking.
  }
}

int checkTimeout(int utime, int initialTime)
{
 return (SYSTIME - initialTime > utime);
}

int readEncoderI2C(I2C_TypeDef* i2cPort, uint8_t encoderNumber, int timeout)
{
  writeRegisterI2C(i2cPort, AS5048_7BIT_ADDRESS, AS5048B_ANGLLSB_REG);
  uint8_t valueRead[2];
  readBytesI2C(i2cPort, AS5048_7BIT_ADDRESS, 2, valueRead);
  g_state.encoders[encoderNumber] = (((uint16_t) valueRead[0] << 6) + ((uint16_t) (valueRead[1] & 0x3F))); 

  return 0;
}

int readEncoderSPI(SPI_TypeDef* spiPort, uint8_t encoderNumber, int timeout)
{
  writeRegisterSPI(spiPort, AS5048_7BIT_ADDRESS_SPI, AS5048B_ANGLLSB_REG);
  uint8_t valueRead[2];
  readBytesSPI(spiPort, AS5048_7BIT_ADDRESS_SPI, 2, valueRead);
  g_state.encoders[encoderNumber] = (((uint16_t) valueRead[0] << 6) + ((uint16_t) (valueRead[1] & 0x3F)));

  return 0;
}
#include "enc.h"

// GLOBAL VARIABLES
enc_async_poll_state_t enc_poll_state = EPS_DONE;

void encInit()
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
        result = readEncoderSPI(SPI1, 1, SPI_TIMEOUT); // (spiPort, encoderNumber, timeout)
        
        if (isBusyI2CPort(I2C3) && isBusyI2CPort(I2C1))
        {
          printf("neither 0 nor 2\n");
          enc_poll_state = EPS_I2C;
          i2cPort1Updated = 0;
          i2cPort3Updated = 0;
        }
        else if (!isBusyI2CPort(I2C3) && !isBusyI2CPort(I2C1))
        {
          printf("0 and 2, EPS_DONE\n");
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          printf("Second\n");
          result = readEncoderI2C(I2C3, 2, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        } 
        else if (!isBusyI2CPort(I2C1))
        {
          printf("0\n");
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        }
        else if (!isBusyI2CPort(I2C3))
        {
          printf("2\n");
          result = readEncoderI2C(I2C3, 2, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        }      
      break;
    case EPS_I2C:
        if (!isBusyI2CPort(I2C3) && !isBusyI2CPort(I2C1))
        {
          printf("0 and 2, EPS_I2C\n");
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          result = readEncoderI2C(I2C3, 2, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          enc_poll_state = EPS_DONE;
        } 
        else if (!isBusyI2CPort(I2C1) && i2cPort1Updated == 0)
        {
          printf("0\n");
          result = readEncoderI2C(I2C1, 0, I2C_TIMEOUT);  // (i2cPort, encoderNumber, timeout)
          i2cPort1Updated = 1;
          if (i2cPort3Updated == 1)
            enc_poll_state = EPS_DONE;
        }
        else if (!isBusyI2CPort(I2C3) && i2cPort3Updated == 0)
        {
          printf("2\n");
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
  printf("Encoder %d: ", encoderNumber);
  writeRegisterI2C(i2cPort, AS5048_7BIT_ADDRESS, AS5048B_ANGLLSB_REG);
  uint8_t valueRead[2];
  readBytesI2C(i2cPort, AS5048_7BIT_ADDRESS, 2, valueRead);
  g_state.encoders[encoderNumber] = (((uint16_t) valueRead[0] << 6) + ((uint16_t) (valueRead[1] & 0x3F))); 
  printf("%d\n" , g_state.encoders[encoderNumber]);


  return 0;
}

int readEncoderSPI(SPI_TypeDef* spiPort, uint8_t encoderNumber, int timeout)
{
  printf("Encoder %d: ", encoderNumber);
  writeRegisterSPI(spiPort, AS5048_7BIT_ADDRESS_SPI, AS5048B_ANGLLSB_REG);
  uint8_t valueRead[2];
  readBytesSPI(spiPort, AS5048_7BIT_ADDRESS_SPI, 2, valueRead);
  g_state.encoders[encoderNumber] = (((uint16_t) valueRead[0] << 6) + ((uint16_t) (valueRead[1] & 0x3F)));
  printf("%d\n", g_state.encoders[encoderNumber]);

  return 0;
}
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

#include "config.h"
#include "pin.h"

// FUNCTION PROTOTYPES
void configurePorts(void);

int main()
{
  // general initialization
  configurePorts();

  console_init();
  systime_init();
  leds_init();
  enet_init();
  dmxl_init();
  fan_init();
  tactile_init();
  enc_init();
  state_init();
  async_poll_init();
  fan_on(); // todo: be smarter. probably doesn't need to run all the time.
  __enable_irq();

  dmxl_set_baud_rates();
  dmxl_set_status_return_levels();

  volatile uint32_t prev_start_time = SYSTIME;
  uint_fast8_t poll_cycles_to_skip = 0;

  for (uint_fast32_t loop_count = 1; ; loop_count++)
  {
    err_service();
    if (SYSTIME - prev_start_time >= POLL_PERIOD_US)
    {
      prev_start_time += POLL_PERIOD_US;
      if (poll_cycles_to_skip > 0)
        poll_cycles_to_skip--;
      else
      {
        g_state.systime = SYSTIME;
        async_poll_start();
      }
    }
    const async_poll_tick_result_t aptr = async_poll_tick();
    if (aptr == APT_JUST_FINISHED)
    {
      if (enet_get_link_status() == ENET_LINK_UP)
      {
        err_unset(ERR_NO_ETHERNET);
        enet_send_state();
      }
      else
      {
        err_set(ERR_NO_ETHERNET);
      }
      enet_process_rx_ring();
    }
  }
  return 0;
}

void configurePorts(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
                  RCC_AHB1ENR_GPIOBEN |
                  RCC_AHB1ENR_GPIOCEN |
                  RCC_AHB1ENR_GPIODEN;
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN |
                  RCC_APB1ENR_I2C3EN |
                  RCC_APB1ENR_SPI2EN;
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  pin_set_alternate_function(GPIOB, PORTB_I2C1_SCL, 4);
  pin_set_alternate_function(GPIOB, PORTB_I2C1_SDA, 4);
  pin_set_output_type(GPIOB, PORTB_I2C1_SCL, PIN_OUTPUT_TYPE_OPEN_DRAIN);
  pin_set_output_type(GPIOB, PORTB_I2C1_SDA, PIN_OUTPUT_TYPE_OPEN_DRAIN);

  pin_set_alternate_function(GPIOA, PORTA_I2C3_SCL, 4);
  pin_set_alternate_function(GPIOC, PORTC_I2C3_SDA, 4);
  pin_set_output_type(GPIOA, PORTA_I2C3_SCL, PIN_OUTPUT_TYPE_OPEN_DRAIN);
  pin_set_output_type(GPIOC, PORTC_I2C3_SDA, PIN_OUTPUT_TYPE_OPEN_DRAIN);

  //I2C1->CCR |= I2C_CCR_FS | // set fast mode
  //             35; // 42 MHz / (3 * 400 kHz) == 35
  //I2C1->TRISE = 42 * 300 / 1000 + 1; // not sure here.
  I2C1->CR2   |= APB_MHZ; // 42MHz -> 101010
  I2C1->CCR   |= I2C_CCR; // I2C_CCR = 210
  I2C1->TRISE &= ~0x3f; // 0x3f = 0b00111111 -> 0b11000000, clearing register
  I2C1->TRISE |= I2C_TRISE; // = I2C_TRISE = (APB_MHZ * 200 / 1000 + 1), maximum rise time
  I2C1->CR1   |= I2C_CR1_PE; // peripheral enable

  I2C3->CR2   |= APB_MHZ;
  I2C3->CCR   |= I2C_CCR;
  I2C3->TRISE &= ~0x3f;
  I2C3->TRISE |= I2C_TRISE;
  I2C3->CR1   |= I2C_CR1_PE;

  // now, set up the spi-to-i2c bridges
  pin_set_output(GPIOC, PORTC_I2C_BRIDGE_RESET);
  pin_set_output(GPIOA, PORTA_BRIDGE0_CS);
  pin_set_output(GPIOB, PORTB_BRIDGE1_CS);
  pin_set_output_level(GPIOA, PORTA_BRIDGE0_CS, 1);
  pin_set_output_level(GPIOA, PORTB_BRIDGE1_CS, 1);

  pin_set_alternate_function(GPIOA, PORTA_BRIDGE0_MISO, 5);
  pin_set_alternate_function(GPIOB, PORTB_BRIDGE0_MOSI, 5);
  pin_set_alternate_function(GPIOA, PORTA_BRIDGE0_SCLK, 5);
  pin_set_alternate_function(GPIOC, PORTC_BRIDGE1_MISO, 5);
  pin_set_alternate_function(GPIOC, PORTC_BRIDGE1_MOSI, 5);
  pin_set_alternate_function(GPIOD, PORTD_BRIDGE1_SCLK, 5);

  // SPI1
  // running from a 84 MHz pclk. set it up with
  // sclk = pclk/64 to stay within datasheet limits.
  SPI1->CR1 = SPI_CR1_BR_2 | // sclk = pclk/64
              SPI_CR1_BR_0 | // sclk = pclk/64
              SPI_CR1_MSTR | // master mode
              SPI_CR1_CPOL | // clock to 1 when idle
              SPI_CR1_CPHA | // the second clock transition is the first data capture edge
              SPI_CR1_SSM  | // software slave-select mode
              SPI_CR1_SSI  | // the value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored.
              SPI_CR1_SPE;   // SPI enable

  // bit rate = 42 mhz / 32 = 1.313 MHz
  SPI2->CR1 = SPI_CR1_BR_2 | // sclk = pclk/32
              SPI_CR1_MSTR | // master mode
              SPI_CR1_CPOL | // clock to 1 when idle
              SPI_CR1_CPHA | // the second clock transition is the first data capture edge
              SPI_CR1_SSM  | // software slave-select mode
              SPI_CR1_SSI  | // the value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored.
              SPI_CR1_SPE;   // SPI enable
}


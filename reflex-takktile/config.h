
//#define POLL_PERIOD_US 33333
#define POLL_PERIOD_US 25000

/////////////////////////////////////////////////////////////////////////
// I2C SETUP
//
// i2c1 scl = pb6, alternate function 4
// i2c1 sda = pb7, alternate function 4
// i2c3 scl = pa8, alternate function 4
// i2c3 sda = pc9, alternate function 4
#define PORTB_I2C1_SCL                6
#define PORTB_I2C1_SDA                7
#define PORTA_I2C3_SCL                8
#define PORTC_I2C3_SDA                9

/////////////////////////////////////////////////////////////////////////
// SPI-TO-I2C BRIDGES SETUP
//
// bridge shared reset = pc14
// bridge0 cs = pa4
// bridge0 miso = pa6 via SPI1 on AF 5
// bridge0 mosi = pb5 via SPI1 on AF 5
// bridge0 sclk = pa5 via SPI1 on AF 5
// bridge0 int = pc15
// bridge1 cs = pb9
// bridge1 miso = pc2 via SPI2 on AF 5
// bridge1 mosi = pc3 via SPI2 on AF 5
// bridge1 sclk = pd3 via SPI2 on AF 5
// bridge1 int = pa0
#define PORTC_I2C_BRIDGE_RESET         14
#define PORTA_BRIDGE0_CS               4
#define PORTA_BRIDGE0_MISO             6
#define PORTB_BRIDGE0_MOSI             5
#define PORTA_BRIDGE0_SCLK             5
#define PORTC_BRIDGE0_INT              15
#define PORTB_BRIDGE1_CS               9
#define PORTC_BRIDGE1_MISO             2
#define PORTC_BRIDGE1_MOSI             3
#define PORTD_BRIDGE1_SCLK             3
#define PORTA_BRIDGE1_INT              0

#define BAROM_ADDR                     0xC0
#define BCAST_ENABLE_ADDR              0x0C
#define BCAST_DISABLE_ADDR             0x0D


#define APB_MHZ                        42 // our APB frequency is 42 mhz

// for 100 kHz i2c: 42 mhz / (2 * 100 khz) = 210
// for 400 kHz i2c: 42 mhz / (2 * 400 khz) =  53
#define I2C_CCR                        210
#define I2C_TRISE                      (APB_MHZ * 200 / 1000 + 1)
#define I2C_READ                       1
## U8G2 Kendryte SDK (K210) Hardware Abstraction Layer

This project a port of the u8g2 HAL for the Kendryte SDK Platform (K210 Chipset). This HAL makes use of the DMAC (Direct Memory Access Controller) Channels available on the K210 for high speed data transfer (This can be disabled optionally). 


## Development Status
Currently, the hardware I2C is available for usage. You can use FPIOA to select any two pins to be the SDA & SCL for the I2C Bus.
u8g2 cb functions for I2C SW Bit banging & SPI (3-wire & 4-wire) have not been implemented yet.

## Usage

To use this u8g2 HAL, add this repository as a submodule to your `kendryte-standalone-sdk` project and include the sources files (including headers) in your project cmake files.
Once you have `u8g2_kendryte_hal.h` & `u8g2.h` available in your main application file, you can initialize your u8g2 supported display device, the `u8g2_kendryte_hal_t` object and `u8g2_t` object.

alternatively, you can clone my fork of `kendryte-standalone-sdk` that contains the `u8g2_kendryte_hal` and other useful drivers & plugins for the K210 Platform from here:

https://github.com/sreedevk/kendryte-sdk-dev

This repository on the above link is a template repository.
make sure that you clone the repo along with its' submodules to ensure that you have all the drivers available:

```bash
  git clone https://github.com/sreedevk/kendryte-sdk-dev --recurse-submodules
```

## Example Usage

This code was used to test SSD1306 128x32 i2c interface oled display on the MAix Bit development board.

```c
#include <stdbool.h>
#include <stdlib.h>
#include <sleep.h>
#include <syslog.h>
#include <math.h>
#include <u8g2.h>
#include <u8g2_kendryte_hal.h>

#define U8G2_SW_I2C_SCL_PIN 32
#define U8G2_SW_I2C_SDA_PIN 34
#define U8G2_SW_I2C_ADDRESS 0x78

u8g2_t display_handle;

void initialize_oled_display(u8g2_t *oled_display);
void draw_sine_wave(u8g2_t *u8g2);

int main() {
  initialize_oled_display(&display_handle);
  draw_sine_wave(&display_handle);
  while(true) sleep(1);
}

void draw_sine_wave(u8g2_t *u8g2) {
  double amplitude = 8;
  double y_shift   = 16;
  double frequency = 0.2; /* in kilohertz */
  double wave_animate_offset = 1;
  while(true) {
    u8g2_ClearBuffer(u8g2);
    for(uint8_t x=0; x < 128; x++) {
      u8g2_DrawPixel(u8g2, x, amplitude * sin((x+wave_animate_offset) * frequency) + y_shift);
    }
    u8g2_SendBuffer(u8g2);
    wave_animate_offset++;
  }
}

void initialize_oled_display(u8g2_t *oled_display) {
  /* configure & initialize u8g2_kendryte_hal */
  u8g2_kendryte_hal_t u8g2_hal_config = u8g2_kendryte_hal_configure_hw_i2c(U8G2_SW_I2C_SDA_PIN, U8G2_SW_I2C_SCL_PIN);
  u8g2_kendryte_hal_init(u8g2_hal_config);

  /* initialize u8g2 */
  u8g2_Setup_ssd1306_i2c_128x32_univision_f(oled_display, U8G2_R0, kendryte_u8x8_byte_hw_i2c, kendryte_u8x8_gpio_and_delay);
  u8x8_SetI2CAddress(&(oled_display->u8x8), U8G2_SW_I2C_ADDRESS);
  u8g2_InitDisplay(oled_display);
  u8g2_SetPowerSave(oled_display, 0);
  u8g2_ClearBuffer(oled_display);
  u8g2_SendBuffer(&display_handle);
}
```

### Result:
![u8g2_kendryte_example_result](https://media.giphy.com/media/y8ljSQdLt1VP0hu5l4/giphy.gif)


## Configuration

```c
typedef struct {
  /* i2c */
  int8_t sda;         /* has no default value - is set through the u8g2_kendryte_hal_configure_hw_i2c function */
  int8_t scl;         /* has no default value - is set through the u8g2_kendryte_hal_configure_hw_i2c function */
  bool   use_hw_i2c;  /* default value is false, but is set to true through u8g2_kendryte_hal_configure_hw_i2c */
  bool   use_dmac;    /* default value is true. you can manually alter this if you do not wish to use the DMAC channels for data transactions */
  int i2c_bus_freq;   /* default value is 400KHz you can change that as per your requirements */
  dmac_channel_number_t tx_dmac_channel; /* if DMAC is enabled, default value is DMAC_CHANNEL0 */
  dmac_channel_number_t rx_dmac_channel; /* if DMAC is enabled, default value is DMAC_CHANNEL1 */
  i2c_device_number_t   i2c_device_number; /* default value is I2C_DEVICE_0 */

  /* spi */
  int8_t sck;
  int8_t mosi;
  int8_t miso;
  int8_t reset;
  int8_t dc;
  int8_t cs;
  bool use_hw_spi;

  /* optional */
  int8_t power;
} u8g2_kendryte_hal_t;
```

as shown in the example usage above, the following function can be used to initialize `u8g2_kendryte_hal_t` with all the default options.
```c
  u8g2_kendryte_hal_t u8g2_hal_config = u8g2_kendryte_hal_configure_hw_i2c(U8G2_SW_I2C_SDA_PIN, U8G2_SW_I2C_SCL_PIN);
```

after calling the above functions, you can manually alter the struct values before calling the `u8g2_kendryte_hal_init` function. for example, I can disable DMAC usage by:

```c
  u8g2_hal_config.use_dmac = false;
```

once the `u8g2_kendryte_hal_t` struct has been configured as per your requirements, you can initialize the HAL using the following function passing in your `u8g2_kendryte_hal_t` to the function.

```c
  u8g2_kendryte_hal_init(u8g2_hal_config);
```

After initialization of the HAL, you can go about the normal initialization process for u8g2 as follows:

```c
  u8g2_Setup_ssd1306_i2c_128x32_univision_f(oled_display, U8G2_R0, kendryte_u8x8_byte_hw_i2c, kendryte_u8x8_gpio_and_delay);
  u8x8_SetI2CAddress(&(oled_display->u8x8), U8G2_SW_I2C_ADDRESS);
  u8g2_InitDisplay(oled_display);
  u8g2_SetPowerSave(oled_display, 0);
  u8g2_ClearBuffer(oled_display);
  u8g2_SendBuffer(&display_handle);
```

please note that `kendryte_u8x8_byte_hw_i2c` & `kendryte_u8x8_gpio_and_delay` functions are the functions defined by the library to u8g2 to communicate
with your display peripherals.

more such functions will be added in future implementing 3-wire SPI, 2-wire SPI & the u8g2 bit-banging implementation of all these protocols.


## K210 Boards Tested
1. Sipeed MAix Bit
2. Sipeed MAixduino

## U8G2 Supported Displays Tested
1. SSD1306 (128x32) I2C Oled Display
2. SH1106  (128x64) I2C Oled Display


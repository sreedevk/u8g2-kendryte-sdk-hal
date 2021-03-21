## Example Usage

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

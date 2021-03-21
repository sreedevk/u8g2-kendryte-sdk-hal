/* MIT License
 * 
 * Copyright (c) 2021 Sreedev Kodichath
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <u8g2.h>
#include <stdint.h>
#include <stdbool.h>
#include <dmac.h>
#include <i2c.h>

#define DEFAULT_DMAC_STATE          true
#define DEFAULT_DMAC_TX_CHANNEL     DMAC_CHANNEL0
#define DEFAULT_DMAC_RX_CHANNEL     DMAC_CHANNEL1
#define DEFAULT_I2C_DEVICE_NUM      I2C_DEVICE_0
#define OLED_I2C_BUS_DEFAULT_FREQ   400000 /* 4 KHz */

#define INIT_U8G2_KENDRYTE_HAL(x) u8g2_kendryte_hal_t x = { \
  /* I2C CONFIG */            \
  .sda               = -1,    \
  .scl               = -1,    \
  .use_hw_i2c        = false, \
  .i2c_bus_freq      = OLED_I2C_BUS_DEFAULT_FREQ,  \
  .use_dmac          = DEFAULT_DMAC_STATE,         \
  .tx_dmac_channel   = DEFAULT_DMAC_TX_CHANNEL,    \
  .rx_dmac_channel   = DEFAULT_DMAC_RX_CHANNEL,    \
  .i2c_device_number = DEFAULT_I2C_DEVICE_NUM,     \
  /* SPI CONFIG*/            \
  .sck               = -1,   \
  .mosi              = -1,   \
  .miso              = -1,   \
  .reset             = -1,   \
  .dc                = -1,   \
  .cs                = -1,   \
  .use_hw_spi        = false \
};


typedef struct {
  /* i2c */
  int8_t sda;
  int8_t scl;
  bool   use_hw_i2c;
  bool   use_dmac;
  int i2c_bus_freq;
  dmac_channel_number_t tx_dmac_channel;
  dmac_channel_number_t rx_dmac_channel;
  i2c_device_number_t   i2c_device_number;

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

u8g2_kendryte_hal_t u8g2_kendryte_hal_configure_hw_i2c(int8_t i2c_sda, int8_t i2c_scl);
u8g2_kendryte_hal_t u8g2_kendryte_hal_configure_sw_i2c(int8_t i2c_sda, int8_t i2c_scl);
void                u8g2_kendryte_hal_init(u8g2_kendryte_hal_t kendryte_hal_param);
uint8_t             kendryte_u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t             kendryte_u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


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

#include <u8g2_kendryte_hal.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <i2c.h>
#include <fpioa.h>
#include <gpiohs.h>
#include <sleep.h>
#include <syslog.h>
#include <u8g2.h>
#include <dmac.h>

/*               DEBUG INFO
 * +=====================================+
 * |      U8G2 FUNCTION NUM MAPPING      |
 * +=====================================+
 * |    FUNC(GPIO_AND_DELAY)       | MSG |
 * +-------------------------------------+
 * | U8X8_MSG_GPIO_AND_DELAY_INIT  | 40  |
 * | U8X8_PIN_RESET                | 75  | 
 * | U8X8_MSG_DELAY_NANO           | 44  |
 * | U8X8_MSG_DELAY_100NANO        | 43  |
 * | U8X8_MSG_DELAY_10MICRO        | 42  |
 * | U8X8_MSG_DELAY_MILLI          | 41  |
 * | U8X8_MSG_DELAY_I2C            | 45  |
 * | U8X8_MSG_GPIO_I2C_CLOCK       | 76  |
 * | U8X8_MSG_GPIO_I2C_DATA        | 77  |
 * |-------------------------------------|
 * |     FUNC(BYTE_TRANSFER)       | MSG |
 * |-------------------------------------|
 * | U8X8_MSG_BYTE_SEND            | 23  |
 * | U8X8_MSG_BYTE_INIT            | 20  |
 * | U8X8_MSG_BYTE_START_TRANSFER  | 24  |
 * | U8X8_MSG_BYTE_END_TRANSFER    | 25  |
 * +===============================+=====+
 */

static u8g2_kendryte_hal_t kendryte_hal;

void u8g2_kendryte_hal_init(u8g2_kendryte_hal_t kendryte_hal_param) {
  kendryte_hal = kendryte_hal_param;
}

u8g2_kendryte_hal_t u8g2_kendryte_hal_configure_hw_i2c(int8_t i2c_sda, int8_t i2c_scl) {
  INIT_U8G2_KENDRYTE_HAL(kendryte_hal_obj);
  kendryte_hal_obj.sda          = i2c_sda;
  kendryte_hal_obj.scl          = i2c_scl;
  kendryte_hal_obj.use_hw_i2c   = true;
  return kendryte_hal_obj;
}

u8g2_kendryte_hal_t u8g2_kendryte_hal_configure_sw_i2c(int8_t i2c_sda, int8_t i2c_scl) { 
  INIT_U8G2_KENDRYTE_HAL(kendryte_hal_obj);
  kendryte_hal_obj.sda        = i2c_sda;
  kendryte_hal_obj.scl        = i2c_scl;
  kendryte_hal_obj.use_hw_i2c = false;
  return kendryte_hal_obj;
}

/* calculate delay period based on frequency in KHz */
double delay_period_in_usec(uint8_t frequency) {
  return (double) (10/(2 * ((double) frequency)));
}

uint8_t kendryte_u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  switch(msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      /* called once during the initialization phase of u8g2/u8x8 */
      if(!kendryte_hal.use_hw_i2c) {
        fpioa_set_function(kendryte_hal.scl, FUNC_GPIOHS1);
        fpioa_set_function(kendryte_hal.sda, FUNC_GPIOHS2);
        gpiohs_set_drive_mode(fpioa_get_io_by_function(FUNC_GPIOHS1), GPIO_DM_OUTPUT);
        gpiohs_set_drive_mode(fpioa_get_io_by_function(FUNC_GPIOHS2), GPIO_DM_OUTPUT);
      }
      break; 
    case U8X8_MSG_DELAY_NANO:
      usleep(arg_int/1000);
      break;
    case U8X8_MSG_DELAY_100NANO:
      usleep((arg_int * 100)/1000);
      break;
    case U8X8_MSG_DELAY_10MICRO:
      usleep(arg_int * 10);
      break;
    case U8X8_MSG_DELAY_MILLI:
      msleep(arg_int);
      break;
    case U8X8_MSG_DELAY_I2C:
      usleep(delay_period_in_usec(arg_int)); 
      break;							
    case U8X8_MSG_GPIO_D0: /* aka U8X8_MSG_GPIO_SPI_CLOCK */
      break;
    case U8X8_MSG_GPIO_D1:
      break;
    case U8X8_MSG_GPIO_D2:
      break;
    case U8X8_MSG_GPIO_D3:
      break;
    case U8X8_MSG_GPIO_D4:
      break;
    case U8X8_MSG_GPIO_D5:
      break;
    case U8X8_MSG_GPIO_D6:
      break;
    case U8X8_MSG_GPIO_D7:
      break;
    case U8X8_MSG_GPIO_E:
      break;
    case U8X8_MSG_GPIO_CS:
      break;
    case U8X8_MSG_GPIO_DC:
      break;
    case U8X8_MSG_GPIO_RESET:
      break;
    case U8X8_MSG_GPIO_CS1:
      break;
    case U8X8_MSG_GPIO_CS2:
      break;
    case U8X8_MSG_GPIO_I2C_CLOCK:
      break;
    case U8X8_MSG_GPIO_I2C_DATA:
      break;
    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
      break;
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}

uint8_t kendryte_u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  static uint8_t buffer[32];
  static uint8_t buffer_index;

  uint8_t ack_byte;
  uint8_t *data;

  switch(msg) {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *) arg_ptr;
      while(arg_int > 0) {
        buffer[buffer_index++] = *data++;
        arg_int--;
      }
      break;
    case U8X8_MSG_BYTE_INIT:
      /* custom code for i2c subsystem initialization */
      if(kendryte_hal.use_hw_i2c) {
        fpioa_set_function(kendryte_hal.scl, FUNC_I2C0_SCLK);
        fpioa_set_function(kendryte_hal.sda, FUNC_I2C0_SDA);
        dmac_init();
        i2c_init(kendryte_hal.i2c_device_number, u8x8_GetI2CAddress(u8x8) >> 1, 7, kendryte_hal.i2c_bus_freq);
      }
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      buffer_index = 0;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      ack_byte = 0;
      if(kendryte_hal.use_dmac) {
        i2c_recv_data_dma(
            kendryte_hal.tx_dmac_channel,
            kendryte_hal.rx_dmac_channel,
            kendryte_hal.i2c_device_number,
            buffer,
            buffer_index,
            &ack_byte,
            1
        );
      }
      else {
        i2c_recv_data(
            kendryte_hal.i2c_device_number,
            buffer,
            buffer_index,
            &ack_byte,
            1
        );
      }
      break;
    default:
      return 0;
  }
  return 1;
}

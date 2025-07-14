#ifndef I2C_KEYBOARD_H
#define I2C_KEYBOARD_H
#include <pico/stdlib.h>
#include <pico/platform.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>

#define I2C_KBD_MOD i2c1
#define I2C_KBD_SDA 6
#define I2C_KBD_SCL 7

#define I2C_KBD_SPEED  10000 // if dual i2c, then the speed of keyboard i2c should be 10khz

#define I2C_KBD_ADDR 0x1F

void i2c_kbd_init();
int i2c_kbd_read();
int i2c_kbd_read_battery();
int i2c_kbd_set_backlight(uint8_t);

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitClear(value, bit) ((value) &= ~(1 << (bit)))
#define bitSet(value, bit) ((value) |= (1 << (bit)))

#endif
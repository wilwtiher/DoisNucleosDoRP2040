#ifndef VL53L0X_SENSOR_H
#define VL53L0X_SENSOR_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Endereço I2C padrão do VL53L0X
#define VL53L0X_ADDRESS 0x29

// Funções para interagir com o sensor
void vl53l0x_init(i2c_inst_t* i2c);
uint16_t vl53l0x_read_range(i2c_inst_t* i2c);

#endif
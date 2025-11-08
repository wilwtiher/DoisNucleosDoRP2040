#include "vl53l0x_sensor.h"
#include <unistd.h>

// Funções internas para leitura e escrita de registradores
static void write_byte(i2c_inst_t* i2c, uint8_t reg, uint8_t data) {
    uint8_t buffer[2] = {reg, data};
    i2c_write_blocking(i2c, VL53L0X_ADDRESS, buffer, 2, false);
}

static uint8_t read_byte(i2c_inst_t* i2c, uint8_t reg) {
    uint8_t data;
    i2c_write_blocking(i2c, VL53L0X_ADDRESS, &reg, 1, true);
    i2c_read_blocking(i2c, VL53L0X_ADDRESS, &data, 1, false);
    return data;
}

// Inicialização básica do sensor
void vl53l0x_init(i2c_inst_t* i2c) {
    // Uma inicialização mais robusta envolveria uma sequência complexa de escritas
    // de registradores, conforme o datasheet. Para simplicidade, usamos uma sequência mínima.
    write_byte(i2c, 0x88, 0x00);
    write_byte(i2c, 0x80, 0x01);
    write_byte(i2c, 0xFF, 0x01);
    write_byte(i2c, 0x00, 0x00);
    uint8_t val = read_byte(i2c, 0x00);
    write_byte(i2c, 0x00, 0x01);
    write_byte(i2c, 0xFF, 0x00);
    write_byte(i2c, 0x80, 0x00);
}

// Leitura da distância em milímetros
uint16_t vl53l0x_read_range(i2c_inst_t* i2c) {
    write_byte(i2c, 0x00, 0x01);
    uint8_t val = 0;
    while (!(read_byte(i2c, 0x14) & 0x01)) {
        sleep_ms(10);
    }

    uint8_t range_data[2];
    i2c_write_blocking(i2c, VL53L0X_ADDRESS, (uint8_t[]){0x1E}, 1, true);
    i2c_read_blocking(i2c, VL53L0X_ADDRESS, range_data, 2, false);

    return (range_data[0] << 8) | range_data[1];
}
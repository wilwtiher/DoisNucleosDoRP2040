#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"
#include "vl53l0x_sensor.h" // Inclui o novo driver do sensor
#include "bh1750_light_sensor.h"
#include "ssd1306.h"
#include "font.h"
#include <math.h>

// Para o sensor de distância VL53L0X. Endereço 0x29
#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

// Para o sensor de luz BH1750. Endereço 0x23
#define I2C_PORT2 i2c1               // i2c0 pinos 0 e 1, i2c1 pinos 2 e 3
#define I2C_SDA2 2                   // 0 ou 2
#define I2C_SCL2 3                   // 1 ou 3

// Display na I2C
#define I2C_PORT_DISP i2c1
#define I2C_SDA_DISP 14
#define I2C_SCL_DISP 15
#define endereco 0x3C

#define LED_VERDE 11 // GPIO do LED Verde
#define LED_AZUL 12  // GPIO do LED Azul

char str_dist[16];
char str_lux[10];
uint16_t distance = 0;
uint16_t lux = 0;

// Função executada no Core 1 (piscando o LED Azul)
void core1_entry()
{
    while (1)
    {
        // Leitura do sensor de Distância VL53L0X
        distance = vl53l0x_read_range(I2C_PORT);
        printf("Distancia = %d mm\n", distance);

        sprintf(str_dist, "%d mm", distance);


        // Leitura do sensor de Luz BH1750
        lux = bh1750_read_measurement(I2C_PORT2);
        printf("Lux = %d\n", lux);

 
        sprintf(str_lux, "%d Lux", lux);  // Converte o inteiro em string
        sleep_ms(1);
    }
}

void inicBH1750(){
    gpio_pull_up(I2C_SDA2);
    gpio_pull_up(I2C_SCL2);
    // Inicializa o sensor de luz BH1750
    bh1750_power_on(I2C_PORT2);
}

// I2C0 para o sensor VL53L0X
void inicVL53L0X(){
    i2c_init(I2C_PORT, 100 * 1000); // VL53L0X pode ser mais lento
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // Inicializa o sensor VL53L0X
    vl53l0x_init(I2C_PORT);
}

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

int main()
{
    // Para ser utilizado o modo BOOTSEL com botão B
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);

    // Inicia o Core 1
    multicore_launch_core1(core1_entry);

    stdio_init_all();

    
    i2c_init(I2C_PORT_DISP, 400 * 1000);
    gpio_set_function(I2C_SDA_DISP, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_DISP, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_DISP);
    gpio_pull_up(I2C_SCL_DISP);
    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT_DISP);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    inicVL53L0X();
    inicBH1750();


    while (1)
    {
        // Atualiza o display
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);
        ssd1306_draw_string(&ssd, "Sensor VL53L0X:", 0, 0);
        ssd1306_draw_string(&ssd, str_dist, 0, 16);
        ssd1306_draw_string(&ssd, "Sensor  BH1750:", 0, 32);// Desenha uma string
        ssd1306_draw_string(&ssd, str_lux, 0, 48);         // Desenha uma string
        ssd1306_send_data(&ssd);

        if(distance < 100){
           gpio_put(LED_VERDE, 1); 
        }else{
            gpio_put(LED_VERDE, 0); 
        }
        if(lux < 40){
           gpio_put(LED_AZUL, 1); 
        }else{
            gpio_put(LED_AZUL, 0); 
        }
        sleep_ms(1);
    }
}
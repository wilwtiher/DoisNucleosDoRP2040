# 🚀 BitDogLab: Monitor de Sensores com Dual Core (RP2040)

<div align="center">

</div>

Um projeto para a placa BitDogLab que demonstra o uso eficiente dos dois núcleos (Core 0 e Core 1) do Raspberry Pi Pico (RP2040). O sistema divide as tarefas para garantir leituras de sensores e atualizações de interface sem bloqueios mútuos.

Este projeto atende aos requisitos da tarefa de "Desenvolvimento de sensores e atuadores IoT" , focando na utilização de processamento paralelo .

-----

### 📝 Descrição Breve

Este projeto implementa um sistema de monitoramento que utiliza os dois núcleos do RP2040 de forma independente:

  * **Núcleo 1 (Core 1):** Fica inteiramente responsável pela **aquisição contínua de dados** . Ele realiza leituras constantes de um sensor de distância (VL53L0X) e um sensor de luz (BH1750) .
  * **Núcleo 0 (Core 0):** Atua como a **Interface de Usuário** . Ele é responsável por receber os dados do Core 1 e usá-los para atualizar um display OLED SSD1306 e controlar LEDs de status .

A comunicação entre os núcleos é feita através de variáveis globais, permitindo que o Core 0 sempre tenha os dados mais recentes dos sensores para exibir, sem nunca precisar esperar por uma leitura de sensor .

-----

### ✨ Funcionalidades Principais

  * **✅ Processamento Paralelo (Multicore):** O Core 1 é dedicado 100% à leitura de sensores, enquanto o Core 0 é dedicado 100% à atualização da interface, demonstrando o paralelismo real sem bloqueios .
  * **✅ Leitura de Sensores Múltiplos:** Coleta dados de dois sensores I2C diferentes em barramentos separados (distância e luminosidade).
  * **✅ Interface de Usuário Responsiva:** O display OLED é atualizado em alta velocidade pelo Core 0, mostrando os dados mais recentes lidos pelo Core 1.
  * **✅ Feedback Visual com LEDs:** Os LEDs da placa (Verde e Azul) são controlados pelo Core 0 e mudam de estado com base nos valores lidos dos sensores (ex: acende o LED verde se a distância for menor que 100mm).

-----

### ⚙ Hardware Necessário

| Componente | Quant. | Observações |
| :--- | :---: | :--- |
| Placa BitDogLab (RP2040) | 1 | A placa já inclui LEDs e botões. |
| Sensor de Distância VL53L0X | 1 | (Endereço I2C: `0x29`) |
| Sensor de Luz BH1750 | 1 | (Endereço I2C: `0x23`) |
| Display OLED 128x64 (SSD1306) | 1 | (Endereço I2C: `0x3C`) |

-----

### 🔌 Conexões e Configuração

**Barramento I2C0 (Sensor de Distância):**

  * `VL53L0X SDA` -\> `GPIO 0` (Pino 1)
  * `VL53L0X SCL` -\> `GPIO 1` (Pino 2)

**Barramento I2C1 (Display e Sensor de Luz):**

  * `Display OLED SDA` -\> `GPIO 14` (Pino 19)
  * `Display OLED SCL` -\> `GPIO 15` (Pino 20)
  * `BH1750 SDA` -\> `GPIO 14` (Pino 19)
  * `BH1750 SCL` -\> `GPIO 15` (Pino 20)

**Pinos da Placa (BitDogLab):**

  * `LED Verde` -\> `GPIO 11`
  * `LED Azul` -\> `GPIO 12`
  * `Botão B` -\> `GPIO 6` (Usado para BOOTSEL)

> **⚠ Importante:** Garanta um `GND` comum entre todos os componentes. Ambos os dispositivos no barramento `i2c1` (OLED e BH1750) devem ter endereços diferentes (`0x3C` e `0x23`) para funcionar corretamente.

-----

### 🚀 Começando

#### Pré-requisitos de Software

  * **SDK:** Raspberry Pi Pico SDK
  * **Linguagem:** C
  * **Build System:** CMake
  * **IDE Recomendada:** VS Code com a extensão "CMake Tools"

#### Compilação

1.  **Clone o repositório:**

    ```bash
    git clone https://github.com/wilwtiher/DoisNucleosDoRP2040
    cd [NOME_DO_DIRETORIO]
    ```

2.  **Crie e acesse a pasta de build:**

    ```bash
    mkdir build
    cd build
    ```

3.  **Gere os arquivos de compilação:**
    (Certifique-se de que seu `PICO_SDK_PATH` está configurado)

    ```bash
    cmake ..
    ```

4.  **Compile o projeto e envie para o pico:**
      * Na extensão, clique no botão Run no canto esquerdo inferior.
        
5.  **Acesso:**

      * Após o upload, abra um monitor serial (Baud Rate: 115200) para ver os logs de `printf` do Core 1.
      * Observe o display OLED para ver a interface sendo atualizada pelo Core 0.

-----

### 📁 Estrutura do Projeto

```
.
├── lib/
│   ├── bh1750_light_sensor.c
│   ├── bh1750_light_sensor.h
│   ├── vl53l0x_sensor.c
│   ├── vl53l0x_sensor.h
│   ├── ssd1306.c
│   ├── ssd1306.h
│   └── font.h
├── .gitignore
├── CMakeLists.txt
├── main.c
└── ...
```

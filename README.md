# 🚦 Simulador de Semáforo com Raspberry Pi Pico

Este projeto implementa um simulador de semáforo utilizando a placa *Raspberry Pi Pico*. O sistema controla um LED RGB simulando as fases de um semáforo de trânsito: vermelho, amarelo e verde, alternando os estados a cada 3 segundos.

## 🛠️ Requisitos

- Placa *Raspberry Pi Pico*
- LEDs e resistores correspondentes
- Ambiente de desenvolvimento configurado com o *SDK do Raspberry Pi Pico*

## 📝 Descrição do Funcionamento

O sistema funciona da seguinte forma:

1. Inicializa a comunicação serial e os pinos dos LEDs.
2. Começa com o LED vermelho aceso.
3. Um temporizador altera o estado do semáforo a cada 3 segundos, seguindo a sequência:
   - 🔴 **Vermelho** → 🟡 **Amarelo** → 🟢 **Verde** → 🔴 **Vermelho**...
4. O loop principal imprime uma mensagem a cada segundo para indicar que o sistema está rodando.

## 📜 Código

O código principal está implementado em C e faz uso das bibliotecas do *Raspberry Pi Pico* para manipulação de GPIOs e temporizadores.

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_PIN_RED    11
#define LED_PIN_YELLOW 12
#define LED_PIN_GREEN  13
#define SEMAFORO_DELAY_MS 3000
#define MAIN_DELAY_MS 1000

typedef enum {
    SEMAPHORE_RED,
    SEMAPHORE_YELLOW,
    SEMAPHORE_GREEN
} semaphore_state_t;

volatile semaphore_state_t current_state = SEMAPHORE_RED;

void update_semaphore_state(void) {
    gpio_put(LED_PIN_RED, false);
    gpio_put(LED_PIN_YELLOW, false);
    gpio_put(LED_PIN_GREEN, false);
    
    switch (current_state) {
        case SEMAPHORE_RED:
            gpio_put(LED_PIN_RED, true);
            break;
        case SEMAPHORE_YELLOW:
            gpio_put(LED_PIN_YELLOW, true);
            break;
        case SEMAPHORE_GREEN:
            gpio_put(LED_PIN_GREEN, true);
            break;
    }
}

bool repeating_timer_callback(struct repeating_timer *t) {
    switch (current_state) {
        case SEMAPHORE_RED:
            current_state = SEMAPHORE_YELLOW;
            break;
        case SEMAPHORE_YELLOW:
            current_state = SEMAPHORE_GREEN;
            break;
        case SEMAPHORE_GREEN:
            current_state = SEMAPHORE_RED;
            break;
    }
    update_semaphore_state();
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_init(LED_PIN_YELLOW);
    gpio_set_dir(LED_PIN_YELLOW, GPIO_OUT);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    
    current_state = SEMAPHORE_RED;
    update_semaphore_state();
    
    struct repeating_timer timer;
    add_repeating_timer_ms(SEMAFORO_DELAY_MS, repeating_timer_callback, NULL, &timer);
    
    while (true) {
        sleep_ms(MAIN_DELAY_MS);
        printf("Passou 1 segundo.\n");
    }
    return 0;
}
```

## 🚀 Como Compilar e Executar

1. Certifique-se de que o SDK do Raspberry Pi Pico está instalado.
2. Compile o código usando *CMake* e *Make*:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```
3. Carregue o arquivo `.uf2` gerado na placa Raspberry Pi Pico.

## 📌 Observações

- Os LEDs estão conectados aos pinos **GPIO 11, 12 e 13**.
- O tempo de transição entre os estados do semáforo é de **3 segundos**.
- A cada **1 segundo**, uma mensagem é impressa no terminal.

## 📚 Referências

- [Documentação do Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [SDK do Raspberry Pi Pico](https://github.com/raspberrypi/pico-sdk)

---
✍️ **Desenvolvido por GABRIEL SANTOS DE LACERDA**


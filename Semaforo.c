#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos para o LED RGB
#define LED_PIN_RED    11  // LED Vermelho
#define LED_PIN_YELLOW 12  // LED Amarelo
#define LED_PIN_GREEN  13  // LED Verde

// Tempo para a mudança de estado do semáforo (3.000 ms = 3 segundos)
#define SEMAFORO_DELAY_MS 3000
// Intervalo para mensagem no main (1.000 ms = 1 segundo)
#define MAIN_DELAY_MS 1000

// Definição dos estados do semáforo
typedef enum {
    SEMAPHORE_RED,
    SEMAPHORE_YELLOW,
    SEMAPHORE_GREEN
} semaphore_state_t;

// Variável global que guarda o estado atual do semáforo
volatile semaphore_state_t current_state = SEMAPHORE_RED;

// Função que atualiza os LEDs de acordo com o estado atual do semáforo
void update_semaphore_state(void) {
    // Primeiro, apaga todos os LEDs
    gpio_put(LED_PIN_RED, false);
    gpio_put(LED_PIN_YELLOW, false);
    gpio_put(LED_PIN_GREEN, false);
    
    // Acende apenas o LED correspondente ao estado
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

// Função de callback chamada a cada 3 segundos pelo temporizador
bool repeating_timer_callback(struct repeating_timer *t) {
    
    // Atualiza o estado do semáforo: 
    // vermelho -> amarelo -> verde -> vermelho...
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
    
    // Atualiza os LEDs conforme o novo estado
    update_semaphore_state();
    
    // Retorna true para manter o temporizador repetindo
    return true;
}

int main() {
    // Inicializa a comunicação serial (para o printf) e o sistema básico
    stdio_init_all();
    
    // Inicializa os pinos dos LEDs como saída
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    
    gpio_init(LED_PIN_YELLOW);
    gpio_set_dir(LED_PIN_YELLOW, GPIO_OUT);
    
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    
    // Inicializa o semáforo com o estado vermelho
    current_state = SEMAPHORE_RED;
    update_semaphore_state();
    
    // Configura o temporizador repetidor para 3 segundos
    struct repeating_timer timer;
    add_repeating_timer_ms(SEMAFORO_DELAY_MS, repeating_timer_callback, NULL, &timer);
    
    // Loop principal: imprime uma mensagem a cada 1 segundo
    while (true) {
        sleep_ms(MAIN_DELAY_MS);
        printf("Passou 1 segundo.\n");
    }
    
    return 0;
}

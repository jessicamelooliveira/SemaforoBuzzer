// Retirado de https://github.com/BitDogLab/BitDogLab-C/blob/main/blink/blink.c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define TEMP_VERMELHO 1
#define TEMP_AMARELO 1
#define TEMP_VERDE 1
#define PIN_LED_VEI_VERM 1
#define PIN_LED_VEI_AMAR 2
#define PIN_LED_VEI_VERD 3
#define PIN_LED_PED_VERD 4
#define PIN_BOTAO 13
#define PIN_BUZZER 6


void modo_semaforo(int step){
        if (step == 1) {
            gpio_put(PIN_LED_VEI_VERM, 1);   // Liga o LED vermelho
            gpio_put(PIN_LED_VEI_AMAR, 0);   // Desliga o LED amarelo
            gpio_put(PIN_LED_VEI_VERD, 0);  // Desliga o LED verde dos veiculos
            gpio_put(PIN_LED_PED_VERD, 1);  // Liga o LED verde dos pedestres
            sleep_ms(TEMP_VERMELHO*1000);
            // Falta implementar o BUZZER aqui
        }
        else if (step == 2){
            gpio_put(PIN_LED_VEI_VERM, 0);   // Desliga o LED vermelho
            gpio_put(PIN_LED_VEI_AMAR, 1);   // Liga o LED amarelo
            gpio_put(PIN_LED_VEI_VERD, 0);  // Desliga o LED verde dos veiculos
            gpio_put(PIN_LED_PED_VERD, 0);  // Desliga o LED verde dos pedestres
            sleep_ms(TEMP_AMARELO*1000);
        }
        else if (step == 3) {
            gpio_put(PIN_LED_VEI_VERM, 0);   // Desliga o LED vermelho
            gpio_put(PIN_LED_VEI_AMAR, 0);   // Desliga o LED amarelo
            gpio_put(PIN_LED_VEI_VERD, 1);  // Liga o LED verde dos veiculos
            gpio_put(PIN_LED_PED_VERD, 0);  // Desliga o LED verde dos pedestres
            sleep_ms(TEMP_VERDE*1000);
        }
        else {      // Modo segurança, fica piscando o LED AMARELO a cada 500ms
          gpio_put(PIN_LED_VEI_VERM, 0);   // Desliga o LED vermelho
          gpio_put(PIN_LED_VEI_AMAR, 1);   // Liga o LED amarelo
          gpio_put(PIN_LED_VEI_VERD, 0);  // Desliga o LED verde dos veiculos
          gpio_put(PIN_LED_PED_VERD, 0);  // Desliga o LED verde dos pedestres
          sleep_ms(500);
          gpio_put(PIN_LED_VEI_VERM, 0);   // Desliga o LED vermelho
          gpio_put(PIN_LED_VEI_AMAR, 0);   // Desliga o LED amarelo
          gpio_put(PIN_LED_VEI_VERD, 0);  // Desliga o LED verde dos veiculos
          gpio_put(PIN_LED_PED_VERD, 0);  // Desliga o LED verde dos pedestres
          sleep_ms(500);
        }
}

int main() {
    gpio_init(PIN_LED_VEI_VERM);
    gpio_init(PIN_LED_VEI_AMAR);
    gpio_init(PIN_LED_VEI_VERD);
    gpio_init(PIN_LED_PED_VERD);
    gpio_init(PIN_BOTAO);
    gpio_init(PIN_BUZZER);

    gpio_set_slew_rate(PIN_LED_VEI_VERM, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(PIN_LED_VEI_AMAR, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(PIN_LED_VEI_VERD, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(PIN_LED_PED_VERD, GPIO_SLEW_RATE_SLOW);

    gpio_set_dir(PIN_LED_VEI_VERM, GPIO_OUT);
    gpio_set_dir(PIN_LED_VEI_AMAR, GPIO_OUT);
    gpio_set_dir(PIN_LED_VEI_VERD, GPIO_OUT);
    gpio_set_dir(PIN_LED_PED_VERD, GPIO_OUT);
    gpio_set_dir(PIN_BOTAO, GPIO_IN);
    gpio_pull_up(PIN_BOTAO);
    gpio_set_dir(PIN_BUZZER, GPIO_OUT);

    // Inicialmente, desligar todos os LEDs
    gpio_put(PIN_LED_VEI_VERM, 0);
    gpio_put(PIN_LED_VEI_AMAR, 0);
    gpio_put(PIN_LED_VEI_VERD, 0);
    gpio_put(PIN_LED_PED_VERD, 0);

    while (true) {
        // Se o botão for pressionado
        if (gpio_get(PIN_BOTAO) == 0) {
          sleep_ms(TEMP_AMARELO*100);
          modo_semaforo(1);
        }
        // Se nenhum botão estiver pressionado
        else {
          modo_semaforo(1);
          modo_semaforo(2);
          modo_semaforo(3);
        }
        sleep_ms(1000);
    }
    return 0;
}

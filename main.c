#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define BOTAO 5
#define LED_VERM 1
#define LED_AMAR 2
#define LED_VERD 3
#define LED_PED 4

int main() {
  stdio_init_all();

  // CONFIGURACOES DO BOTAO
  gpio_init(BOTAO);
  gpio_set_dir(BOTAO, GPIO_IN);
  gpio_pull_up(BOTAO);

  // CONFIGURACOES DOS LEDS
  gpio_init(LED_VERM);
  gpio_init(LED_AMAR);
  gpio_init(LED_VERD);
  gpio_init(LED_PED);
  gpio_set_dir(LED_VERM, GPIO_OUT);
  gpio_set_dir(LED_AMAR, GPIO_OUT);
  gpio_set_dir(LED_VERD, GPIO_OUT);
  gpio_set_dir(LED_PED, GPIO_OUT);

  gpio_put(LED_VERM, 0);
  gpio_put(LED_AMAR, 0);
  gpio_put(LED_VERD, 0);
  gpio_put(LED_PED, 0);

  while (true) {
    if (gpio_get(BOTAO) == 0) {
      gpio_put(LED_VERM, 1);
      gpio_put(LED_AMAR, 1);
      gpio_put(LED_VERD, 1);
      gpio_put(LED_PED, 1);
      printf("Hello, Wokwi!\n");
    }
    else {
      gpio_put(LED_VERM, 0);
      gpio_put(LED_AMAR, 0);
      gpio_put(LED_VERD, 0);
      gpio_put(LED_PED, 0);
    }
    sleep_ms(150);
  }
  return 0;
}
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define LED_VERM 1
#define LED_AMAR 2
#define LED_VERD 3
#define LED_PED 4
#define BOTAO 5
#define BUZZER 14

#define BUZZER_FREQ 2000

void pwm_init_buzzer(unsigned int pino) {
    gpio_set_function(pino, GPIO_FUNC_PWM);

    unsigned int slice_num = pwm_gpio_to_slice_num(pino);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQ * 4096));
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(pino, 0);
}

void beep(unsigned int pino, unsigned int duracao) {
    unsigned int slice_num = pwm_gpio_to_slice_num(pino);

    pwm_set_gpio_level(pino, 1024);

    sleep_ms(duracao);

    pwm_set_gpio_level(pino, 0);

    sleep_ms(100);
}

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

  // INICIALIZACAO DO BUZZER
  pwm_init_buzzer(BUZZER);

  while (true) {
    if (gpio_get(BOTAO) == 0) {
      gpio_put(LED_VERM, 1);
      gpio_put(LED_AMAR, 1);
      gpio_put(LED_VERD, 1);
      gpio_put(LED_PED, 1);
      beep(BUZZER, 100);
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

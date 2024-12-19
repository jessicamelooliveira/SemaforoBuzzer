#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// DEFINICAO DOS PINOS DE CADA COMPONENTE
#define LED_VERM 1
#define LED_AMAR 2
#define LED_VERD 3
#define LED_PED 4
#define BOTAO 5
#define BUZZER 14

// DEFINICAO DA FREQUENCIA DO BUZZER
#define BUZZER_FREQ 2000

// DEFINICAO TEMPOS DO SEMAFORO
#define TEMPO_VERD 8
#define TEMPO_AMAR 2
#define TEMPO_VERM 10
#define TEMPO_PED_AMAR 5
#define TEMPO_PED_VERM 15

// FUNCAO PARA INICIAR O BUZZER
void pwm_init_buzzer(unsigned int pino) {
  gpio_set_function(pino, GPIO_FUNC_PWM);

  unsigned int slice_num = pwm_gpio_to_slice_num(pino);

  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQ * 4096));
  pwm_init(slice_num, &config, true);

  pwm_set_gpio_level(pino, 0);
}

// FUNCAO PARA O BEEP DO BUZZER
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

  // VARIAVEIS DE CONTROLE
  bool botao_pressionado = false;
  absolute_time_t botao_pressionado_tempo = 0;

  // Variáveis de controle de tempo
  absolute_time_t tempo_inicial;
  absolute_time_t tempo_atual;
  int estado_semaforo = 0; // 0 = normal, 1 = emergência

  // Configura o tempo inicial
  tempo_inicial = get_absolute_time();

  while (true) {
    // Verifica o tempo atual
    tempo_atual = get_absolute_time();

    // Verifica se o botão foi pressionado
    if (gpio_get(BOTAO) == 0 && !botao_pressionado) {
      botao_pressionado = true;
      botao_pressionado_tempo = tempo_atual;
      printf("Botão pressionado. Semáforo em modo de emergência.\n");
    }

    if (botao_pressionado) {
      // Modo de emergência
      if (absolute_time_diff_us(botao_pressionado_tempo, tempo_atual) < TEMPO_PED_AMAR * 1000000) {
        // Modo amarelo (5 segundos)
        gpio_put(LED_VERD, 0);
        gpio_put(LED_AMAR, 1);
        gpio_put(LED_VERM, 0);
        gpio_put(LED_PED, 0);
      }
      else if (absolute_time_diff_us(botao_pressionado_tempo, tempo_atual) < (TEMPO_PED_AMAR + TEMPO_PED_VERM) * 1000000) {
        // Modo vermelho (15 segundos)
        gpio_put(LED_AMAR, 0);
        gpio_put(LED_VERM, 1);
        gpio_put(LED_PED, 1);
        beep(BUZZER, 100);  // Buzzer
      }
      else {
        // Após o vermelho, volta ao ciclo normal
        botao_pressionado = false;
        gpio_put(LED_PED, 0);
        printf("Modo de emergência finalizado. Retornando ao ciclo normal.\n");
        tempo_inicial = tempo_atual; // Reseta o tempo do semáforo
      }
    } else {
      // Modo normal do semáforo
      if (absolute_time_diff_us(tempo_inicial, tempo_atual) < TEMPO_VERD * 1000000) {
        // Verde (8 segundos)
        gpio_put(LED_VERD, 1);
        gpio_put(LED_AMAR, 0);
        gpio_put(LED_VERM, 0);
      }
      else if (absolute_time_diff_us(tempo_inicial, tempo_atual) < (TEMPO_VERD + TEMPO_AMAR) * 1000000) {
        // Amarelo (2 segundos)
        gpio_put(LED_VERD, 0);
        gpio_put(LED_AMAR, 1);
        gpio_put(LED_VERM, 0);
      }
      else if (absolute_time_diff_us(tempo_inicial, tempo_atual) < (TEMPO_VERD + TEMPO_AMAR + TEMPO_VERM) * 1000000) {
        // Vermelho (10 segundos)
        gpio_put(LED_VERD, 0);
        gpio_put(LED_AMAR, 0);
        gpio_put(LED_VERM, 1);
      }
      else {
        // Reseta o ciclo
        tempo_inicial = tempo_atual;
      }
    }

    // Delay não bloqueante
    sleep_ms(100);
  }

  return 0;
}

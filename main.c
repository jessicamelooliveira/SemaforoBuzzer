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
  gpio_set_function(pino, GPIO_FUNC_PWM); // configura o pino para usar PWM

  unsigned int slice_num = pwm_gpio_to_slice_num(pino); // obtem o numero da "slice" PWM associada ao pino

  pwm_config config = pwm_get_default_config(); // cria uma configuracao padrao de PWM
  pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQ * 4096)); // divisor de clock
  pwm_init(slice_num, &config, true); // inicializa o PWM com a configuracao ajustada

  pwm_set_gpio_level(pino, 0); // seta nivel inicial do PWM como desligado
}

// FUNCAO PARA O BEEP DO BUZZER
void beep(unsigned int pino, unsigned int duracao) {
  unsigned int slice_num = pwm_gpio_to_slice_num(pino); // obtem o numero da "slice" PWM associada ao pino

  pwm_set_gpio_level(pino, 1024); // seta nivel do sinal PWM para 1024
  sleep_ms(duracao); // duracao do beep
  pwm_set_gpio_level(pino, 0); // seta nivel inicial do PWM como desligado 
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
  bool botao_pressionado = false; // indica se o botao foi pressionado
  absolute_time_t botao_pressionado_tempo = 0; // tempo em que o botao foi pressionado

  // VARIAVEIS DE CONTROLE DE TEMPO
  absolute_time_t tempo_inicial; // armazena o tempo inicial
  absolute_time_t tempo_atual; // armazena o tempo final
  int estado_semaforo = 0; // 0 = normal, 1 = pedestre

  tempo_inicial = get_absolute_time(); // configura o tempo inicial

  while (true) {
    tempo_atual = get_absolute_time(); // verifica o tempo atual

    // VERIFICACAO SE O BOTAO FOI PRESSIONADO
    // se o botao esta com estado baixo e nao foi registrado que foi pressionado
    if (gpio_get(BOTAO) == 0 && !botao_pressionado) {
      botao_pressionado = true; // registra que o botao foi pressionado
      botao_pressionado_tempo = tempo_atual; // armazena o tempo atual em que o botao foi pressionado
      printf("Botão pressionado. Semáforo em modo pedestre.\n");
    }

    if (botao_pressionado) {
      // MODO PEDESTRE
      // verifica se o tempo decorrido desde que o botão foi pressionado eh menor que o tempo definido para o amarelo (TEMPO_PED_AMAR)
      if (absolute_time_diff_us(botao_pressionado_tempo, tempo_atual) < TEMPO_PED_AMAR * 1000000) {
        // amarelo (5 segundos)
        gpio_put(LED_VERD, 0);
        gpio_put(LED_AMAR, 1);
        gpio_put(LED_VERM, 0);
        gpio_put(LED_PED, 0);
      }
      // verifica se o tempo decorrido desde que o botão foi pressionado eh menor que o tempo definido para o amarelo + vermelho (TEMPO_PED_AMAR + TEMPO_PED_VERM)
      else if (absolute_time_diff_us(botao_pressionado_tempo, tempo_atual) < (TEMPO_PED_AMAR + TEMPO_PED_VERM) * 1000000) {
        // vermelho (15 segundos)
        gpio_put(LED_AMAR, 0);
        gpio_put(LED_VERM, 1);
        gpio_put(LED_PED, 1); // semaforo do pedestre acionado
        beep(BUZZER, 100);  // com buzzer
      }
      else {
        // apos o vermelho, volta ao modo normal
        botao_pressionado = false; // reseta o indicativo do botao pressionado
        gpio_put(LED_PED, 0); // desliga o semaforo do pedestre
        printf("Modo pedestre finalizado. Retornando ao modo normal.\n");
        tempo_inicial = tempo_atual; // Rrseta o tempo do semáforo
      }
    } else {
      // MODO NORMAL DO SEMAFORO
      if (absolute_time_diff_us(tempo_inicial, tempo_atual) < TEMPO_VERD * 1000000) {
        // verde (8 segundos)
        gpio_put(LED_VERD, 1);
        gpio_put(LED_AMAR, 0);
        gpio_put(LED_VERM, 0);
      }
      else if (absolute_time_diff_us(tempo_inicial, tempo_atual) < (TEMPO_VERD + TEMPO_AMAR) * 1000000) {
        // amarelo (2 segundos)
        gpio_put(LED_VERD, 0);
        gpio_put(LED_AMAR, 1);
        gpio_put(LED_VERM, 0);
      }
      else if (absolute_time_diff_us(tempo_inicial, tempo_atual) < (TEMPO_VERD + TEMPO_AMAR + TEMPO_VERM) * 1000000) {
        // vermelho (10 segundos)
        gpio_put(LED_VERD, 0);
        gpio_put(LED_AMAR, 0);
        gpio_put(LED_VERM, 1);
      }
      else {
        // reseta o ciclo
        tempo_inicial = tempo_atual;
      }
    }
    sleep_ms(100);
  }
  return 0;
}

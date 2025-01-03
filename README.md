# Semáforo com sinal sonoro

>:computer: Por: [Jéssica de Melo Oliveira](https://github.com/jessicamelooliveira)\
>:hourglass: Status: Finalizado\
>:date: Última modificação: 20 de dezembro de 2024, 12:21

## 1. Objetivo
Este projeto tem como objetivo desenvolver um sistema de controle de sinais de trânsito para um cruzamento, incluindo:
- Sinal sonoro indicando permissão para pedestres atravessarem.
- Botão de acionamento para deficientes visuais que desencadeia uma sequência de fechamento do sinal de trânsito para veículos, permitindo a travessia segura em todas no cruzamento da via.

## 2. Descrição do Projeto
Os componentes utilizados foram:
- Raspberry Pi Pico para controle lógico.
- LEDs:
  - Vermelho, amarelo e verde para representar o sinal de trânsito dos carros.
  - Um LED verde para pedestres.
- Buzzer: para o som emitido quando o pedestre pode atravessar.
- Push button: para simular a botoeira (acionador do sinal de pedestre).
- Resistores: para os LEDs e botão.
- Protoboard e fiação.

A linguagem utilizada foi C.

## 4. Funcionamento do Sistema
O sistema possui dois modos:
### Modo inicial:
- Os LEDs para carros alternarão automaticamente (verde -> amarelo -> vermelho) seguindo uma lógica de tempo fixa (ex.: 8 segundos no verde, 2 no amarelo, 10 no vermelho).
- O LED verde para pedestres estará desligado e o buzzer ficará inativo.

### Modo pedestre:
  - O led amarelo deve ficar acionado por 5 segundos e depois o vermelho deve ser acionado por 15 segundos, permitindo o tempo de fechamento seguro para os veículos.
  - A luz verde do pedestre deve ficar acesa pelo mesmo tempo em que a luz vermelha do semáforo estiver ativa (15 segundos).
- Além disso, durante esse tempo, o buzzer deverá emitir um som intermitente, indicando que o sinal está aberto para o pedestre.

## 5. Simulação
Você pode acessar uma simulação do projeto através desse link: [https://wokwi.com/projects/417803015247022081](https://wokwi.com/projects/417803015247022081)

---
### License
This project is licensed under the Creative Commons Attribution 4.0 International License - see the [LICENSE](LICENSE) file for details.

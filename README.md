# Semáforo com sinal sonoro

>:computer: Por: [Jéssica de Melo Oliveira](https://github.com/jessicamelooliveira)\
>:hourglass: Status: Em construção\
>:date: Última modificação: 19 de dezembro de 2024, 20:35

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
O sistema possui dois estados:
### Estado inicial:
- Os LEDs para carros alternarão automaticamente (verde -> amarelo -> vermelho) seguindo uma lógica de tempo fixa (ex.: 8 segundos no verde, 2 no amarelo, 10 no vermelho).
- O LED verde para pedestres estará desligado e o buzzer ficará inativo.

### Estado de emergência:
  - O led amarelo deve ficar acionado por 5 segundos e depois o vermelho deve ser acionado por 15 segundos, permitindo o tempo de fechamento seguro para os veículos.
  - A luz verde do pedestre deve ficar acesa pelo mesmo tempo em que a luz vermelha do semáforo estiver ativa (15 segundos).
- Além disso, durante esse tempo, o buzzer deverá emitir um som intermitente, indicando que o sinal está aberto para o pedestre.

---
### License
This project is licensed under the Creative Commons Attribution 4.0 International License - see the [LICENSE](LICENSE) file for details.

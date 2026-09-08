# Integral

# Sistema de Controle da Válvula - Tractus
A ideia desse sistema é automatizar o controle do fluxo de água no acoplamento dos transdutores do TRACTUS, substituindo o controle manual. Para isso, é necessário que o sistema faça uma leitura do fluxo de água, processe esse sinal e retorne o comando à válvula proporcional, fazendo-a abrir ou fechar, de acordo com o nível de fluxo desejado.

Para o projeto, os componentes utilizados serão:
- [Fusível 2A + Porta Fusível]() (proteção elétrica do sistema)
- [Potenciômetro Linear 10K + Knob](https://www.eletrogate.com/potenciometro-linear-10k?utm_source=Site&utm_medium=GoogleMerchant&utm_campaign=GoogleMerchant&gad_source=4&gad_campaignid=20223015315&gbraid=0AAAAADqxjs_gpqOJfLVmq1WIuAi4hx24R&gclid=CjwKCAjwqJXUBhBNEiwA8BgG7omOoSZQO_TGoV6BXl1_1kDrA87dErBUL3YU2gr2WFoq7l1gJ6wrxxoCuWoQAvD_BwE) (selecionar o fluxo desejado)
- [Display LCD 16x2 I2C](https://www.eletrogate.com/display-lcd-16x2-i2c-backlight-azul?utm_source=Site&utm_medium=GoogleMerchant&utm_campaign=GoogleMerchant&srsltid=AfmBOoo3RP5EGnFZOkpZSUs0i7zt_wHO_53pNtBFpeUyX1B0HMtXSWeSKIY) (exibir o fluxo máximo, medido e desejado na tela)
- [Chave Gangorra]() (liga e desliga o sistema) 
- [Botão Cogumelo]() (emergência)
- [LED RGB](https://www.usinainfo.com.br/led-arduino/modulo-led-rgb-ky-016-2541.html)(status do fluxo)
- [Conector para fonte]()
- [Conector de aviação 3 pin para sensor (gx12)]()
- [Conector de aviação 5 pin para válvula (gx16)]()
- [espaçadores de nylon + parafusos m3]()
- [Conexões hidráulicas para sensor e válvula]()

[toc]
## **Arduino Uno R4 Minima**

A escolha desse modelo de Arduino se deve pela necessidade de um pino DAC para controlar a válvula. Com a válvula escolhida, não seria possível usar um pino de PWM, então o pino DAC seria a melhor opção. Além disso, esse modelo do Arduino pode ser alimentado por 12V, tensão que é utilizada pela maioria dos outros componentes, removendo a necessidade de um regulador ou circuito auxiliar para alimentação do microcontrolador.
Alternativa: usar um Arduino Uno R4 Minima (mais barato e sem wifi)
[datasheet](https://docs.arduino.cc/hardware/uno-r4-minima/)
- Corrente máxima: Até 170mA (módulo wifi ligado)
- Alimentação: 12V
- Pino DAC
- (16x11x3)cm

## **Válvula Proporcional de Esfera Motorizada 1/2"**
A válvula funciona com uma esfera motorizada, que é movida para abrir ou fechá-la e controlar o fluxo. 
Nesse caso, quando a corrente for de 4mA, a válvula está totalmente fechada e, quando a corrente for de 20mA, está totalmente aberta.

Ela foi escolhida por ser uma válvula proporcional, alimentada por 12V (que é utilizado em todo o restante do circuito), tem tamanho de 1/2" e é de fácil controle.

Essa válvula também tem um fio de saída, que determina para o microcontrolador a posição atual da esfera. Não está sendo utilizado no momento.

Será usado um fusível de 1.5A na alimentação dessa válvula, para protegê-la caso algo dê errado, visto que é o componente mais caro do projeto.

- Alimentação: 12V
- Entrada: Sinal de 4 a 20mA proveniente do conversor de sinal
- Saída: Sinal que diz a posição da esfera motorizada
- Corrente de Pico: 950mA

[link](https://ussolid.com/products/12-proportional-motorized-ball-valve-stainless-steel-dc-924v-420ma-control-5-wire-with-position-indicator-ip67-full-port?srsltid=AfmBOopSO5vGeMlotOkvBaC3cu2WbbtJKdKoAcXBQKKmXDnx0gKAIlW_)

## **Sensor de Fluxo YF-B1**
Esse sensor de Fluxo de Água se adequa ao projeto pelo tamanho do tubo (1/2"), equivalente ao tubo já utilizado para o projeto, e pela vazão medida, que vai de 1L/min a 25L/min. O fluxo máximo medido pela empresa foi de cerca de 20L/min, então não haveria problemas de o fluxo real superar a capacidade de medição do sensor.

O sensor é composto por uma turbina, um ímã e um sensor hall. Quando a água passa pelo sensor, a turbina é girada e o ímã gera um campo magnético, medido pelo sensor hall e convertido em sinal elétrico, na forma de uma onda quadrada, com amplitude (Vpp) igual à alimentação. O fluxo é medido pela contagem de pulsos (frequência), dividido por 11.
- Corrente máxima: 15mA
- Alimentação: 5-15V
- Q = F/11
[datasheet](https://www.berrybase.de/en/product-datasheet/019391e394b77123b67497a442305b7e/create?srsltid=AfmBOop15xInDI9Svvvt8oZdoz8aRe0pngtSfpSjBk5hLPy-z7e2IX37)

## **Módulo Optoacoplador PC-817**
Esse módulo é usado para reduzir a tensão de saída do sensor de fluxo de 12V para 5V, tensão utilizada nos pinos GPIO do microcontrolador. 

O optoacoplador em si é composto por um LED infravermelho e um transistor bipolar fotossensível. Quando o LED é alimentado por 12V (nível lógico alto), ele emite luz infravermelho e aciona o transistor, sem conexão física. Quando o LED está apagado (nível lógico baixo), o transistor entra em corte. Esse componente, além de provocar a queda de tensão, possibilita o isolamento e proteção do Arduino, já que, se uma corrente muito alta for aplicada a ele, queimará apenas o LED infravermelho e o módulo pode ser substituído.
- Corrente nominal: 50mA
- Alimentação: 5V proveniente do Arduino
- Entrada: Output do sensor de fluxo
- Saída: Ligada em um pino GPIO do Arduino 
- (30x39x12)mm

[link 1](https://www.proesi.com.br/pc-817-modulo-optoacoplador-2-canais?utm_source=google-ads&utm_source=[V4]-[VIN%C3%8DCIUS]-[VENDAS]-[PMAX]-[ROAS-DESEJADO]&utm_content=[P1]-[REMARKETING]-[VISITANTES-E-COMPRADORES]&gad_source=1&gad_campaignid=17656097982&gbraid=0AAAAADP44mArpzV4-UN7oo_pnqo0EcWCb&gclid=CjwKCAjwhZDUBhBGEiwAbi5bjkqhbMGBGWkPnYsqBNMthL__a9TDpPIeV4m9nKeR4rEb-cksaXv_9BoC_8YQAvD_BwE#derivacao=8)
[link 2](https://www.saravati.com.br/modulo-de-isolamento-optoacoplador-2-canais-pc817.html?gad_source=1&gad_campaignid=23551463662&gbraid=0AAAAAC2QUl8vy7cdIWwDqgJ-0JNm94Uwm&gclid=CjwKCAjwhZDUBhBGEiwAbi5bjlApaNRcONzjVyzdU4BQFQJQwJzLJDVWXSfT0V6XE5J2UZfgk_Wf9hoCdF0QAvD_BwE)

## **Módulo Conversor de Sinal**
O módulo conversor de sinais deve ser utilizado porque o controle da válvula escolhida opera baseado em um sinal de corrente de 4 a 20mA. O Arduino não é capaz de fornecer essa corrente variável a partir de um pino.

Esse módulo recebe o sinal analógico de tensão do arduino e o converte em corrente. Deve ser calibrado de acordo com as tensões de zero e máximo.

- Corrente máxima: 20mA
- Alimentação: 12V
- Entrada: proveniente do pino DAC do Arduino
- Saída: válvula
- (55x26x12)mm

[link do conversor](https://www.usinainfo.com.br/conversores-de-sinal/conversor-de-tensao-para-corrente-0-5v-para-4-20ma-ctc10-6168.html?srsltid=AfmBOopTunHgteONqKAmFXb7Kcbxz_sJInJUIO49kJ4LP1mp9DUe2ZYv8xc)

## Fonte
https://www.fontesmeanwell.com.br/rs-35-12-fonte-chaveada-fechada-36w-88-264vca125-373vcc-saida-12v-3a-mean-well
- (99x82x36)mm

## **Orçamento de Corrente**
20 (conversor) + 50 (optoacoplador) + 950 (válvula) + 170 (arduino) + 15 (sensor) = 1205 mA + 20% ~= 1.5A

## Esquemático
![image](https://hackmd.io/_uploads/rkuJSFovfg.png)


## Controle
No código, é feita a contagem de pulsos vindos do sensor para determinar o fluxo. A partir disso, a ideia é definir o valor de máximo do fluxo quando a válvula está 100% aberta e definir o valor desejado do fluxo.

As opções para definir o fluxo desejado, por enquanto, são: definir através de uma interface e comunicar via wifi para o Arduino; ou definir por um potenciômetro.

A partir daí, deve ser implementado o algoritmo de controle, verificando o fluxo instantâneo e abrindo ou fechando a válvula, conforme a necessidade.

## Diagrama Lógico
![diagrama_logico_controle_valvula_tractus](https://hackmd.io/_uploads/ryZ84KiPzx.png)

## Diagrama de Potência
![diagrama_potencia_controle_valvula_tractus](https://hackmd.io/_uploads/rkaWeKoPMl.png)

## Diagrama do Código
![diagrama_de_blocos_codigo_controle_valvula_tractus](https://hackmd.io/_uploads/Sk5DtdjDze.png)

## Requisição de Compras
[Planilha de RC](https://integralmonitoramentoein138-my.sharepoint.com/:x:/g/personal/joao_baptista_integral-imi_com_br/IQD-fAICyIjvRIVYvroE831LAU3roNpbqv6PYGpeJ6V6XJs?e=of8ZUL&nav=MTVfezAwMDAwMDAwLTAwMDEtMDAwMC0wMDAwLTAwMDAwMDAwMDAwMH0)

## Testes
1) Teste de Continuidade entre alimentação e terra, pinos do Arduino e componentes.
2) Com a válvula desconectada, ligar o sistema pela chave e medir a tensão da fonte (aproximadamente 12V).
3) Testar a tela LCD e verificar sua mensagem.
4) Girar o potenciômetro e testar a variação do Set no display e a variação das cores do LED.
5) Pressionar o botão de Stop e verificar o display e o LED. Depois soltar o botão e ver se o sistema retorna ao estado normal.
6) Testar a turbina do sensor e verificar o display e o LED.
7) Medir a corrente de saída do módulo conversor, onde o mínimo deve ser 4mA e o  máximo deve ser 20mA.
8) Conectar a válvula no sistema e girar o potenciômetro para verificar se o controle está sendo efetuado.

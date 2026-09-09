# Sistema de Controle da Válvula - Tractus
A ideia desse sistema é automatizar o controle do fluxo de água no acoplamento dos transdutores do TRACTUS, substituindo o controle manual. Para isso, é necessário que o sistema faça uma leitura do fluxo de água, processe esse sinal e retorne o comando à válvula proporcional, fazendo-a abrir ou fechar, de acordo com o nível de fluxo desejado.

Para o projeto, os componentes utilizados serão:
- [Fusível PTC 2.5A]()
- [Potenciômetro Linear 10K + Knob](https://www.eletrogate.com/potenciometro-linear-10k?utm_source=Site&utm_medium=GoogleMerchant&utm_campaign=GoogleMerchant&gad_source=4&gad_campaignid=20223015315&gbraid=0AAAAADqxjs_gpqOJfLVmq1WIuAi4hx24R&gclid=CjwKCAjwqJXUBhBNEiwA8BgG7omOoSZQO_TGoV6BXl1_1kDrA87dErBUL3YU2gr2WFoq7l1gJ6wrxxoCuWoQAvD_BwE)
- [Display ST7735 SPI]()
- [Chave Gangorra]()
- [Botão Cogumelo]() 
- [LED RGB]()
- [Conector para fonte]()
- [Conector de aviação 7 ou 8 pin para válvula (gx16)]()
- [espaçadores de nylon + parafusos m3]()
- [Conexões hidráulicas para sensor e válvula]()

[toc]
## **Renesas R7FA4M1AB3CFM_AA0**

Esse chip de microcontrolador foi selecionado pois é o mesmo chip usado no Arduino R4 Minima, que seria utilizado anteriormente, mas foi substituído quando a decisão da placa foi adotada. Ele tem um pino DAC, usado para enviar o sinal para o controle da válvula. Os outros pinos GPIO serão usados para os demais componentes.
[Datasheet](https://www.renesas.com/en/document/dst/ra4m1-group-datasheet?r=1054146)
[Manual de usuário](https://www.renesas.com/en/document/mah/renesas-ra4m1-group-users-manual-hardware?r=1054146)
- Corrente máxima: 8.3mA
- Alimentação: 3.3V
- Pino DAC
![image](https://hackmd.io/_uploads/HkJtU0pOMl.png)


## **Válvula Proporcional de Esfera Motorizada 1/2"**
A válvula funciona com uma esfera motorizada, que é movida para abrir ou fechá-la e controlar o fluxo. 
Nesse caso, quando a tensão for de 0V, a válvula está totalmente fechada e, quando a tensão for de 10V, está totalmente aberta.

Ela foi escolhida por ser uma válvula proporcional, alimentada por 12V (tensão da fonte), tem tamanho de 1/2" e é de fácil controle.

Essa válvula também tem um fio de saída, que determina para o microcontrolador a posição atual da esfera. Não está sendo utilizado no momento.

O microcontrolador só é capaz de fornecer 3.3V. Portanto, um amplificador operacional foi adotado para amplificar o sinal analógico, dando um ganho de 3.
- Alimentação: 12V
- Entrada: Sinal de 0 a 10V proveniente do amplificador operacional
- Saída: Sinal que diz a posição da esfera motorizada
- Corrente de Pico: 950mA

[Link](https://simokit.com.br/product/valvula-esfera-reta-inox-dn15-proporcional-0-10v/)

## **Sensor de Fluxo YF-B1**
Esse sensor de Fluxo de Água se adequa ao projeto pelo tamanho do tubo (1/2"), equivalente ao tubo já utilizado para o projeto, e pela vazão medida, que vai de 1L/min a 25L/min. O fluxo máximo medido pela empresa foi de cerca de 20L/min, então não haveria problemas de o fluxo real superar a capacidade de medição do sensor.

O sensor é composto por uma turbina, um ímã e um sensor hall. Quando a água passa pelo sensor, a turbina é girada e o ímã gera um campo magnético, medido pelo sensor hall e convertido em sinal elétrico, na forma de uma onda quadrada, com amplitude (Vpp) igual à alimentação. O fluxo é medido pela contagem de pulsos (frequência), dividido por 11.
- Corrente máxima: 15mA
- Alimentação: 5-15V
- Q = F/11

[Link](https://www.smartcomponentes.com/produto/sensor-de-fluxo-de-agua-12-1-25lmin-latao-yf-b1-175mpa.html)
[Datasheet](https://www.berrybase.de/en/product-datasheet/019391e394b77123b67497a442305b7e/create?srsltid=AfmBOop15xInDI9Svvvt8oZdoz8aRe0pngtSfpSjBk5hLPy-z7e2IX37)

## **Amplificador Operacional TLV2372**
Esse chip tem dois amplificadores operacionais dentro de si, que serão utilizados para funções diferentes.

O primeiro é utilizado para dar ganho ao sinal de saída do controlador para a válvula. Ele está conectado em configuração não inversora e tem ganho a partir da seguinte relação:
![image](https://hackmd.io/_uploads/ryTeXRT_fl.png)
No caso, os resistores em questão são R6 e R3. O sinal vem do pino 53 do microcontrolador. 
O resistor R7 é um resistor de pulldown para drenar a corrente e forçar a saída a ser 0V, evitando que haja sinal quando não deveria.
![image](https://hackmd.io/_uploads/HyJP-RauMe.png)

O segundo amplificador está sendo utilizado como um buffer entre o sensor de fluxo e o microcontrolador. O divisor de tensão da entrada reduz a tensão de 12V para 3.3V, que passa pelo buffer e dá a saída no pino 49 do microcontrolador.
O resistor R13 atua como proteção, limitando a corrente e reduzindo o atraso introduzido pelas capacitâncias parasitas do circuito.
![image](https://hackmd.io/_uploads/rk0a4R6_Ge.png)



## **Regulador de Tensão 3.3V AMS1117-3.3**
O regulador está no circuito para baixar a tensão de 12V para 3.3V, que é usada para alimentar alguns componentes. Os capacitores são um circuito auxiliar exigido pelo datasheet do regulador e servem para fazer a filtragem de variações de baixas e altas potências.
![image](https://hackmd.io/_uploads/HJjoSC6Oze.png)

[Link]()

## **LED RGB**
Usado para exibir visualmente o estado do sistema, com cada cor tendo um significado diferente:
- Azul:
- Amarelo:
- Vermelho:
- Laranja:
Eles são conectados ao microcontrolador pelos pinos 12 (RED), 13 (BLUE), 14 (GREEN).
![image](https://hackmd.io/_uploads/HyXmOAp_zl.png)


## Fonte
https://www.fontesmeanwell.com.br/rs-35-12-fonte-chaveada-fechada-36w-88-264vca125-373vcc-saida-12v-3a-mean-well
- (99x82x36)mm

## Display ST7735
![image](https://hackmd.io/_uploads/rJVMsy1KGe.png)

![image](https://hackmd.io/_uploads/r1kUICTdzx.png)


## Conector USB-C 2.0
Usado para passar o código ao microcontrolador.
![image](https://hackmd.io/_uploads/rkEPI0a_Gl.png)


## BOM
- 7x Capacitor Cerâmico smd 100nF
- 1x Capacitor Cerâmico smd 22uF
- 1x Capacitor Cerâmico smd 10uF
- 2x Capacitor Cerâmico smd 4.7uF
- 6x Resistor smd 10k
- 2x Resistor smd 20k
- 2x Resistor smd 1k
- 2x Resistor smd 5.1k
- 3x Resistor smd 330
- 1x Resistor smd 30k
- 1x Fusível PTC smd 2.5A
- 2x Botão Push smd
- 1x LED RGB catodo comum
- 1x Conector Fêmea USB-C 2.0
- 1x Conector Molex Mini-Fit Jr 2x4
- 1x Conector Molex Mini-Fit Jr 2x3
- 1x Conector Molex Mini-Fit Jr 2x2
- 1x Botão Cogumelo
- 1x Chave Gangorra On-Off
- 1x TLV2372
- 1x AMS1117-3.3
- 1x YF-B1
- 1x Potenciômetro Linear 10k
- 1x Knob de potenciômetro
- 1x Válvula Proporcional
- 1x Renesas R7FA4M1AB3CFM_AA0
- 1x Fonte RS-35-12
- 1x Conector de Tomada Tripolar Macho
- 1x Conectores de Aviação GX16 7 pinos
- ?x componentes de fixação mecânica

## **Modelos 3D**
- [Potenciômetro](https://sketchfab.com/3d-models/potenciometro-linear-10k-ee4e9d38fba44a51ad0ec0225995b813)
- [Knob](https://www.traceparts.com/br/product/rs-group-knob-black-blue-marker-line-6mm-d?CatalogPath=RS_COMPONENTS%3APSF_433042&Product=90-08032024-083055)
- [Display](https://grabcad.com/library/tft-lcd-1-8-inch-spi-st7735-128x160-1)
- [LED RGB](https://sketchfab.com/3d-models/rgb-led-7ee14ba4a6204e74a5d37169f342c785)
- [Fonte - não encontrado]()
- [Botão de Emergência - não encontrado]()
- [Chave Gangorra On-Off - não encontrado]()
- [Conector Tomada Tripolar Macho](https://grabcad.com/library/tomada-macho-2p-t-10a-250v-1)
- [Conector de Aviação GX16 - 7 ou 8 Pinos - não encontrado]()

## **Consumo de Corrente**


## Esquemático
![image](https://hackmd.io/_uploads/H1esLRpuze.png)



## Controle
No código, é feita a contagem de pulsos vindos do sensor para determinar o fluxo. A partir disso, a ideia é definir o valor de máximo do fluxo quando a válvula está 100% aberta e definir o valor desejado do fluxo. Após isso, o fluxo será definido por uma conversão digital-analógico de valor até 3.3V. Esse sinal passa por um amplificador operacional de ganho 3 e segue para a válvula.

## Diagrama Lógico


## Diagrama de Potência


## Diagrama do Código


## Requisição de Compras
[RC Válvula](https://integralmonitoramentoein138-my.sharepoint.com/:x:/g/personal/joao_baptista_integral-imi_com_br/IQAJ0VLL1GrMQLJxgtgs7qGfAQLTgIY3VIOuGB1zUlPJKKM?e=Wh28EZ)
[RC Sensor](https://integralmonitoramentoein138-my.sharepoint.com/:x:/g/personal/joao_baptista_integral-imi_com_br/IQDEU0ApyflLTq0Ow4kNu74WAR9PTYG0pupGPaK7aSaTUOs?e=R4D45n)

## Testes
1) Teste de Continuidade entre alimentação e terra, pinos do microcontrolador e componentes.
2) Com a válvula desconectada, ligar o sistema pela chave e medir a tensão da fonte (aproximadamente 12V).
3) Testar o display e verificar sua mensagem.
4) Girar o potenciômetro e testar a variação do Set no display e a variação das cores do LED.
5) Pressionar o botão de Stop e verificar o display e o LED. Depois soltar o botão e ver se o sistema retorna ao estado normal.
6) Testar a turbina do sensor e verificar o display e o LED.
7) Medir a tensão de saída do amplificador operacional, onde o mínimo deve ser 0V e o máximo deve ser 10V (~9.99V).
8) Conectar a válvula no sistema e girar o potenciômetro para verificar se o controle está sendo efetuado.

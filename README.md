# DataLogger
#### Monitoramento de Temperatura, Umidade e Luminosidade

Este projeto tem como finalidade realizar o monitoramento dos valores de temperatura, umidade e luminosidade de um ambiente.
Possui um sistema de alerta sonoro e luminoso para quando os parametros estiverem fora do intervalo seguro
As ocorrencias são salvas automaticamente na memória do arduino, mostrando os valores junto com o TimeStamp

## Componentes

1x - Protoboard 830 pontos

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/b3013de0-25c3-406b-8304-a71c9d6bd800)

1x - Arduino Uno (ou similar)

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/fe9fd2f9-14f1-416e-a87c-ad581d214809)

1x - Display de Cristal Líquido (LCD) com interface I2C

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/74edf07e-8465-41a1-9ae3-897024bcf76e)

1x - Sensor de Temperatura e Umidade DHT11 (Temperatura °C ou °F, Umidade medido em % umidade relativa)

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/305b00f8-ee31-4ea0-af80-44de1a7fd088)

1x - Sensor de Luminosidade LDR (Medido em %)

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/51fbc2af-d724-4181-82d5-1040e6c71eba)

1x - RTC (Módulo de Relógio em Tempo Real)  DS3231 (Formato de data e hora no padrao BR ou EUA)

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/5a43cbf9-858f-4550-a300-17befceb3a7f)

1x - Buzzer

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/11aaec84-473a-4a14-9a99-ace950229045)

1x - Resistor 10K Ohm

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/b7fdfb67-0e5c-41e3-8005-bb56b99e792f)

1x - Resistor 330 Ohm

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/1a14f904-ae1c-4302-a3f4-c21f2948c29e)

25x (±) - jumpers para conexões

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/5c741e9a-8c59-46ed-8a77-b079aab7f1d6)

## Funcionamento

O sistema realiza a leitura dos sensores de temperatura, umidade e luminosidade em intervalos regulares. As leituras são então processadas para calcular a média dos valores obtidos. Se os valores médios estiverem fora de um intervalo pré-estabelecido considerado seguro, o sistema emite um alerta sonoro e luminoso e salva os dados na memória EEPROM.

## Pinagem

### Arduino Uno:

- Pino 2: Conectado ao botão físico para alternar entre os idiomas.
- Pino 7 (DHTPIN): Conectado ao sensor de temperatura e umidade (DHT11).
- Pino A0 (LDRPIN): Conectado ao sensor de luminosidade (LDR).
- Pino 10: Conectado a um buzzer para emitir alertas sonoros.

### Módulo de Relógio em Tempo Real (RTC DS3231):

- SCL: Conectado ao pino A5 do Arduino (SCL/I2C Clock).
- SDA: Conectado ao pino A4 do Arduino (SDA/I2C Data).
- VCC: Conectado ao 5V do Arduino.
- GND: Conectado ao GND do Arduino.

### Display de Cristal Líquido (LCD) com interface I2C:

- SDA: Conectado ao pino A4 do Arduino (SDA/I2C Data).
- SCL: Conectado ao pino A5 do Arduino (SCL/I2C Clock).
- VCC: Conectado ao 5V do Arduino.
- GND: Conectado ao GND do Arduino.

### Outros:

- Pino 8: Conectado a um LED para emitir alertas visuais.
- Pino 7 (DHTPIN): Conectado ao sensor de temperatura e umidade (DHT11).
- Pino 10: Conectado a um buzzer para emitir alertas sonoros.
- Pino A0 (LDRPIN): Conectado ao sensor de luminosidade (LDR).

## Montagem

### Esquema eletrico

O desenvolvimento do projeto foi feito utilizando os conhecimentos eletricos/eletronicos adquiridos até aqui.



### Simulação Wokwi

A simulação foi desenvolvida efetuando as ligaçoes assim como explicado no tópico "Pinagem".

Obs: Tivemos de utilizar o sensor de temperatura e umidade DHT22, pela falta do DHT11 no software.

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/33f09664-e96f-4654-9df1-3c629a0b0da3)

### Protótipo

A montagem foi realizada atraves da ligação dos componentes respeitando a pinagem adequada. Em seguida plugamos em uma máquina que possui a IDE do Arduino e passamos ocodigo para o Arduino.

![image](https://github.com/Hugo-Victorr/ProjetoDataLogger/assets/105120915/bd924cb9-e440-45da-a60a-2c78e315e4b4)


## Fluxo do programa

- Durante a configuração inicial, o logotipo é exibido no LCD.
- Após a inicialização, o sistema realiza leituras dos sensores, calcula a média e verifica a validade dos valores.
- Se um alerta for acionado, os dados são salvos na EEPROM.
- O sistema exibe continuamente as leituras no LCD, atualizando a cada minuto.

## Considerações finais

O projeto fornece um sistema básico de monitoramento ambiental que pode ser expandido e personalizado conforme necessário. A utilização de uma memória EEPROM permite o armazenamento de dados históricos para análise posterior. Além disso, a capacidade de alternar entre os idiomas oferece flexibilidade para usuários de diferentes regiões.

Projeto desenvolvido para a aula de Sistemas embarcados, lecionada pelo professor Cabrini, pela Faculdade Egengenheiro Salvador Arena.

Link Youtube: [https://youtu.be/mdeD3P1tvLw](https://youtu.be/mdeD3P1tvLw)

Integrantes:

- Danilo Miranda;

- Hugo Victor;

- Matheus Martins;

- Matheus Pedroza;

- Thiago Sousa.
  












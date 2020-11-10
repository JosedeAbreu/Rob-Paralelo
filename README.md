<h1 align='center'>Robô Paralelo</h1>
<p align='center'>Projeto de um Sistema Surpervisório e Controlador para um Robô Paralelo</p>

## *Estrutura Física*

O sistema físico é constituido pela operação em conjunto de três motores CC a fim de controlar o anexo estrutural posicionado no interior da máquina.

## *Processo*

- [x] <a href="#Leitura de Encoders">Leitura de Encoders</a>
- [x] Acionamento dos Carros
- [x] Implementação do Sistema Supervisório
- [x] Definição dos Parâmetros PID
- [x] Controle de Posição
- [ ] Implementação de modelo matemático

## *Descrição de Etapas*
### Leitura de Encoders

Com a leitura dos canais provenientes do hardware, realiza-se a formulação lógica capaz de identificar o deslocamento dos atuadores em relação ao seu eixo.
#### Sequências
Os padrões sequenciais tem como referência as `Entrada 1` e `Entrada 2`, respectivamente responsáveis pelos `Canal 1` e `Canal 2`.

###### Ciclo positivo 
| Tempo | Entrada 1 | Entrada 2 |
|-------|-----------|-----------|
|*T1*|0|0|
|*T2*|1|0|
|*T3*|1|1|
|*T4*|0|1|

###### Ciclo negativo
| Tempo | Entrada 1 | Entrada 2 |
|-------|-----------|-----------|
|*T1*|0|0|
|*T2*|0|1|
|*T3*|1|1|
|*T4*|1|0|

#### Métodos Biblioteca
Com base em tais padrões sequenciais, foi desevolvida a Biblioteca <a href='./BIBLIOTECAS/ENCODER'>ENCODER</a>, com o intuito de automatizar o processo de medição dos sinais referentes aos canais digitais.

```c++
ENCODER(port1, port2);        //Método construtor.
registerRead();               //Retorna a leitura por registrador da Porta 2.
getPort1();                   //Retorna o endereço da Porta 1.
getPort2();                   //Retorna o endereço da Porta 2.
```
A biblioteca atua inserida em anexo a uma interrupção, parametrizada para `RISING`, aplicada ao `Canal 1` em cada Encoder.

### Acionamento dos Carros

A movimentação dos carros é realizada por meio da manipulação de portas `PWM` e `Digital`, que indicam potência e sentido de rotação, respectivamente.
#### Padrão de Movimentação
| Porta Digital | PWM | Movimentação |
|-------|-----------|-----------|
| 0 | 0 | Parado |
| 1 | 0 | Parado |
| 1 | 255 | Sentido Horário Máximo |
| 0 | 255 | Sentido Anti-Horário Máximo |

#### Métodos Biblioteca
A Biblioteca <a href="./BIBLIOTECAS/ACIONAMENTO">ACIONAMENTO</a> foi desenvolvida a fim de simplificar o processo de movimentação dos Carros.
  
```c++
ACIONAMENTO(portPWM, portDigital);        //Construtor define as portas como saída
OUT(int value);                           //Altera os valores da porta de saída de acordo com "value".
STOP();                                   //Anula os valores das portas de saída.
```

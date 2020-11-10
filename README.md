<h1 align='center'>Robô Paralelo</h1>
<p align='center'>Projeto de um Sistema Surpervisório e Controlador para um Robô Paralelo</p>

## *Estrutura Física*

O sistema físico é constituido pela operação em conjunto de três motores CC a fim de controlar o anexo estrutural posicionado no interior da máquina.

## *Processo*

- [x] Leitura de Encoders
- [x] Acionamento dos Carros
- [x] Implementação do Sistema Supervisório
- [x] Definição dos Parâmetros PID
- [x] Controle de Posição
- [ ] Implementação de modelo matemático

## *Descrição de Etapas*
- `Leitura de Encoders`

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

Com base em tais padrões sequenciais, foi desevolvida a Biblioteca <a href='./BIBLIOTECAS/ENCODER'>ENCODER</a>, com o intuito de automatizar o processo de medição dos sinais referentes aos canais digitais.

```c++
ENCODER(Port1, Port2);        //Método construtor.
registerRead();               //Realiza a leitura da Porta 2.
getPort1();                   //Retorna o endereço da Porta 1.
getPort2();                   //Retorna o endereço da Porta 2.
```
A biblioteca é aplicada em anexo a uma interrupção aplicada ao `Canal 1`, parametrizada para `RISING`, para cada Encoder.

- `Acionamento dos Carros`

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
###### Positiva 

###### Negativa


Com base em tais padrões sequenciais, foi desevolvida a Biblioteca <a href='./BIBLIOTECAS/ENCODER'>ENCODER</a>, com o intuito de automatizar o processo de medição dos sinais referentes aos canais digitais.

```c++
ENCODER(port_1, port_2);      //Método construtor, define as portas como entrada.
registerRead();               //Realiza a leitura da port_2.
getPort1();                   //Retorna o endereço da port_1.
getPort2();                   //Retorna o endereço da port_2.
```


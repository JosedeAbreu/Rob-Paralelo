// bibliotecas
#include <ACIONAMENTO.h>
#include <ENCODER.h>

// Leitura deslocamento - CARRO C
#define encoder_BASE_C1 2
#define encoder_BASE_C2 3

// Porta de sentido e intensidade - CARRO C
#define SENTIDO_out 11
#define PWM_out 12

// Botões Acionamento
#define botao_C A0
#define botao_B A1
#define potenciometro A3

// Instânciando objetos de Leitura(ENCODER) e Acionamento(ACIONAMENTO)
ENCODER carroC_ENCODER(encoder_BASE_C1, encoder_BASE_C2);
ACIONAMENTO carroC_ACIONAMENTO(SENTIDO_out, PWM_out);

// Função para contagem de pulsos
void contagem() {
  bool newSample_port2;

  newSample_port2 = carroC_ENCODER.registerRead();

  //  contagem negativa
  if (newSample_port2) {
    carroC_ENCODER.pulsos--;
  } 
  //  contagem positiva
  else if (!(newSample_port2)) {
    carroC_ENCODER.pulsos++;
  }
  
  carroC_ENCODER.pose = (carroC_ENCODER.pulsos * 0.0125);
}

void setup() {
  Serial.begin(9600);

  pinMode(botao_C, INPUT_PULLUP);
  pinMode(botao_B, INPUT_PULLUP);
  pinMode(potenciometro, INPUT);  

  attachInterrupt(digitalPinToInterrupt(carroC_ENCODER.getPort1()),  contagem, RISING);
}

void loop() {

  // Imprime o deslocamento do carro e a contagem de pulsos
  Serial.print(carroC_ENCODER.pose);
  Serial.print(" ");
  Serial.println(carroC_ENCODER.pulsos);

  // Leitura dos botões de acionemanto
  bool bot_C = !(digitalRead(botao_C));
  bool bot_B = !(digitalRead(botao_B));

  //ACIONAMENTO P/ CIMA
  if (bot_C && !(bot_B)) { 
      uint8_t pwm_value = (map(analogRead(potenciometro), 0, 1024, 0, 255));
      carroC_ACIONAMENTO.OUT(-(pwm_value));
  }
  // ACIONAMENTO P/ BAIXO
  else if(bot_B && !(bot_C)){
      uint8_t pwm_value = map(analogRead(potenciometro), 0, 1024, 0, 255);
      carroC_ACIONAMENTO.OUT(pwm_value);
  }
  else{
      carroC_ACIONAMENTO.STOP();
  }
}

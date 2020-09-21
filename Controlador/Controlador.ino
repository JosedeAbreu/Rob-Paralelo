// bibliotecas
#include <PID.h>
#include <ACIONAMENTO.h>
#include <ENCODER.h>

// Portas Leitura

#define encoder_BASE_A1 10
#define encoder_BASE_A2 11

#define encoder_BASE_B1 17
#define encoder_BASE_B2 16

#define encoder_BASE_C1 27
#define encoder_BASE_C2 29

#define fimcurso_A1     12
#define fimcurso_A2     13

#define fimcurso_B1     15
#define fimcurso_B2     14

#define fimcurso_C1     31
#define fimcurso_C2     33

// Portas Acionamento - CARRO C
#define SENTIDO_Aout 8
#define SENTIDO_Bout 19
#define SENTIDO_Cout 23

#define PWM_Aout 9
#define PWM_Bout 18
#define PWM_Cout 25

// GANHOS 
#define KP_A 0
#define KI_A 0
#define KD_A 0

#define KP_B 0
#define KI_B 0
#define KD_B 0

#define KP_C 0
#define KI_C 0
#define KD_C 0

// Instânciando objetos
ACIONAMENTO ACIONAMENTO_A(SENTIDO_Aout, PWM_Aout);
ACIONAMENTO ACIONAMENTO_B(SENTIDO_Bout, PWM_Bout);
ACIONAMENTO ACIONAMENTO_C(SENTIDO_Cout, PWM_Cout);

ENCODER carroA_ENCODER(encoder_BASE_A1,encoder_BASE_A2);
ENCODER carroB_ENCODER(encoder_BASE_B1,encoder_BASE_B2);
ENCODER carroC_ENCODER(encoder_BASE_C1,encoder_BASE_C2);

// Parametros para controle
PID PIDA(KP_A, KI_A, KD_A, -255, 255);                           
PID PIDB(KP_B, KI_B, KD_B, -255, 255);                                  
PID PIDC(KP_C, KI_C, KD_C, -255, 255);                                 

unsigned long lastComputeTime[3];
unsigned long tempo[3];
float dt[3];


//  Acionamento PID
void UP_PIDA(){
      tempo[0] = (millis() - lastComputeTime[0]);     
      dt[0] = (tempo[0])/1000.0f;   
      
      float out = PIDA.compute(dt[0]);

      lastComputeTime[0] = millis();

      ACIONAMENTO_A.OUT(out);
}
void UP_PIDB(){
      tempo[1] = (millis() - lastComputeTime[1]);     
      dt[1] = (tempo[1])/1000.0f;   
      
      float out = PIDB.compute(dt[1]);

      lastComputeTime[1] = millis();

      ACIONAMENTO_B.OUT(out);
}
void UP_PIDC(){
      tempo[2] = (millis() - lastComputeTime[2]);     
      dt[2] = (tempo[2])/1000.0f;   
      
      float out = PIDC.compute(dt[2]);

      lastComputeTime[2] = millis();

      ACIONAMENTO_A.OUT(out);
}

// Função contadora de pulsos
void contagem_A() { 
      bool newSample_port2;
    
      newSample_port2 = carroA_ENCODER.registerRead();
    
      //contagem negativa
      if (newSample_port2) {
        carroA_ENCODER.pulsos--;
      } 
      
      //contagem positiva
      else if (!(newSample_port2)) {
        carroA_ENCODER.pulsos++;
      }
      
      carroA_ENCODER.pose = (carroA_ENCODER.pulsos * 0.0125);
      PIDA.addInput(carroA_ENCODER.pose);
}
void contagem_B() { 
      bool newSample_port2;
    
      newSample_port2 = carroB_ENCODER.registerRead();
    
      //contagem negativa
      if (newSample_port2) {
        carroB_ENCODER.pulsos--;
      } 
      
      //contagem positiva
      else if (!(newSample_port2)) {
        carroB_ENCODER.pulsos++;
      }
      
      carroB_ENCODER.pose = (carroB_ENCODER.pulsos * 0.0125);
      PIDB.addInput(carroB_ENCODER.pose);
}
void contagem_C() { 
      bool newSample_port2;
    
      newSample_port2 = carroC_ENCODER.registerRead();
    
      //contagem negativa
      if (newSample_port2) {
        carroC_ENCODER.pulsos--;
      } 
      
      //contagem positiva
      else if (!(newSample_port2)) {
        carroC_ENCODER.pulsos++;
      }
      
      carroC_ENCODER.pose = (carroC_ENCODER.pulsos * 0.0125);
      PIDC.addInput(carroC_ENCODER.pose);
}

// Funções de parada
void STOP_A(){
      ACIONAMENTO_A.STOP();
      PIDA.setSetPoint(carroA_ENCODER.pose);
}
void STOP_B(){
      ACIONAMENTO_B.STOP();
      PIDB.setSetPoint(carroB_ENCODER.pose);
}
void STOP_C(){
      ACIONAMENTO_C.STOP();
      PIDC.setSetPoint(carroC_ENCODER.pose);
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);

  attachInterrupt(digitalPinToInterrupt(carroA_ENCODER.getPort1()),  contagem_A, RISING);
  attachInterrupt(digitalPinToInterrupt(carroB_ENCODER.getPort1()),  contagem_B, RISING);
  attachInterrupt(digitalPinToInterrupt(carroC_ENCODER.getPort1()),  contagem_C, RISING);  

  attachInterrupt(digitalPinToInterrupt(fimcurso_A1),  STOP_A, HIGH);
  attachInterrupt(digitalPinToInterrupt(fimcurso_A2),  STOP_A, HIGH);

  attachInterrupt(digitalPinToInterrupt(fimcurso_B1),  STOP_B, HIGH);
  attachInterrupt(digitalPinToInterrupt(fimcurso_B2),  STOP_B, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(fimcurso_C1),  STOP_C, HIGH);
  attachInterrupt(digitalPinToInterrupt(fimcurso_C2),  STOP_C, HIGH);

  lastComputeTime[0] = millis();
  lastComputeTime[1] = millis();
  lastComputeTime[2] = millis();
}

void loop() {  
  if(Serial.available() > 0){
    // PADRÃO: (SETPOINT) 'S'+(CARRO)+VALOR+'\n'
    String msg = Serial.readString();
    msg.replace("\n",""); 
    
    if(msg.startsWith("SA")){
      msg.replace("SA","");
      PIDA.setSetPoint(msg.toInt());
    }
    else if(msg.startsWith("SB")){
      msg.replace("SB","");
      PIDB.setSetPoint(msg.toInt());
    }
    else if(msg.startsWith("SC")){
      msg.replace("SC","");
      PIDC.setSetPoint(msg.toInt());
    }
    
  }

  Serial.print(carroC_ENCODER.pose);
  Serial.print(" ");
  Serial.println(carroC_ENCODER.pulsos);
  UP_PIDA();
  UP_PIDB();
  UP_PIDC(); 
}


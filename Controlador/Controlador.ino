// bibliotecas
#include <PID.h>
#include <ACIONAMENTO.h>
#include <ENCODER.h>

// Portas Leitura

#define encoder_BASE_A1 10
#define encoder_BASE_A2 11

#define encoder_BASE_B1 16
#define encoder_BASE_B2 17

#define encoder_BASE_C1 29
#define encoder_BASE_C2 27

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
PID PIDA(KP_A, KI_A, KD_A, -250, 250);                           
PID PIDB(KP_B, KI_B, KD_B, -250, 250);                                  
PID PIDC(KP_C, KI_C, KD_C, -250, 250);                                 

unsigned long lastComputeTime[3] = {0,0,0};

int TIME_INICIAL=0;
int TIME_EXECUTE=0;

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
      PIDA.addInput((int)carroA_ENCODER.pose);
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
      PIDB.addInput((int)carroB_ENCODER.pose);
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
      PIDC.addInput((int)carroC_ENCODER.pose);
}

//  Acionamento PID
void UP_PIDA(){
      float tempo = (micros() - lastComputeTime[0]);     
      float dt = (tempo)/1000.0f;   
      
      float out = PIDA.compute(dt);
      
      lastComputeTime[0] = micros();
      
      ACIONAMENTO_A.OUT(out);
      WRITE_OUTPUT('A',out);
}
void UP_PIDB(){
      float tempo = (micros() - lastComputeTime[1]);     
      float dt = (tempo)/1000.0f;   
      
      float out = PIDB.compute(dt);

      lastComputeTime[1] = micros();

      ACIONAMENTO_B.OUT(out);
      WRITE_OUTPUT('B',out);
}
void UP_PIDC(){
      float tempo = (micros() - lastComputeTime[2]);     
      float dt = (tempo)/1000.0f;   
      
      float out = PIDC.compute(dt);
      
      lastComputeTime[2] = micros();
       
      ACIONAMENTO_C.OUT(out);
      WRITE_OUTPUT('C',out);
}

// Funções de parada
void STOP_A(){
      ACIONAMENTO_A.STOP();
}
void STOP_B(){
      ACIONAMENTO_B.STOP();
}
void STOP_C(){
      ACIONAMENTO_C.STOP();
}

void READ_SERIAL(){
    if(Serial.available() > 0){
      String msg = Serial.readString();
      msg.replace("\n",""); 
      
      if(msg.startsWith("S")){
            if(msg.startsWith("SA")){
              msg.replace("SA","");
              float value = msg.toInt();
              value = value/100;
              PIDA.setSetPoint(value);
            }
            else if(msg.startsWith("SB")){
              msg.replace("SB","");
              float value = msg.toInt();
              value = value/100;
              PIDB.setSetPoint(value);
            }
            else if(msg.startsWith("SC")){
              msg.replace("SC","");
              float value = msg.toInt();
              value = value/100;
              PIDC.setSetPoint(value);
            }
     }
     else if(msg.startsWith("P")){
            if(msg.startsWith("PA")){
              msg.replace("PA","");
              float value = msg.toInt();
              value = value/100;
              PIDA.setKP(value);
            }
            else if(msg.startsWith("PB")){
              msg.replace("PB","");
              float value = msg.toInt();
              value = value/100;
              PIDB.setKP(value);
            }
            else if(msg.startsWith("PC")){
              msg.replace("PC","");
              float value = msg.toInt();
              value = value/100;
              PIDC.setKP(value);
            }
     }
     else if(msg.startsWith("I")){
            if(msg.startsWith("IA")){
              msg.replace("IA","");
              float value = msg.toInt();
              value = value/100;
              PIDA.setKI(value);
            }
            else if(msg.startsWith("IB")){
              msg.replace("IB","");
              float value = msg.toInt();
              value = value/100;
              PIDB.setKI(value);
            }
            else if(msg.startsWith("IC")){
              msg.replace("IC","");
              float value = msg.toInt();
              value = value/100;
              PIDC.setKI(value);
            }
     }
     else if(msg.startsWith("D")){
            if(msg.startsWith("DA")){
              msg.replace("DA","");
              float value = msg.toInt();
              value = value/100;
              PIDA.setKD(value);
            }
            else if(msg.startsWith("DB")){
              msg.replace("DB","");
              float value = msg.toInt();
              value = value/100;
              PIDB.setKD(value);
            }
            else if(msg.startsWith("DC")){
              msg.replace("DC","");
              float value = msg.toInt();
              value = value/100;
              PIDC.setKD(value);
            }
     }
   }
}

void WRITE_INPUT(){
  Serial.print("I");
  Serial.print("A");
  int multiplication = carroA_ENCODER.pose * 100;
  Serial.print(multiplication);
  Serial.print("\n");

  Serial.print("I");
  Serial.print("B");
  multiplication = carroB_ENCODER.pose * 100;
  Serial.print(multiplication);
  Serial.print("\n");

  Serial.print("I");
  Serial.print("C");
  multiplication = carroC_ENCODER.pose * 100;
  Serial.print(multiplication);
  Serial.print("\n");
}

void WRITE_TIME(int miliseg){
  Serial.print("T");
  Serial.print((int)miliseg);
  Serial.print("\n");
}

void WRITE_OUTPUT(char carro, float out){
  Serial.print("O");
  Serial.print(carro);
  Serial.print(out);
  Serial.print("\n");
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);

  attachInterrupt(digitalPinToInterrupt(carroA_ENCODER.getPort1()),  contagem_A, RISING);
  attachInterrupt(digitalPinToInterrupt(carroB_ENCODER.getPort1()),  contagem_B, RISING);
  attachInterrupt(digitalPinToInterrupt(carroC_ENCODER.getPort1()),  contagem_C, RISING);  

  /*attachInterrupt(digitalPinToInterrupt(fimcurso_A1),  STOP_A, HIGH);
  attachInterrupt(digitalPinToInterrupt(FIM_A.getPort2()),  STOP_A, HIGH);

  attachInterrupt(digitalPinToInterrupt(FIM_B.getPort1()),  STOP_B, HIGH);
  attachInterrupt(digitalPinToInterrupt(FIM_B.getPort2()),  STOP_B, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(FIM_C.getPort1()),  STOP_C, HIGH);
  attachInterrupt(digitalPinToInterrupt(FIM_C.getPort2()),  STOP_C, HIGH);*/

  carroA_ENCODER.pose = 0;
  carroB_ENCODER.pose = 0;
  carroC_ENCODER.pose = 0;

  PIDA.reset();
  PIDB.reset();
  PIDC.reset();

  PIDA.addInput(0);
  PIDB.addInput(0);
  PIDC.addInput(0);

  PIDA.setSetPoint(0);
  PIDB.setSetPoint(0);
  PIDC.setSetPoint(0);

  lastComputeTime[0] = millis();
  lastComputeTime[1] = millis();
  lastComputeTime[2] = millis();

  TIME_INICIAL = millis();
}

void loop(){  
  READ_SERIAL();
  WRITE_INPUT();
  
  int NEW_TIME = (millis() - TIME_INICIAL);
  if((NEW_TIME-TIME_EXECUTE)>10){
    TIME_EXECUTE = NEW_TIME;
    WRITE_TIME(TIME_EXECUTE);
  }
  
  UP_PIDA();
  UP_PIDB();
  UP_PIDC();
}


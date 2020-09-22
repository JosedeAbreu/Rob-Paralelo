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

ENCODER FIM_A(fimcurso_A1,fimcurso_A2);
ENCODER FIM_B(fimcurso_B1,fimcurso_B2);
ENCODER FIM_C(fimcurso_C1,fimcurso_C2);

// Parametros para controle
PID PIDA(KP_A, KI_A, KD_A, -255, 255);                           
PID PIDB(KP_B, KI_B, KD_B, -255, 255);                                  
PID PIDC(KP_C, KI_C, KD_C, -255, 255);                                 

unsigned long lastComputeTime[3] = {0,0,0};

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

void READ_SERIAL(){
    if(Serial.available() > 0){
      String msg = Serial.readString();
      msg.replace("\n",""); 
      
      if(msg.startsWith("S")){
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
     else if(msg.startsWith("P")){
            if(msg.startsWith("PA")){
              msg.replace("PA","");
              PIDA.setKP(msg.toInt());
            }
            else if(msg.startsWith("PB")){
              msg.replace("PB","");
              PIDB.setKP(msg.toInt());
            }
            else if(msg.startsWith("PC")){
              msg.replace("PC","");
              PIDC.setKP(msg.toInt());
            }
     }
     else if(msg.startsWith("I")){
            if(msg.startsWith("IA")){
              msg.replace("IA","");
              PIDA.setKI(msg.toInt());
            }
            else if(msg.startsWith("IB")){
              msg.replace("IB","");
              PIDB.setKI(msg.toInt());
            }
            else if(msg.startsWith("IC")){
              msg.replace("IC","");
              PIDC.setKI(msg.toInt());
            }
     }
     else if(msg.startsWith("D")){
            if(msg.startsWith("DA")){
              msg.replace("DA","");
              PIDA.setKD(msg.toInt());
            }
            else if(msg.startsWith("DB")){
              msg.replace("DB","");
              PIDB.setKD(msg.toInt());
            }
            else if(msg.startsWith("DC")){
              msg.replace("DC","");
              PIDC.setKD(msg.toInt());
            }
     }
   }
}

void WRITE_INPUT(){
  Serial.print("I");
  Serial.print("A");
  Serial.print((int)carroA_ENCODER.pose);
  Serial.print("\n");

  Serial.print("I");
  Serial.print("B");
  Serial.print((int)carroB_ENCODER.pose);
  Serial.print("\n");

  Serial.print("I");
  Serial.print("C");
  Serial.print((int)carroC_ENCODER.pose);
  Serial.print("\n");
}

void WRITE_OUTPUT(char carro, float out){
  Serial.print("O");
  Serial.print(carro);
  Serial.print((int)out);
  Serial.print("\n");
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);

  attachInterrupt(digitalPinToInterrupt(carroA_ENCODER.getPort1()),  contagem_A, RISING);
  attachInterrupt(digitalPinToInterrupt(carroB_ENCODER.getPort1()),  contagem_B, RISING);
  attachInterrupt(digitalPinToInterrupt(carroC_ENCODER.getPort1()),  contagem_C, RISING);  

  /*attachInterrupt(digitalPinToInterrupt(FIM_A.getPort1()),  STOP_A, HIGH);
  attachInterrupt(digitalPinToInterrupt(FIM_A.getPort2()),  STOP_A, HIGH);

  attachInterrupt(digitalPinToInterrupt(FIM_B.getPort1()),  STOP_B, HIGH);
  attachInterrupt(digitalPinToInterrupt(FIM_B.getPort2()),  STOP_B, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(FIM_C.getPort1()),  STOP_C, HIGH);
  attachInterrupt(digitalPinToInterrupt(FIM_C.getPort2()),  STOP_C, HIGH);*/

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
}

void loop(){  
  READ_SERIAL();
  WRITE_INPUT();
  
  UP_PIDA();
  UP_PIDB();
  UP_PIDC(); 
}


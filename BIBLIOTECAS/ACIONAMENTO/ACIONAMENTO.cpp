#include "Arduino.h"
#include "ACIONAMENTO.h"

ACIONAMENTO::ACIONAMENTO(int _port_sentido, int _port_PWM){
      port_sentido = _port_sentido;
      port_PWM = _port_PWM;
 
      pinMode(port_sentido, OUTPUT);
      pinMode(port_PWM, OUTPUT);
}

void ACIONAMENTO::OUT(int value){
  if(value < 0){
	value = value * -1;
  	analogWrite(port_PWM, value);
  	digitalWrite(port_sentido, LOW);
  }
  else{
  	analogWrite(port_PWM, value);
  	digitalWrite(port_sentido, HIGH);
  }
}
void ACIONAMENTO::STOP(){
  analogWrite(port_PWM, 0);
  digitalWrite(port_sentido, LOW);
}


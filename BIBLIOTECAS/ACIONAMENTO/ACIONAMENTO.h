#ifndef ACIONAMENTO_h
#define ACIONAMENTO_h

#include "Arduino.h"

class ACIONAMENTO{
  public:
    
    ACIONAMENTO(int _port_sentido, int _port_PWM);
    void OUT(int value);
    void STOP();
   
  private:
    int8_t port_sentido, port_PWM;
};

#endif

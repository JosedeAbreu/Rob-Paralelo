/*
 *        ENCODER.h      
 *        Autor: JOSÉ LUCAS ABREU
 *        
 *
 */

 #ifndef ENCODER_h
 #define ENCODER_h

 #include "Arduino.h"

 class ENCODER{
  public:
    volatile float pose = 0;
    volatile int pulsos = 0;
    
    ENCODER(uint8_t _port1, uint8_t _port2);

    bool registerRead();

    uint8_t getPort1();
    uint8_t getPort2();
    
  private:
    int port_encoder1, port_encoder2;
 };

 #endif

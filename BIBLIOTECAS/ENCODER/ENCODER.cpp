/*
 *        ENCODER.cpp      
 *        Autor: JOSÉ LUCAS ABREU
 *        
 *
 */

 #include "Arduino.h"
 #include "ENCODER.h"

 ENCODER::ENCODER(uint8_t _port1, uint8_t _port2) {
      port_encoder1 = _port1;
      port_encoder2 = _port2;

      pinMode(port_encoder1, INPUT);
      pinMode(port_encoder2, INPUT);
 }

 bool ENCODER::registerRead() {
      bool sample = PIO_Get(g_APinDescription[port_encoder2].pPort , PIO_INPUT, g_APinDescription[port_encoder2].ulPin);
      return sample;
 }

 uint8_t ENCODER::getPort1() {
      return port_encoder1;
 }

 uint8_t ENCODER::getPort2() {
      return port_encoder2;
 }

#include <ENCODER.h>

#define encoder_BASE_C1 2
#define encoder_BASE_C2 3

/*class ENCODER {
  public:
    volatile float pose = 0;
    volatile int pulsos = 0;

    ENCODER(uint8_t _port1, uint8_t _port2) {
      port_encoder1 = _port1;
      port_encoder2 = _port2;

      pinMode(port_encoder1, INPUT);
      pinMode(port_encoder2, INPUT);

    }

    bool registerRead() {
      bool sample = PIO_Get(g_APinDescription[port_encoder2].pPort , PIO_INPUT, g_APinDescription[port_encoder2].ulPin);
      return sample;
    }

    uint8_t getPort1() {
      return port_encoder1;
    }
    uint8_t getPort2() {
      return port_encoder2;
    }

  private:
    int port_encoder1, port_encoder2;

};*/

ENCODER carroC_ENCODER(encoder_BASE_C1, encoder_BASE_C2);

void contagem() {
  bool newSample_port2;

  newSample_port2 = carroC_ENCODER.registerRead();

  if (newSample_port2) {
    //contagem negativa
    carroC_ENCODER.pulsos--;
  } else if (!(newSample_port2)) {
    //contagem positiva
    carroC_ENCODER.pulsos++;
  }
  carroC_ENCODER.pose = (carroC_ENCODER.pulsos * 0.0125);
}




void setup() {
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(carroC_ENCODER.getPort1()),  contagem, RISING);

}



void loop() {
  Serial.print(carroC_ENCODER.pose);
  Serial.print(" ");
  Serial.println(carroC_ENCODER.pulsos);
}

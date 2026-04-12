#ifndef LORA_H
#define LORA_H

#include <Arduino.h>

class Lora {
  private:
    int myRX;
    int myTX;
    HardwareSerial LoraSerial;

  public:
    Lora(int theRX, int theTX);

    bool begin();
    bool sendData(String theMessage);
    bool status();
};

#endif
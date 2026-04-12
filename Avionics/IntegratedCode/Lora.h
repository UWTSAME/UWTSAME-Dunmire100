#ifndef LORA_H
#define LORA_H

class Lora {
  private:
    int myRX;
    int myTX;
    HardwareSerial LoraSerial(1);

  public:
    Lora(int theRX, int theTX);

    bool begin();
    bool sendData(String theMessage);
    bool status();
};

#endif
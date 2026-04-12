#include "Lora.h"

Lora::Lora(int theRX, int theTX): myRX(theRX), myTX(theTX){
}

bool Lora::begin(){
  LoraSerial.begin(115200, SERIAL_8N1, myRX, myTX);
  delay(500);
  return (LoraSerial.availabel() > 0);
}

bool Lora::sendData(String theMessage){
  bool returner = false;
  if (status()) {
    String compiledMessage = "AT+SEND=0," + String(theMessage.length()) + "," + theMessage;
    LoraSerial.println(compiledMessage);
    returner = true;
}

bool Lora::Status(){
  bool status = false;
  unsigned long deltaTime = millis();
  unsigned int attempts = 0;
  unsigned const int ATTEMPTS_ALLOWED = 5;

  while (ATTEMPTS_ALLOWED > attempts && !status) {

    attempts++;
    LoraSerial.println("AT");

    delay(200);

    if (LoraSerial.available()) {
      String response = LoraSerial.readString();
      if (response.indexOf("+OK") != -1) {
        status = true;
      }
    }
  }

  return status;
}
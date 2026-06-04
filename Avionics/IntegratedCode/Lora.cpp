#include "Lora.h"

Lora::Lora(int theRX, int theTX): LoraSerial(1), myRX(theRX), myTX(theTX), lastSendTime(0){
}

bool Lora::begin(){
  LoraSerial.begin(115200, SERIAL_8N1, myRX, myTX);
  delay(1000);
  return true;
}

bool Lora::sendData(String theMessage){

  if (millis() - lastSendTime < 3000) {
    return false;
  }
  lastSendTime = millis();

  if (status()) {
    String compiledMessage = "AT+SEND=0," + String(theMessage.length()) + "," + theMessage;
    Serial.println("Command: " + compiledMessage);
    LoraSerial.println(compiledMessage); 

    unsigned long startTime = millis(); 

    while (millis() - startTime < 1000) { 
      if (LoraSerial.available()) { 

        String response = LoraSerial.readStringUntil('\n');
        response.trim(); 

        if (response.indexOf("+OK") != -1) {
          return true;
        }

        if (response.indexOf("+ERR") != -1) {
          return false;
        }
      }
    }
    return false;
  }
  return false;
}

bool Lora::status(){
  bool statusGood = false;
  unsigned int attempts = 0;
  unsigned const int ATTEMPTS_ALLOWED = 5;

  while (ATTEMPTS_ALLOWED > attempts && !statusGood) {

    attempts++;

    while (LoraSerial.available()) {
      LoraSerial.read();
    }

    LoraSerial.println("AT");

    delay(200);

    if (LoraSerial.available()) {
      String response = LoraSerial.readStringUntil('\n');
      response.trim(); 
      if (response.indexOf("+OK") != -1) {
        statusGood = true;
      }
    }
  }
  return statusGood;
}